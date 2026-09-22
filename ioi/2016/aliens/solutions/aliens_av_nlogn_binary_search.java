import java.io.*;
import java.util.*;

public class aliens_av_nlogn_binary_search {

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
        k = Math.min(k, n);

        ConvexHullTrick cht = new ConvexHullTrick(n);
        long left = 0, right = 1L * m * m + 1;
        if (right % 2 != 0) {
            right++;
        }
        while (left < right - 1) {
            long mid = (left + right) >>> 1;
//            if (mid % 2 != 0) {
//                mid--;
//            }
            long C = mid + 1;
            long[] result = getValue(n, ranges, cht, C);
            if (result[1] < k) {
                right = mid;
            } else {
                left = mid;
            }
        }


        long[] result1 = getValue(n, ranges, cht, left + 1);
        long[] result2 = getValue(n, ranges, cht, right + 1);
        long answer1 = result1[0] - result1[1] * (left + 1);
        long answer2 = result2[0] - result2[1] * (right + 1);

        if (result1[1] == k) {
            return answer1;
        } else if (result2[1] == k) {
            return answer2;
        } else {
            // result1[1] > k > result2[1]
            long diff = answer1 - answer2;
            if (diff % (result1[1] - result2[1]) != 0) {
                throw new AssertionError();
            }

            return answer1 + diff / (result1[1] - result2[1]) * (k - result1[1]); // k - result1[1] < 0
        }
    }

    long[] getValue(int n, Range[] ranges, ConvexHullTrick cht, long C) {
        cht.clear();
        cht.addLine(-2 * (ranges[0].start - 1), sqr(ranges[0].start - 1), 0);
        for (int i = 1; i <= n; i++) {
            int line = cht.getMax(ranges[i - 1].end);
            long value = cht.as[line] * ranges[i - 1].end + cht.bs[line] + sqr(ranges[i - 1].end) + C;
            int bombs = cht.bombs[line] + 1;
            if (i < n) {
                cht.addLine(-2 * (ranges[i].start - 1), value + sqr(ranges[i].start - 1) - sqr(Math.max(0, ranges[i - 1].end - ranges[i].start + 1)), bombs);
            } else {
                return new long[] {value, bombs};
            }
        }
        throw new AssertionError();
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
        int[] bombs;
        int size, opt;

        public ConvexHullTrick(int maxSize) {
            as = new long[maxSize];
            bs = new long[maxSize];
            bombs = new int[maxSize];
            size = 0;
            opt = 0;
        }

        void addLine(long a, long b, int bomb) {
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
            bombs[size] = bomb;
            size++;
        }

        double intersection(long a1, long b1, long a2, long b2) {
            return 1.0 * (b2 - b1) / (a1 - a2);
        }

        int getMax(long v) {
            opt = Math.min(opt, size - 1);
            while (opt + 1 < size) {
                double x = intersection(as[opt], bs[opt], as[opt + 1], bs[opt + 1]);
                if (v > x) {
                    opt++;
                } else {
                    break;
                }
            }
            return opt;
        }

        void clear() {
            size = 0;
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
        new aliens_av_nlogn_binary_search().run();
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
        long ans = new aliens_av_nlogn_binary_search().take_photos(n, m, k, r, c);
        
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
