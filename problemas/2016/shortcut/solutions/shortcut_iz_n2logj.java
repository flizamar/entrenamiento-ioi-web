import java.io.*;
import java.util.StringTokenizer;

class shortcut {
    private final static long INF = 1_000_000_000_000_000_000L;

    public long find_shortcut(int n, int[] l, int[] d, int c) {
        long x[] = new long[n];
        for (int i = 1; i < n; i++) {
            x[i] = x[i - 1] + l[i - 1];
        }

        long L = 0, R = INF;
        while (R - L > 1) {
            long M = (L + R) / 2;
            long minSum = -INF, maxSum = INF;
            long minDif = -INF, maxDif = INF;
            for (int i = 0; i < n; i++) {
                for (int j = i + 1; j < n; j++) {
                    if (x[j] - x[i] + d[i] + d[j] > M) {
                        minSum = Math.max(minSum, -M + c + d[i] + d[j] + x[i] + x[j]);
                        maxSum = Math.min(maxSum, M - c - d[i] - d[j] + x[i] + x[j]);
                        minDif = Math.max(minDif, -M + c + d[i] + d[j] + x[i] - x[j]);
                        maxDif = Math.min(maxDif, M - c - d[i] - d[j] + x[i] - x[j]);
                    }
                }
            }
            boolean found = false;
            for (int i = 0; i < n; i++) {
                for (int j = i + 1; j < n; j++) {
                    long sum = x[i] + x[j];
                    long dif = x[i] - x[j];
                    if (minSum <= sum && sum <= maxSum && minDif <= dif && dif <= maxDif) {
                        found = true;
                    }
                }
            }
            if (found) R = M;
            else L = M;
        }

        return R;
    }
}


public class shortcut_iz_n2logj {
    public static void main(String[] args) {
        new shortcut_iz_n2logj().run();
    }

    private void run() {
        br = new BufferedReader(new InputStreamReader(System.in));
        out = new PrintWriter(System.out);
        int n = nextInt();
        int c = nextInt();
        int[] l = new int[n - 1];
        int[] d = new int[n];
        for (int i = 0; i < n - 1; i++) {
            l[i] = nextInt();
        }
        for (int i = 0; i < n; i++) {
            d[i] = nextInt();
        }
        long res = new shortcut().find_shortcut(n, l, d, c);

        // BEGIN SECRET
        out.println("14e047d7a2907b9034950b074822b302");
        // END SECRET

        out.println(res);
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

