#include "testlib.h"
#include <algorithm>
#include <cassert>
#include <csignal>
#include <cstdio>
#include <memory>
#include <queue>
#include <string>
#include <vector>

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

#define log_var(var_name) log_printf("%s = %s\n", #var_name, toString(var_name).c_str());

template<class C>
void log_array(const C& c, string delim=" ", string ending="\n") {
	string d = "";
	for (const auto& x : c) {
		log_printf("%s%s", d.c_str(), toString(x).c_str());
		d = delim;
	}
	log_printf("%s", ending.c_str());
}


// grader/manager protocol

const int secret_g2m = 0x34508C80;
const int secret_m2g = 0x75EC8020;
const int code_mask  = 0x0000000F;

const int M2G_CODE__OK = 0;
const int M2G_CODE__DIE = 1;

const int G2M_CODE__OK_NEW_EXPERIMENT = 0;
const int G2M_CODE__OK_END_OF_EXPERIMENTS = 1;
const int G2M_CODE__INVALID_E_LENGTH = 2;
const int G2M_CODE__INVALID_G_LENGTH = 3;
const int G2M_CODE__PV_CALL_EXIT = 13;
const int G2M_CODE__PV_TAMPER_M2G = 14;
const int G2M_CODE__SILENT = 15;

int fifo_idx = 0;

enum class ActionMode {
	ok_new_experiment,
	ok_end_of_experiments,
} ok_action_mode;

void out_flush() {
	fflush(mgr2sol[fifo_idx]);
}

void write_int(int x) {
	if(1 > fprintf(mgr2sol[fifo_idx], "%d\n", x)) {
		nullifyFile(fifo_idx);
		log_printf("Could not write int to mgr2sol[%d]\n", fifo_idx);
	}
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
	int x;
	if(1 != fscanf(sol2mgr[fifo_idx], "%d", &x)) {
		nullifyFile(fifo_idx);
		die(_fail, "Could not read int from sol2mgr[%d]", fifo_idx);
	}
	return x;
}

vector<int> read_int_vector(int len) {
	vector<int> v(len);
	rep(i, len)
		v[i] = read_int();
	return v;
}

void read_secret() {
	int secret = read_int();
	if((secret & ~code_mask) != secret_g2m)
		die(_pv, "Possible tampering with sol2mgr[%d]", fifo_idx);
	int g2m_code = secret & code_mask;
	switch (g2m_code) {
		case G2M_CODE__OK_NEW_EXPERIMENT:
			ok_action_mode = ActionMode::ok_new_experiment;
			return;
		case G2M_CODE__OK_END_OF_EXPERIMENTS:
			ok_action_mode = ActionMode::ok_end_of_experiments;
			return;
		case G2M_CODE__INVALID_E_LENGTH:
			die_invalid_argument("Invalid length of E");
		case G2M_CODE__INVALID_G_LENGTH:
			die(_wa, "Invalid length of G");
		case G2M_CODE__SILENT:
			die(_fail, "Unexpected g2m_code SILENT from sol2mgr[%d]", fifo_idx);
		case G2M_CODE__PV_TAMPER_M2G:
			die(_pv, "Possible tampering with mgr2sol[%d]", fifo_idx);
		case G2M_CODE__PV_CALL_EXIT:
			die(_pv, "Solution[%d] called exit()", fifo_idx);
		default:
			die(_fail, "Unknown g2m_code %d from sol2mgr[%d]", g2m_code, fifo_idx);
	}
}


// problem logic

const int CALLS_CNT_LIMIT = 2750;

int calls_cnt;

int N, M;
std::vector<std::vector<int>> adj;

int count_components(const std::vector<int>& S) {
	int components_cnt = 0;
	std::vector<bool> vis(N, false);
	for (int i = 0; i < N; i++) {
		if (vis[i])
			continue;
		components_cnt++;

		std::queue<int> q;
		vis[i] = true;
		q.push(i);
		while (!q.empty()) {
			int cur = q.front();
			q.pop();
			for (int nxt : adj[cur])
				if (!vis[nxt] && S[nxt] == S[cur]) {
					vis[nxt] = true;
					q.push(nxt);
				}
		}
	}
	return components_cnt;
}

int main(int argc, char *argv[]) {
	registerManager("sphinx", 1, argc, argv);

	// read input
	N = inf.readInt();
	M = inf.readInt();
	std::vector<int> C(N);
	rep(i, N)
		C[i] = inf.readInt();
	std::vector<int> X(M), Y(M);
	rep(j, M)
		X[j] = inf.readInt(), Y[j] = inf.readInt();

	adj.assign(N, vector<int>());
	rep(j, M) {
		adj[X[j]].push_back(Y[j]);
		adj[Y[j]].push_back(X[j]);
	}

	write_secret();
	write_int(N);
	write_int(M);
	rep(j, M)
		write_int(X[j]), write_int(Y[j]);
	out_flush();

	calls_cnt = 0;
	while (true) {
		read_secret();
		if (ok_action_mode != ActionMode::ok_new_experiment) break;

		std::vector<int> E = read_int_vector(N);

		calls_cnt++;
		if (calls_cnt > CALLS_CNT_LIMIT)
			die_too_many_calls("#experiments reached " + to_string(calls_cnt));

		rep(i, N)
			if (!(-1 <= E[i] && E[i] <= N))
				die_invalid_argument("Invalid value of E[" + to_string(i) + "]: " + to_string(E[i]));

		vector<int> S(N);
		rep(i, N)
			S[i] = (E[i] == -1 ? C[i] : E[i]);
		int P = count_components(S);

		write_secret();
		write_int(P);
		out_flush();
	}
	ensuref(ok_action_mode == ActionMode::ok_end_of_experiments, "Expected action code ok_end_of_experiments");
	log_printf("End of experiments.\n");

	std::vector<int> G = read_int_vector(N);

	// Scoring
	if (C == G)
		quitf(_ok, "#experiments: %d", calls_cnt);

	rep(i, N)
		if (!(0 <= G[i] && G[i] < N))
			quitf(_wa, "Invalid value of G[%d]: %d", i, G[i]);

	rep(j, M) {
		int x = X[j], y = Y[j];
		if (C[x] == C[y] && !(G[x] == G[y]))
			quitf(_wa, "Vertices %d and %d do have the same color, but they do not in returned answer", x, y);
		if (C[x] != C[y] && !(G[x] != G[y]))
			quitf(_wa, "Vertices %d and %d do not have the same color, but they do in returned answer", x, y);
	}
	quitp(50 / 100.0);
}
