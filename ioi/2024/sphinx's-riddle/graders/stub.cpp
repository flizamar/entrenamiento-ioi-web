#include "sphinx.h"
#include <cstdio>
#include <cstdlib>
#include <csignal>

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

const int secret_g2m = 0x34508C80;
const int secret_m2g = 0x75EC8020;
const int code_mask  = 0x0000000F;

const int M2G_CODE__OK = 0;

const int G2M_CODE__OK_NEW_EXPERIMENT = 0;
const int G2M_CODE__OK_END_OF_EXPERIMENTS = 1;
const int G2M_CODE__INVALID_E_LENGTH = 2;
const int G2M_CODE__INVALID_G_LENGTH = 3;
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
	if(1 > fprintf(fout, "%d\n", x)) {
		fprintf(stderr, "Could not write int to fout\n");
		authorized_exit(3);
	}
}

void write_int_vector(const vector<int>& v) {
	rep(i, sz(v))
		write_int(v[i]);
}

void write_secret(int g2m_code) {
	write_int(secret_g2m | g2m_code);
}

NORETURN void die(int g2m_code) {
	if(g2m_code == G2M_CODE__OK_NEW_EXPERIMENT || g2m_code == G2M_CODE__OK_END_OF_EXPERIMENTS) {
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
	if(1 != fscanf(fin, "%d", &x)) {
		fprintf(stderr, "Could not read int from fin\n");
		authorized_exit(3);
	}
	return x;
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


// problem logic

int N;

} // namespace


int perform_experiment(std::vector<int> E) {
	if (sz(E) != N) die(G2M_CODE__INVALID_E_LENGTH);
	write_secret(G2M_CODE__OK_NEW_EXPERIMENT);
	write_int_vector(E);
	out_flush();

	read_secret();
	int P = read_int();
	return P;
}

int main() {
	signal(SIGPIPE, SIG_IGN);
	atexit(check_exit_protocol);
	at_quick_exit(check_exit_protocol);

	read_secret();
	N = read_int();
	int M = read_int();
	std::vector<int> X(M), Y(M);
	rep(j, M)
		X[j] = read_int(), Y[j] = read_int();

	std::vector<int> G = find_colours(N, X, Y);

	if (sz(G) != N) die(G2M_CODE__INVALID_G_LENGTH);

	write_secret(G2M_CODE__OK_END_OF_EXPERIMENTS);
	write_int_vector(G);
	out_flush();

	exit_allowed = true;
	return 0;
}
