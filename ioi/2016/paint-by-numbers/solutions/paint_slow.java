import java.io.*;
import java.util.Arrays;
import java.util.StringTokenizer;

/**
 * @author: pashka
 */
public class paint_slow {

    int n;
    int[] C;
    boolean[] canB;
    boolean[] canW;
    int[] sumB;

    public String solve_puzzle(String S, int[] C) {

        this.C = C;
        n = S.length();
        canB = new boolean[n];
        Arrays.fill(canB, true);
        canW = new boolean[n];
        Arrays.fill(canW, true);
        sumB = new int[n + 1];

        for (int i = 0; i < n; i++) {
            canW[i] = S.charAt(i) != 'X';
            canB[i] = S.charAt(i) != '_';
        }

        res = new boolean[C.length + 1][this.n + 1];
        char[] ans = new char[n];

        if (!solvable()) throw new RuntimeException();

        for (int i = 0; i < n; i++) {
            if (canB[i]) {
                if (canW[i]) {
                    int q = 0;
                    canW[i] = false;
                    if (solvable()) q |= 1;
                    canW[i] = true;
                    canB[i] = false;
                    if (solvable()) q |= 2;
                    canB[i] = true;
                    ans[i] = "!X_?".charAt(q);
                } else {
                    ans[i] = 'X';
                }
            } else {
                ans[i] = '_';
            }
        }

        return new String(ans);
    }

    private boolean canPut(int l, int r) {
        if (l < 0 || r > canW.length) return false;
        if (l > 0 && !canW[l - 1]) return false;
        if (r < canW.length && !canW[r]) return false;
        return sumB[r] - sumB[l] == (r - l);
    }

    boolean[][] res;
    private boolean solvable() {
        for (int i = 0; i < n; i++) {
            sumB[i + 1] = sumB[i];
            if (canB[i]) sumB[i + 1]++;
        }
        for (boolean[] re : res) {
            Arrays.fill(re, false);
        }
        res[0][0] = true;
        for (int i = 0; i <= C.length; i++) {
            for (int j = 0; j < n; j++)
                if (canW[j]) {
                    res[i][j + 1] |= res[i][j];
                }
            if (i < C.length)
                for (int j = 0; j + C[i] <= n; j++) {
                    int jj = j == 0 ? 0 : j - 1;
                    if (res[i][jj] && canPut(j, j + C[i])) {
                        res[i + 1][j + C[i]] = true;
                    }
                }
        }
        return res[C.length][n];
    }

    // BEGIN CUT

    public static void main(String[] args) throws IOException {
        new paint_slow().run();
    }

    private void run() throws IOException {
        br = new BufferedReader(new InputStreamReader(System.in));
        out = new PrintWriter(System.out);
        String S = next();
        int Cn = nextInt();
        int[] C = new int[Cn];
        for (int i = 0; i < Cn; i++) {
            C[i] = nextInt();
        }
        String ans = new paint_slow().solve_puzzle(S, C);
        
        // BEGIN SECRET
        out.println("098d134608c94f7413faac591054ee35");
        // END SECRET
        
        out.println(ans);
        out.close();;
    }

    BufferedReader br;
    StringTokenizer st;
    PrintWriter out;

    String next() throws IOException {
        while (st == null || !st.hasMoreTokens()) {
            st = new StringTokenizer(br.readLine());
        }
        return st.nextToken();
    }

    int nextInt() throws IOException {
        return Integer.parseInt(next());
    }

    // END CUT
}
