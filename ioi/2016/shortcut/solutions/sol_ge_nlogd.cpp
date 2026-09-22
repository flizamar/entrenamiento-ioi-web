#include <cstdio>
#include <cstdlib>
#include <vector>
#include <deque>
#include <iostream>
#include <algorithm>
#include <ctime>

using namespace std;

#define pb push_back
#define mp make_pair
#define fs first
#define sc second

const int maxN = 1000 * 1000;
const long long inf = (long long) 1e18;

long long x[maxN + 1];
long long d[maxN + 1];
bool important[maxN + 1];
long long maxsump[maxN + 1], mindifp[maxN + 1];

long long difl, difr, suml, sumr;
double checkempty = 0.0;

bool isNonempty(int n) {    
    if (suml > sumr || difl > difr)	
    	return false;
	
    int p = 0;
	for (int i = 1; i < n; i++) {
		while (p < i && x[p] + x[i] <= sumr && x[p] - x[i] <= difr)
			p++;
		while (p > 0 && (x[p] + x[i] > sumr || x[p] - x[i] > difr))
			p--;
    
		if (x[i] + x[p] >= suml && x[i] + x[p] <= sumr && x[p] - x[i] >= difl && x[p] - x[i] <= difr) {
			return true;
		}
	}

	return false;
}

bool check(int n, int c, long long k) {
    suml = difl = -inf;
    sumr = difr = inf;

    int p = -1;
    deque <int> q;

    for (int i = 0; i < n; i++) {
        if (important[i]) {
            if (q.front() == p)
                q.pop_front();
                
            while (p + 1 < i && d[q.front()] - x[q.front()] > k - x[i] - d[i]) {
                p++;
                if (q.front() == p)
                    q.pop_front();            
            }

//            cerr << i << ' ' << p << ' ' << x[i] << ' ' << d[i] << ' ' << k << ' ' << c << ' ' << maxsump[p] << endl;

            if (p != -1) {
                suml = max(suml, x[i] + d[i] - k + c + maxsump[p]);
                sumr = min(sumr, x[i] - d[i] + k - c + mindifp[p]);
                difl = max(difl, -x[i] + d[i] - k + c + maxsump[p]);         
                difr = min(difr, -x[i] - d[i] + k - c  + mindifp[p]);    
            }
        }     
        while (!q.empty() && d[q.back()] - x[q.back()] <= d[i] - x[i])
            q.pop_back();
        q.push_back(i);
    }

//    cerr << k << ' ' << suml << ' ' << sumr << ' ' << difl << ' ' << difr << endl;

    return isNonempty(n);
}

long long find_shortcut(int n, vector <int> len, vector <int> dep, int c) {
    x[0] = 0ll;

    for (int i = 0; i < n; i++) {
        d[i] = dep[i];
        if (i + 1 < n) {
            x[i + 1] = x[i] + len[i];
        }    
    }

    maxsump[0] = d[0];
    mindifp[0] = -d[0];
    
    for (int i = 1; i < n; i++) {
        maxsump[i] = max(maxsump[i - 1], x[i] + d[i]);
        mindifp[i] = min(mindifp[i - 1], x[i] - d[i]);
    }

    important[n - 1] = true;
    int curb = n - 1;
    for (int i = n - 2; i >= 0; i--) {
        if (x[i] - d[i] < x[curb] - d[curb]) {
            important[i] = true;
            curb = i;    
        }
    }

    long long lb = 0ll;
    long long rb = x[n - 1] + 2e9;
    while (lb < rb) {
    	long long mid = (lb + rb) / 2;
    	if (check(n, c, mid))
    		rb = mid;
    	else
    		lb = mid + 1;
    }

    return lb;
}

// BEGIN CUT

int main()
{
	int n, c;
	scanf("%d%d", &n, &c);
	
	vector<int> l(n - 1);
	vector<int> d(n);
	for (int i = 0; i < n - 1; i++)
	    scanf("%d", &l[i]);
	for (int i = 0; i < n; i++)
	    scanf("%d", &d[i]);
	    
	long long t = find_shortcut(n, l, d, c);
	
	// BEGIN SECRET
    puts("14e047d7a2907b9034950b074822b302");
    // END SECRET
    
	printf("%lld\n", t);
	
	return 0;
}

// END CUT
