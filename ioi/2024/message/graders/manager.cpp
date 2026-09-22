#include "testlib.h"
#include <csignal>
#include <random>

using namespace std;


/******************************** Begin testlib-related material ********************************/

inline FILE* openFile(const char* name, const char* mode) {
	FILE* file = fopen(name, mode);
	if (!file)
		quitf(_fail, "Could not open file '%s' with mode '%s'.", name, mode);
	closeOnHalt(file);
	return file;
}


vector<FILE*> mgr2sol, sol2mgr;
FILE* log_file = nullptr;

void nullifyFile(int idx) {
	mgr2sol[idx] = sol2mgr[idx] = nullptr;
}

#ifdef __GNUC__
__attribute__ ((format (printf, 1, 2)))
#endif
void log_printf(const char* fmt, ...) {
	if (log_file) {
		FMT_TO_RESULT(fmt, fmt, message);
		fprintf(log_file, "%s", message.c_str());
		fflush(log_file);
	}
}

void registerManager(std::string probName, int num_processes, int argc, char* argv[]) {
	setName("manager for problem %s", probName.c_str());
	__testlib_ensuresPreconditions();
	testlibMode = _checker;
	random_t::version = 1; // Random generator version
	__testlib_set_binary(stdin);
	ouf.mode = _output;

	{//Keep alive on broken pipes
		//signal(SIGPIPE, SIG_IGN);
		struct sigaction sa;
		sa.sa_handler = SIG_IGN;
		sigaction(SIGPIPE, &sa, NULL);
	}

	int required_args = 1 + 2 * num_processes;
	if (argc < required_args || required_args+1 < argc) {
		string usage = format("'%s'", argv[0]);
		for (int i = 0; i < num_processes; i++)
			usage += format(" sol%d-to-mgr mgr-to-sol%d", i, i);
		usage += " [mgr_log] < input-file";
		quitf(_fail,
			"Manager for problem %s:\n"
			"Invalid number of arguments: %d\n"
			"Usage: %s",
			probName.c_str(), argc-1, usage.c_str());
	}

	inf.init(stdin, _input);
	closeOnHalt(stdout);
	closeOnHalt(stderr);

	mgr2sol.resize(num_processes);
	sol2mgr.resize(num_processes);
	for (int i = 0; i < num_processes; i++) {
		mgr2sol[i] = openFile(argv[1 + 2*i + 1], "a");
		sol2mgr[i] = openFile(argv[1 + 2*i + 0], "r");
	}

	if (argc > required_args) {
		log_file = openFile(argv[required_args], "w");
	} else {
		log_file = nullptr;
	}
}
/********************************* End testlib-related material *********************************/


// utils

#define rep(i, n) for(int i = 0, i##__n = (int)(n); i < i##__n; ++i)

template<class C> int sz(const C& c) { return std::size(c); }


// grader/manager protocol

const int secret_g2m = 0x647FB390;
const int secret_m2g = 0xB107D5C0;
const int code_mask  = 0x0000000F;

const int M2G_CODE__OK = 0;
const int M2G_CODE__DIE = 1;

const int G2M_CODE__OK = 0;
const int G2M_CODE__OK_END_OF_PACKETS = 1;
const int G2M_CODE__PACKET_SIZE_INCORRECT = 2;
const int G2M_CODE__RETURN_MESSAGE_TOO_LARGE = 3;
const int G2M_CODE__PV_CALL_EXIT = 13;
const int G2M_CODE__PV_TAMPER_M2G = 14;
const int G2M_CODE__SILENT = 15;


int fifo_idx = 0;
bool mode_end_of_packets;

void out_flush() {
	fflush(mgr2sol[fifo_idx]);
}

void write_int(int x) {
	FILE* fout = mgr2sol[fifo_idx];
	if (1 != fwrite(&x, sizeof(x), 1, fout)) {
		nullifyFile(fifo_idx);
		log_printf("Could not write int to mgr2sol[%d]\n", fifo_idx);
	}
}

void write_int_array(const int* arr, int len) {
	FILE* fout = mgr2sol[fifo_idx];
	if (int ret = fwrite(arr, sizeof(int), len, fout); len != ret) {
		nullifyFile(fifo_idx);
		log_printf("Could not write int array of size %d to mgr2sol[%d], fwrite returned %d\n", len, fifo_idx, ret);
	}
}

void write_int_vector(const vector<int>& v) {
	write_int_array(v.data(), sz(v));
}


void write_secret(int m2g_code = M2G_CODE__OK) {
	write_int(secret_m2g | m2g_code);
}

#ifdef __GNUC__
__attribute__ ((format (printf, 2, 3)))
#endif
NORETURN void die(TResult result, const char* format, ...) {
	FMT_TO_RESULT(format, format, message);
	log_printf("Dying with message '%s'\n", message.c_str());
	rep(i, sz(mgr2sol))
		if(mgr2sol[i] != nullptr) {
			fifo_idx = i;
			log_printf("Sending secret with code DIE to mgr2sol[%d]\n", fifo_idx);
			write_secret(M2G_CODE__DIE);
			out_flush();
		}
	log_printf("Quitting with result code %d\n", int(result));
	quit(result, message);
}

NORETURN void die_invalid_argument(const string &msg) {
	RESULT_MESSAGE_WRONG += ": Invalid argument";
	die(_wa, "%s", msg.c_str());
}

NORETURN void die_too_many_calls(const string &msg) {
	RESULT_MESSAGE_WRONG += ": Too many calls";
	die(_wa, "%s", msg.c_str());
}

int read_int() {
	FILE* fin = sol2mgr[fifo_idx];
	int x;
	if (1 != fread(&x, sizeof(x), 1, fin)) {
		nullifyFile(fifo_idx);
		die(_fail, "Could not read int from sol2mgr[%d]", fifo_idx);
	}
	return x;
}

void read_int_array(int* arr, int len) {
	FILE* fin = sol2mgr[fifo_idx];
	if (int ret = fread(arr, sizeof(int), len, fin); len != ret) {
		nullifyFile(fifo_idx);
		die(_fail, "Could not read int array of size %d from sol2mgr[%d], fread returned %d", len, fifo_idx, ret);
	}
}


void read_secret() {
	int secret = read_int();
	if((secret & ~code_mask) != secret_g2m)
		die(_pv, "Possible tampering with sol2mgr[%d]", fifo_idx);
	int g2m_code = secret & code_mask;
	mode_end_of_packets = false;
	switch (g2m_code) {
		case G2M_CODE__OK:
			return;
		case G2M_CODE__OK_END_OF_PACKETS:
			mode_end_of_packets = true;
			return;
		case G2M_CODE__SILENT:
			die(_fail, "Unexpected g2m_code SILENT from sol2mgr[%d]", fifo_idx);
		case G2M_CODE__PV_TAMPER_M2G:
			die(_pv, "Possible tampering with mgr2sol[%d]", fifo_idx);
		case G2M_CODE__PV_CALL_EXIT:
			die(_pv, "Solution[%d] called exit()", fifo_idx);
		case G2M_CODE__PACKET_SIZE_INCORRECT:
			die_invalid_argument("Packet size is incorrect");
		case G2M_CODE__RETURN_MESSAGE_TOO_LARGE:
			die(_wa, "Returned message's length is too large.");
		default:
			die(_fail, "Unknown g2m_code %d from sol2mgr[%d]", g2m_code, fifo_idx);
	}
}


const int PACKET_SIZE = 31;
const int CALLS_CNT_LIMIT = 100;

//mt19937 rng(15061994);
//auto rnd1 =  bind(uniform_int_distribution<int>(0, (1 << 1) - 1), rng);
//auto rnd4 =  bind(uniform_int_distribution<int>(0, (1 << 4) - 1), rng);
//auto rnd31 = bind(uniform_int_distribution<int>(0, (1U << 31) - 1), rng);
int used_days, max_used_days = 0;

int compressed_len(int len) { return (len+31) / 32; }

string vi2binStr(const vector<int>& arr, int len) {
	string str(len, '*');
	rep(i, len) str[i] = (arr[i / 32] >> (i & 31) & 1) + '0';
	return str;
}

enum Strategy {
	BEGIN,
	kExampleStrategy,
	kRandom,
	kJustOnes,
	kJustZeroes,
	kMimicAllButOneRound0,
	kMimicAllButOneRound1,
	kMimicAllButOneRound2,
	kMimicAllButOneRound3,
	kMimicAllButOneRound4,
	kInvertAllButOneRound0,
	kInvertAllButOneRound1,
	kInvertAllButOneRound2,
	kInvertAllButOneRound3,
	kInvertAllButOneRound4,
	kMinimizeDiffOfZeroesAndOnes,
	kEchoPreviousResponses,
	kEchoPreviousResponsesShuffled,
	kSame,
	kFlip,
	kMimicClosestOnLeft,
	kMimicClosestOnRight,
	END
};

struct BaseTestCase {
	const int S;
	const vector<int> M_f;
	const int C_f;
};

struct EncoderTestCase {
	const BaseTestCase *base;
	Strategy strategy;
};

struct DecoderTestCase {
	const BaseTestCase *base;
	vector<int> R_f;
};


vector<BaseTestCase> ReadBaseTestCases() {
	int T = inf.readInt();
	vector<BaseTestCase> base_tests;
	for(int _ = 0; _ < T; _++) {
		int S = inf.readInt();
		vector<int> M_f(compressed_len(S), 0);
		for(int i = 0; i < S; i++) {
			int bit = inf.readInt(0, 1);
			M_f[i / 32] |= bit << (i & 31);
		}

		int C_f = 0;
		for(int i = 0; i < PACKET_SIZE; i++) {
			int ally = inf.readInt(0, PACKET_SIZE-1);
			C_f |= ally << i;
		}

		base_tests.push_back(BaseTestCase{
				.S = S,
				.M_f = M_f,
				.C_f = C_f});
	}
	fclose(stdin);
	return base_tests;
}

vector<EncoderTestCase> GenerateEncoderTests(const vector<BaseTestCase> &base_tests) {
	vector<EncoderTestCase> encoder_tests;
	if(base_tests.size() <= 3) { // This is the example test
		for(const auto &base_test : base_tests)
			encoder_tests.push_back(EncoderTestCase{
					.base = &base_test,
					.strategy = Strategy::kExampleStrategy});
	}
	else {
		for(const auto &base_test : base_tests)
			for(int strategy = Strategy::BEGIN + 1; strategy < Strategy::END; strategy++)
				encoder_tests.push_back(EncoderTestCase{
						.base = &base_test,
						.strategy = static_cast<Strategy>(strategy)});
	}

	shuffle(encoder_tests.begin(), encoder_tests.end());
	return encoder_tests;
}

void setbit(int &x, int i, int b) { b == 1 ? x |= (1 << i) : x &= ~(1 << i); }

int ApplyStrategy(const Strategy strategy, const int C_f, int used_days,
		const vector<int>& previous_answers, const vector<int>& who_to_mimic,
		const vector<int>& non_mimic_index, int x) {
	if(strategy == Strategy::kExampleStrategy) {
		int bit = 0;
		for(int i = 0; i < PACKET_SIZE; i++)
			if(C_f >> i & 1) {
				setbit(x, i, bit);
				bit ^= 1;
			}
		return x;
	}

	if(strategy == Strategy::kSame)
		return x;
	if(strategy == Strategy::kFlip)
		return x ^ C_f;
	if(strategy == Strategy::kMimicClosestOnLeft) {
		int st = __builtin_ctz(~C_f);
		int pbit = (x >> st) & 1;
		for(int dis = 1; dis < PACKET_SIZE; dis++) {
			int i = (st + dis) % PACKET_SIZE;
			if(C_f >> i & 1)
				setbit(x, i, pbit);
			pbit = (x >> i) & 1;
		}
		return x;
	}
	if(strategy == Strategy::kMimicClosestOnRight) {
		int st = __builtin_ctz(~C_f);
		int pbit = (x >> st) & 1;
		for(int dis = 1; dis < PACKET_SIZE; dis++) {
			int i = (st - dis + PACKET_SIZE) % PACKET_SIZE;
			if(C_f >> i & 1)
				setbit(x, i, pbit);
			pbit = (x >> i) & 1;
		}
		return x;
	}

	if(strategy == Strategy::kRandom)
		return x ^ (rnd.next(0U, (1U << 31) - 1) & C_f);
	if(strategy == Strategy::kJustOnes)
		return x | C_f;
	if(strategy == Strategy::kJustZeroes)
		return x & ~C_f;

	vector<char> their_choices;
	for(int i = 0; i < PACKET_SIZE; i++)
		if(!(C_f >> i & 1))
			their_choices.push_back(x >> i & 1);

	if(kMimicAllButOneRound0 <= strategy and strategy <= kInvertAllButOneRound4) {
		int id = strategy - kMimicAllButOneRound0;
		if(kInvertAllButOneRound0 <= strategy)
			id = strategy - kInvertAllButOneRound0;
		if(used_days == non_mimic_index[id]) {
			for(int i = 0; i < PACKET_SIZE; i++)
				if(C_f >> i & 1) {
					if(kMimicAllButOneRound0 <= strategy and strategy <= kMimicAllButOneRound4)
						setbit(x, i, x >> who_to_mimic[i] & 1);
					else
						setbit(x, i, (x >> who_to_mimic[i] & 1) ^ 1);
				}
			return x;
		}
		else
			return x ^ (rnd.next(0U, (1U << 31) - 1) & C_f);
	}

	if(strategy == Strategy::kMinimizeDiffOfZeroesAndOnes) {
		int zeroes_count = 0, ones_count = 0;
		for(int previous_answer: previous_answers) {
			for(int i = 0; i < PACKET_SIZE; i++)
				if((previous_answer >> i & 1) == 0)
					zeroes_count++;
				else
					ones_count++;
		}
		for(int i = 0; i < PACKET_SIZE; i++) {
			if(!(C_f >> i & 1)) {
				if((x >> i & 1) == 0)
					zeroes_count++;
				else
					ones_count++;
			}
		}
		vector<int> my_choices;
		for(int i = 0; i < (PACKET_SIZE - 1) / 2; i++) {
			int choice = rnd.next(0, 1);
			if(zeroes_count > ones_count)
				choice = 1;
			if(zeroes_count < ones_count)
				choice = 0;
			if(choice == 0)
				zeroes_count++;
			if(choice == 1)
				ones_count++;
			my_choices.push_back(choice);
		}
		shuffle(my_choices.begin(), my_choices.end());
		int ix = 0;
		for(int i = 0; i < PACKET_SIZE; i++)
			if(C_f >> i & 1)
				setbit(x, i, my_choices[ix++]);
		return x;
	}

	if(strategy == Strategy::kEchoPreviousResponses or
			strategy == Strategy::kEchoPreviousResponsesShuffled) {
		if(used_days == 1) {
			for(int i = 0; i < PACKET_SIZE; i++)
				if(C_f >> i & 1)
					setbit(x, i, rnd.next(0, 1));
		}
		else {
			int previous_response = previous_answers.back();
			vector<int> my_response;
			for(int i = 0; i < PACKET_SIZE; i++)
				if(!(C_f >> i & 1))
					my_response.push_back(previous_response >> i & 1);
			if(strategy == Strategy::kEchoPreviousResponsesShuffled)
				shuffle(my_response.begin(), my_response.end());
			int ix = 0;
			for(int i = 0; i < PACKET_SIZE; i++)
				if(C_f >> i & 1)
					setbit(x, i, my_response[ix++]);
		}
		return x;
	}

	die(_fail, "Unknown strategy!");
}

vector<int> CalculateMimicyMap(int C_f) {
	vector<int> their_indices;
	for(int i = 0; i < PACKET_SIZE; i++) {
		if(!(C_f >> i & 1))
			their_indices.push_back(i);
	}
//	shuffle(their_indices.begin(), their_indices.end(), rng);
	their_indices.erase(their_indices.begin() + rnd.next(16));
	
	vector<int> who_to_mimic(PACKET_SIZE, -1);
	int ix = 0;
	for(int i = 0; i < PACKET_SIZE; i++)
		if(C_f >> i & 1)
			who_to_mimic[i] = their_indices[ix++];
	return who_to_mimic;
}

vector<DecoderTestCase> RunEncoder(vector<EncoderTestCase> &encoder_tests) {
	int test_cases = encoder_tests.size();

	vector<DecoderTestCase> decoder_tests;

	for(int _ = 0; _ < test_cases; _++) {
		const EncoderTestCase &test = encoder_tests[_];

		write_secret();
		write_int(test.base->S);
		write_int_vector(test.base->M_f);
		write_int(test.base->C_f);
		out_flush();

		auto who_to_mimic = CalculateMimicyMap(test.base->C_f);
		vector<int> non_mimic_index(5);
		non_mimic_index[1] = rnd.next(0, 7);
		non_mimic_index[0] = rnd.next(8, 19);
		non_mimic_index[2] = rnd.next(20, 44);
		non_mimic_index[3] = rnd.next(45, 66);
		non_mimic_index[4] = rnd.next(67, 100);

		used_days = 0;
		vector<int> R_f;
		while(true) {
			read_secret();
			if(mode_end_of_packets == true) break;

			int A_f = read_int();

			if(++used_days > CALLS_CNT_LIMIT)
				die_too_many_calls("Used too many days");

			int B_f = ApplyStrategy(test.strategy, test.base->C_f, used_days, R_f, who_to_mimic, non_mimic_index, A_f);

			ensuref(((B_f ^ A_f) | test.base->C_f) == test.base->C_f, "ApplyStrategy should not edit uncontrolled bits");
			
			R_f.push_back(B_f);

			write_secret();
			write_int(B_f);
			out_flush();
		}
		decoder_tests.push_back(DecoderTestCase{
				.base = test.base,
				.R_f = R_f,
				});
		max_used_days = max(max_used_days, used_days);
	}

	write_secret();
	write_int(-1);
	out_flush();

	return decoder_tests;
}

void RunDecoder(const vector<DecoderTestCase> &decoder_tests) {
	int test_cases = decoder_tests.size();
	for(int _ = 0; _ < test_cases; _++) {
		const DecoderTestCase &test = decoder_tests[_];

		write_secret();
		write_int(sz(test.R_f));
		write_int_vector(test.R_f);
		out_flush();

		read_secret();
		int L = read_int();
		vector<int> D_f(compressed_len(L), 0);
		read_int_array(D_f.data(), sz(D_f));

		if(L != test.base->S || D_f != test.base->M_f) {
			log_printf(
					"WA:\n"
					"original message:\n"
					"'%s'\n"
					"decoded message:\n"
					"'%s'\n",
					vi2binStr(test.base->M_f, test.base->S).c_str(),
					vi2binStr(D_f, L).c_str());
	       	die(_wa, "decoded message is incorrect");
		}
	}

	write_secret();
	write_int(-1);
	out_flush();
}

int main(int argc, char **argv) {
	registerManager("message", 2, argc, argv);

	fifo_idx = 0; // Mode: Encoder
	vector<BaseTestCase> base_tests = ReadBaseTestCases();
	vector<EncoderTestCase> encoder_tests = GenerateEncoderTests(base_tests);
	vector<DecoderTestCase> decoder_tests = RunEncoder(encoder_tests);
	nullifyFile(fifo_idx);

	shuffle(decoder_tests.begin(), decoder_tests.end());

	fifo_idx = 1; // Mode: Decoder
	RunDecoder(decoder_tests);
	nullifyFile(fifo_idx);

	// scoring
	const int days_threshold = 66;
	if(max_used_days <= days_threshold)
		quitf(_ok, "Used %d days", max_used_days);
	else if(max_used_days <= CALLS_CNT_LIMIT)
		quitp(pow(0.95, max_used_days - days_threshold), "Used %d days", max_used_days);

	die(_fail, "Reached an unreachable code!!!");

	return 0;
}
