import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.StringTokenizer;

public class aliens_av_n2k_dp_wa {

    public long take_photos(int n, int m, int k, int[] x, int[] y) {
        Range[] ranges = new Range[n];
        for (int i = 0; i < n; i++) {
            ranges[i] = new Range(Math.min(x[i], y[i]), Math.max(x[i], y[i]));
        }
        long result = solve(n, m, k, ranges);
        return result;
    }

    long solve(int n, int m, int k, Range[] ranges) {
        Arrays.sort(ranges);
        ranges = removeIncluded(ranges);
        n = ranges.length;

        final long INF = Long.MAX_VALUE / 3;

        long[][] dp = new long[n + 1][k + 1];
        for (long[] i : dp) {
            Arrays.fill(i, INF);
        }
        dp[0][0] = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < k; j++) {
                for (int next = i; next < n; next++) {
                    long cost = sqr(ranges[next].end - ranges[i].start + 1);
                    if (i > 0 && ranges[i].start <= ranges[i - 1].end) {
                        cost -= sqr(ranges[i - 1].end - ranges[i].start + 1);
                    }
                    dp[next + 1][j + 1] = Math.min(dp[next + 1][j + 1], dp[i][j] + cost);
                }
            }
        }
        return dp[n][k];
    }

    class Range implements Comparable<Range> {
        int start, end;

        public Range(int start, int end) {
            this.start = start;
            this.end = end;
        }

        @Override
        public int compareTo(Range o) {
            int cmp = Integer.compare(start, o.start);
            if (cmp != 0) {
                return cmp;
            }
            return -Integer.compare(end, o.end);
        }
    }

    long sqr(long x) {
        return x * x;
    }

    Range[] removeIncluded(Range[] ranges) {
        Arrays.sort(ranges);
        int maxRight = -1;
        List<Range> result = new ArrayList<>();
        for (int i = 0; i < ranges.length; i++) {
            if (ranges[i].end > maxRight) {
                result.add(ranges[i]);
                maxRight = ranges[i].end;
            }
        }
        return result.toArray(new Range[result.size()]);
    }


    // BEGIN CUT

    public static void main(String[] args) throws IOException {
        new aliens_av_n2k_dp_wa().run();
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
        long ans = new aliens_av_n2k_dp_wa().take_photos(n, m, k, r, c);
        
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

    // END CUT
}
