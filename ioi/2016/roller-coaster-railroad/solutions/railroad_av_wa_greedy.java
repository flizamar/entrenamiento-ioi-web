import java.io.*;
import java.util.*;

public class railroad_av_wa_greedy {

    class Node {
        List<Integer> ids = new ArrayList<>();
        int s, t;

        public Node(int id, int s, int t) {
            ids.add(id);
            this.s = s;
            this.t = t;
        }
    }

    public long plan_roller_coaster(int[] s, int[] t) {
        int n = s.length;
        Node[] nodes = new Node[n];
        boolean[] used = new boolean[n];
        for (int i = 0; i < n; i++) {
            nodes[i] = new Node(i, s[i], t[i]);
        }

        long result = 0;
        for (int IT = 0; IT < n - 1; IT++) {
            int best = Integer.MAX_VALUE;
            int bestI = -1, bestJ = -1;

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (i == j || used[i] || used[j]) {
                        continue;
                    }
                    int cost = Math.max(0, nodes[i].t - nodes[j].s);
                    if (cost < best) {
                        best = cost;
                        bestI = i;
                        bestJ = j;
                    }
                }
            }
            result += best;
            nodes[bestI] = mergeNodes(nodes[bestI], nodes[bestJ]);
            used[bestJ] = true;
        }

        return result;
    }

    private Node mergeNodes(Node first, Node second) {
        first.ids.addAll(second.ids);
        first.t = second.t;
        return first;
    }

    //BEGIN CUT
    public static void main(String[] args) {
        new railroad_av_wa_greedy().run();
    }

    private void run() {
        br = new BufferedReader(new InputStreamReader(System.in));
        out = new PrintWriter(System.out);
        int n = nextInt();
        int[] s = new int[n], t = new int[n];
        int needAnswer = nextInt();
        for (int i = 0; i < n; ++i) {
            s[i] = nextInt();
            t[i] = nextInt();
        }
        long ans = new railroad_av_wa_greedy().plan_roller_coaster(s, t);

        // BEGIN SECRET
        out.println("14e047d7a2907b9034950b074822b302");
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
    //END CUT
}
