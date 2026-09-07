import java.io.*;
import java.util.*;

public class aliens_av_nk_convex_hull_trick {
    
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

        ConvexHullTrick cht = new ConvexHullTrick(n);
        final long INF = Long.MAX_VALUE / 2;
        long[] dp = new long[n + 1];
        long[] newDp = new long[n + 1];
        Arrays.fill(dp, INF);
        dp[0] = 0;

        for (int IT = 0; IT < k; IT++) {
            cht.clear();
            cht.addLine(-2 * (ranges[0].start - 1), sqr(ranges[0].start - 1));
            for (int i = 1; i <= n; i++) {
                newDp[i] = cht.getMax(ranges[i - 1].end) + sqr(ranges[i - 1].end);
                if (i < n) {
                    cht.addLine(-2 * (ranges[i].start - 1), dp[i] + sqr(ranges[i].start - 1) - sqr(Math.max(0, ranges[i - 1].end - ranges[i].start + 1)));
                }
            }
            System.arraycopy(newDp, 0, dp, 0, dp.length);
        }

        return dp[n];
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


    class ConvexHullTrick {
        long[] as, bs;
        int size, opt;

        public ConvexHullTrick(int maxSize) {
            as = new long[maxSize];
            bs = new long[maxSize];
            size = 0;
            opt = 0;
        }

        void addLine(long a, long b) {
            while (size > 1) {
                double x1 = intersection(as[size - 2], bs[size - 2], as[size - 1], bs[size - 1]);
                double x2 = intersection(as[size - 1], bs[size - 1], a, b);
                if (x1 < x2) {
                    break;
                } else {
                    size--;
                }
            }
            as[size] = a;
            bs[size] = b;
            size++;
        }

        double intersection(long a1, long b1, long a2, long b2) {
            return 1.0 * (b2 - b1) / (a1 - a2);
        }

        long getMax(long v) {
            opt = Math.min(opt, size - 1);
            while (opt + 1 < size) {
                double x = intersection(as[opt], bs[opt], as[opt + 1], bs[opt + 1]);
                if (v > x) {
                    opt++;
                } else {
                    break;
                }
            }
            return as[opt] * v + bs[opt];
        }

        void clear() {
            size = 0;
            opt = 0;
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
        new aliens_av_nk_convex_hull_trick().run();
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
        long ans = new aliens_av_nk_convex_hull_trick().take_photos(n, m, k, r, c);
        
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
