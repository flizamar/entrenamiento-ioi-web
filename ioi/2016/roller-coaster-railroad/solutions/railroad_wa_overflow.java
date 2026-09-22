import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.StringTokenizer;

public class railroad_wa_overflow {

    private static final int INF = (int) 1e9;

    private int dsu_get(int[] p, int u) {
        return (u == p[u]) ? u : (p[u] = dsu_get(p, p[u]));
    }

    private boolean dsu_union(int[] p, int u, int v) {
        u = dsu_get(p, u);
        v = dsu_get(p, v);
        p[u] = v;
        return (u != v);
    }

    public long plan_roller_coaster(int[] s, int[] t) {
        int n = s.length;
        ArrayList<Event> e = new ArrayList<>();
        for (int i = 0; i < n; ++i) {
            e.add(new Event(s[i], 1, i));
            e.add(new Event(t[i], -1, i));
        }
        e.add(new Event(INF, 1, n));
        e.add(new Event(1, -1, n));
        n++;
        int[] p = new int[n];
        for (int i = 0; i < n; ++i) {
            p[i] = i;
        }
        Collections.sort(e);
        long res = 0;
        ArrayList<Edge> edges = new ArrayList<>();
        for (int i = 0, delta = 0; i + 1 < e.size(); ++i) {
            delta += e.get(i).type;
            res += Math.max(0, delta) * (e.get(i + 1).c - e.get(i).c);
            edges.add(new Edge(e.get(i + 1).c - e.get(i).c, e.get(i).num, e.get(i + 1).num));
            if ((e.get(i + 1).c == e.get(i).c) || (delta != 0))
                dsu_union(p, e.get(i).num, e.get(i + 1).num);
        }
        Collections.sort(edges);
        for (Edge edge : edges)
            if (dsu_union(p, edge.l, edge.r))
                res += edge.cost;
        return res;
    }

    private class Event implements Comparable<Event> {
        final int c, type, num;

        private Event(int c, int type, int num) {
            this.c = c;
            this.type = type;
            this.num = num;
        }

        @Override
        public int compareTo(Event o) {
            int cmp = Integer.compare(this.c, o.c);
            if (cmp == 0) {
                return Integer.compare(o.type, this.type);
            }
            return cmp;
        }
    }

    private class Edge implements Comparable<Edge> {
        final int cost, l, r;

        Edge(int cost, int l, int r) {
            this.cost = cost;
            this.l = l;
            this.r = r;
        }

        @Override
        public int compareTo(Edge o) {
            return Integer.compare(this.cost, o.cost);
        }
    }

    //BEGIN CUT
    public static void main(String[] args) {
        new railroad_wa_overflow().run();
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
        long ans = new railroad_wa_overflow().plan_roller_coaster(s, t);
        
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
