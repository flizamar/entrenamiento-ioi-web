import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.StringTokenizer;

class aliens {
    class Pair implements Comparable<Pair> {
        int first, second;
        Pair() {}
        Pair(int first, int second) {
            this.first = first;
            this.second = second;
        }

        @Override
        public int compareTo(Pair pair) {
            if (first != pair.first) {
                return Integer.compare(first, pair.first);
            }
            return -Integer.compare(second, pair.second);
        }
    }

    final long INF = 2_000_000_000_000L;

    long sqr(long x) {
        return x * x;
    }

    public long take_photos(int n, int m, int k, int[] r, int[] cc) {
        ArrayList<Pair> a = new ArrayList<>();
        for (int i = 0; i < n; i++) {
            a.add(new Pair(Math.min(r[i], cc[i]), Math.max(r[i], cc[i])));
        }
        Collections.sort(a);
        ArrayList<Pair> t = new ArrayList<>();
        for (int i = 0; i < n; i++) {
            if (!t.isEmpty() && t.get(t.size() - 1).first <= a.get(i).first && t.get(t.size() - 1).second >= a.get(i).second) continue;
            t.add(a.get(i));
        }
        a = t;
        n = a.size();

        long ans = INF;
        long dp1[] = new long[n + 1];
        Arrays.fill(dp1, INF);
        dp1[0] = 0;
        for (int j = 0; j < k; j++) {
            long dp2[] = new long[n + 1];
            Arrays.fill(dp2, INF);

            long c[] = new long[n + 1];
            long b[] = new long[n + 1];
            c[0] = 0; b[0] = INF;
            int sz = 1;
            int cur = 0;
            for (int i = 0; i <= n; i++) {
                if (i > 0) {
                    cur = Math.min(cur, sz - 1);
                    while (cur < sz - 1) {
                        long y1 = c[cur] * a.get(i - 1).second + b[cur];
                        long y2 = c[cur + 1] * a.get(i - 1).second + b[cur + 1];
                        if (y1 >= y2) {
                            cur++;
                        } else {
                            break;
                        }
                    }
                    dp2[i] = sqr(a.get(i - 1).second) + c[cur] * a.get(i - 1).second + b[cur];
                }
                if (i < n && dp1[i] < INF) {
                    long b3 = - sqr(Math.max(0L, (i > 0 ? a.get(i - 1).second : -INF) - a.get(i).first + 1)) + sqr(a.get(i).first - 1) + dp1[i];
                    long c3 = 2 * (-a.get(i).first + 1);
                    while (sz > 1) {
                        long c1 = c[sz - 2], b1 = b[sz - 2];
                        long c2 = c[sz - 1], b2 = b[sz - 1];
                        if (c3 * (b2 - b1) + b3 * (c1 - c2) <= c2 * (b2 - b1) + b2 * (c1 - c2)) {
                            sz--;
                        } else {
                            break;
                        }
                    }
                    b[sz] = b3;
                    c[sz] = c3;
                    sz++;
                }
            }
            ans = Math.min(ans, dp2[n]);
            dp1 = dp2;
        }
        return ans;
    }
}

public class aliens_iz_nk {

    public static void main(String[] args) {
        new aliens_iz_nk().run();
    }

    void run() {
        br = new BufferedReader(new InputStreamReader(System.in));
        out = new PrintWriter(System.out);
        int n = nextInt();
        int m = nextInt();
        int k = nextInt();

        int[] r = new int[n];
        int[] c = new int[n];
        for (int i = 0; i < n; i++) {
            r[i] = nextInt();
            c[i] = nextInt();
        }
        long ans = new aliens().take_photos(n, m, k, r, c);

        // BEGIN SECRET
        out.println("098d134608c94f7413faac591054ee35");
        // END SECRET

        out.println(ans);
        out.close();
    }

    BufferedReader br;
    StringTokenizer st;
    PrintWriter out;

    String next() {
        try {
            while (st == null || !st.hasMoreTokens()) {
                st = new StringTokenizer(br.readLine());
            }
            return st.nextToken();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    int nextInt() {
        return Integer.parseInt(next());
    }
}
