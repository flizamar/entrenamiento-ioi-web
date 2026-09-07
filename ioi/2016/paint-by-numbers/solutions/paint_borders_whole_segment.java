import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.Arrays;
import java.util.StringTokenizer;

/**
 * @author: pashka
 */
public class paint_borders_whole_segment {

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

        reverse(C);
        reverse(canB);
        reverse(canW);

        boolean[][] right = calcLeft();

        reverse(C);
        reverse(canB);
        reverse(canW);

        boolean[][] left = calcLeft();

        int[] leftmost = new int[C.length];
        int[] rightmost = new int[C.length];

        int last = 0;
        for (int i = 0; i < C.length; i++) {
            for (int j = last; j < n; j++) {
                int rr = j + C[i] == n ? 0 : n - (j + C[i]) - 1;
                if (canPut(j, j + C[i]) && right[C.length - i - 1][rr]) {
                    leftmost[i] = j;
                    last = j + C[i] + 1;
                    break;
                }
            }
        }
        last = n;
        for (int i = C.length - 1; i >= 0; i--) {
            for (int j = last; j >= 0; j--) {
                int ll = j - C[i] == 0 ? 0 : j - C[i] - 1;
                if (canPut(j - C[i], j) && left[i][ll]) {
                    rightmost[i] = j - C[i];
                    last = j - C[i] - 1;
                    break;
                }
            }
        }

        char[] ans = new char[n];
        Arrays.fill(ans, '_');
        for (int i = 0; i < C.length; i++) {
            for (int j = leftmost[i]; j <= rightmost[i]; j++) {
                if (canPut(j, j + C[i])) {
                    for (int k = 0; k < C[i]; k++) {
                        ans[j + k] = '?';
                    }
                }
            }
        }
        for (int i = 0; i < C.length; i++) {
            for (int j = rightmost[i]; j < leftmost[i] + C[i]; j++) {
                ans[j] = 'X';
            }
        }

        for(int i = 0; i < C.length; i++) {
            if (leftmost[i] == rightmost[i]) {
                if (leftmost[i] > 0) {
                    ans[leftmost[i] - 1] = '_';
                }
                if (leftmost[i] + C[i] < n) {
                    ans[leftmost[i] + C[i]] = '_';
                }
            }
        }

        for (int i = 0; i < n; i++) {
            if (!canB[i]) ans[i] = '_';
            if (!canW[i]) ans[i] = 'X';
        }

        return new String(ans);
    }

    private void reverse(int[] a) {
        int l = 0, r = a.length - 1;
        while (l < r) {
            int t = a[l];
            a[l] = a[r];
            a[r] = t;
            l++;
            r--;
        }
    }

    private void reverse(boolean[] a) {
        int l = 0, r = a.length - 1;
        while (l < r) {
            boolean t = a[l];
            a[l] = a[r];
            a[r] = t;
            l++;
            r--;
        }
    }

    private boolean canPut(int l, int r) {
        if (l < 0 || r > canW.length) return false;
        if (l > 0 && !canW[l - 1]) return false;
        if (r < canW.length && !canW[r]) return false;
        return sumB[r] - sumB[l] == (r - l);
    }

    private boolean[][] calcLeft() {
        for (int i = 0; i < n; i++) {
            sumB[i + 1] = sumB[i];
            if (canB[i]) sumB[i + 1]++;
        }
        boolean[][] res = new boolean[C.length + 1][n + 1];
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
        return res;
    }

    // BEGIN CUT


    public static void main(String[] args) throws IOException {
        new paint_borders_whole_segment().run();
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
        String ans = new paint_borders_whole_segment().solve_puzzle(S, C);
        
        // BEGIN SECRET
        out.println("098d134608c94f7413faac591054ee35");
        // END SECRET
        
        out.println(ans);
        out.close();
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
