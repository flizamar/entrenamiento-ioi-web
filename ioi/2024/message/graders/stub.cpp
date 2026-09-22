#include "message.h"
#include <csignal>
#include <cstdlib>
#include <cstdio>
#include <string>

using namespace std;


namespace {

/******************************** Begin testlib-related material ********************************/
#ifdef _MSC_VER
#   define NORETURN __declspec(noreturn)
#elif defined __GNUC__
#   define NORETURN __attribute__ ((noreturn))
#else
#   define NORETURN
#endif
/********************************* End testlib-related material *********************************/


// utils

#define rep(i, n) for(int i = 0, i##__n = (int)(n); i < i##__n; ++i)

template<class C> int sz(const C& c) { return std::size(c); }

// grader/manager protocol

const int secret_g2m = 0x647FB390;
const int secret_m2g = 0xB107D5C0;
const int code_mask  = 0x0000000F;

const int M2G_CODE__OK = 0;

const int G2M_CODE__OK = 0;
const int G2M_CODE__OK_END_OF_PACKETS = 1;
const int G2M_CODE__PACKET_SIZE_INCORRECT = 2;
const int G2M_CODE__RETURN_MESSAGE_TOO_LARGE = 3;
const int G2M_CODE__PV_CALL_EXIT = 13;
const int G2M_CODE__PV_TAMPER_M2G = 14;
const int G2M_CODE__SILENT = 15;


bool exit_allowed = false;

NORETURN void authorized_exit(int exit_code) {
  exit_allowed = true;
  exit(exit_code);
}


FILE* fin = stdin;
FILE* fout = stdout;

void out_flush() {
	fflush(fout);
}

void write_int(int x) {
	if (1 != fwrite(&x, sizeof(x), 1, fout)) {
		fprintf(stderr, "Could not write int to fout\n");
		authorized_exit(3);
	}
}

void write_int_array(const int* arr, int len) {
	if (int ret = fwrite(arr, sizeof(int), len, fout); len != ret) {
		fprintf(stderr, "Could not write int array of size %d to fout, fwrite returned %d\n", len, ret);
		authorized_exit(3);
	}
}


void write_secret(int g2m_code = G2M_CODE__OK) {
	write_int(secret_g2m | g2m_code);
}

NORETURN void die(int g2m_code) {
	if(g2m_code == G2M_CODE__OK) {
		fprintf(stderr, "Shall not die with code OK\n");
		authorized_exit(5);
	}
	fprintf(stderr, "Dying with code %d\n", g2m_code);
	if(g2m_code != G2M_CODE__SILENT)
		write_secret(g2m_code);
	fclose(fin);
	fclose(fout);
	authorized_exit(0);
}


int read_int() {
	int x;
	if (1 != fread(&x, sizeof(x), 1, fin)) {
		fprintf(stderr, "Could not read int from fin\n");
		authorized_exit(3);
	}
	return x;
}

void read_int_array(int* arr, int len) {
	if (int ret = fread(arr, sizeof(int), len, fin); len != ret) {
		fprintf(stderr, "Could not read int array of size %d from fin, fread returned %d\n", len, ret);
		authorized_exit(3);
	}
}


void read_secret() {
	int secret = read_int();
	if((secret & ~code_mask) != secret_m2g)
		die(G2M_CODE__PV_TAMPER_M2G);
	int m2g_code = secret & code_mask;
	if(m2g_code != M2G_CODE__OK)
		die(G2M_CODE__SILENT);
}

void check_exit_protocol() {
  if (!exit_allowed)
    die(G2M_CODE__PV_CALL_EXIT);
}


// problem utils

int vb2int(const vector<bool> &v) {
	int r = 0;
	rep(i, sz(v)) r |= ((v[i] ? 1 : 0) << i);
	return r;
}

vector<bool> int2vb(int x, int len) {
	vector<bool> v(len);
	rep(i, sz(v)) v[i] = (x >> i & 1) ? true : false;
	return v;
}

void arr2vb(int* arr, int len, vector<bool> &r) {
	r.resize(len);
	rep(i, len) r[i] = (arr[i / 32] >> (i & 31) & 1) ? true : false;
}

int compressed_len(int len) { return (len+31) / 32; }

void vb2arr(vector<bool> v, int* arr, int len) {
	len = sz(v);
	rep(i, compressed_len(len)) arr[i] = 0;
	rep(i, len) arr[i / 32] |= (v[i] ? 1 : 0) << (i & 31);
}


// grader logic

const int PACKET_SIZE = 31;
const int MAX_MESSAGE_SIZE = 1024;
const int MESSAGE_SIZE_LIMIT = 2 * MAX_MESSAGE_SIZE;

NORETURN void run_encoder() {
	for(int S; read_secret(), (S = read_int()) != -1; ) {
		int M_f[(MAX_MESSAGE_SIZE + 32 - 1) / 32];
		read_int_array(M_f, compressed_len(S));
		int C_f = read_int();

		vector<bool> M;
		arr2vb(M_f, S, M);
		vector<bool> C = int2vb(C_f, PACKET_SIZE);

		send_message(M, C);

		write_secret(G2M_CODE__OK_END_OF_PACKETS);
		out_flush();
	}
	die(G2M_CODE__SILENT);
}

NORETURN void run_decoder() {
	for(int K; read_secret(), (K = read_int()) != -1; ) {
		vector<int> R_f(K, 0);
		read_int_array(R_f.data(), K);

		vector<vector<bool>> R(K);
		rep(i, K) R[i] = int2vb(R_f[i], PACKET_SIZE);
		vector<bool> D = receive_message(R);
		int L = sz(D);

		if(L < 0 || L > MESSAGE_SIZE_LIMIT) die(G2M_CODE__RETURN_MESSAGE_TOO_LARGE);

		int D_f[(MESSAGE_SIZE_LIMIT + 32 - 1) / 32];
		vb2arr(D, D_f, L);

		write_secret();
		write_int(L);
		write_int_array(D_f, compressed_len(L));
		out_flush();
	}
	die(G2M_CODE__SILENT);
}


} // namespace


vector<bool> send_packet(vector<bool> A) {
	if(sz(A) != PACKET_SIZE) die(G2M_CODE__PACKET_SIZE_INCORRECT);

	int A_f = vb2int(A);

	write_secret();
	write_int(A_f);
	out_flush();

	read_secret();
	int B_f = read_int();
	return int2vb(B_f, PACKET_SIZE);
}

int main(int argc, char **argv) {
	signal(SIGPIPE, SIG_IGN);
	atexit(check_exit_protocol);
	at_quick_exit(check_exit_protocol);

	if(argc < 2) {
		fprintf(stderr, "invalid args\n");
		authorized_exit(1);
	}

	string grader_id = argv[1];
	if (grader_id == "0")
		run_encoder();

	if (grader_id == "1")
		run_decoder();

	fprintf(stderr, "invalid grader id: '%s'\n", grader_id.c_str());
	authorized_exit(1);
}
