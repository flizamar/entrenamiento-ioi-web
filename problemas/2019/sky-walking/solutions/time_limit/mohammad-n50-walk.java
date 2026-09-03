import java.util.*;

class walk {

	class triple implements Comparable<triple>{
		public int a, b, c;
		public triple(int a, int b, int c){
			this.a = a;
			this.b = b;
			this.c = c;
		}

		@Override
			public int compareTo(triple cur){
				if(this.a < cur.a)return -1;
				if(this.a > cur.a)return 1;
				if(this.b < cur.b)return -1;
				if(this.b > cur.b)return 1;
				if(this.c < cur.c)return -1;
				if(this.c > cur.c)return 1;
				return 0;
			}

		@Override
			public boolean equals(Object o){
				if (o == this) { 
					return true; 
				} 
				if (!(o instanceof triple)) { 
					return false; 
				} 
				triple cur = (triple) o; 
				return (this.a == cur.a && this.b == cur.b && this.c == cur.c);
			}
		@Override
			public int hashCode() {
				int p = 1000000007;
				return p * p * this.a + p * this.b + this.c;
			}
	}

	class pair implements Comparable<pair>{
		public long a, b;
		public pair(long a, long b){
			this.a = a;
			this.b = b;
		}

		@Override
			public int compareTo(pair cur){
				if(this.a < cur.a)return -1;
				if(this.a > cur.a)return 1;
				if(this.b < cur.b)return -1;
				if(this.b > cur.b)return 1;
				return 0;
			}
		@Override
			public boolean equals(Object o){
				if (o == this) { 
					return true; 
				} 
				if (!(o instanceof pair)) { 
					return false; 
				} 
				pair cur = (pair) o; 
				return (this.a == cur.a && this.b == cur.b);
			}
		@Override
			public int hashCode() {
				int p = 1000000007;
				return p * (int)this.a + (int)this.b;
			}
	}


	public static final int MAXN = 1000000 + 10;
	public static final long INF = 1000000000000000000L;
	public int n, m, node;
	public int[] X, Y, H, L, R = new int[MAXN];
	public ArrayList<pair>[] adj = new ArrayList[MAXN];
	public long[] dis = new long[MAXN];
	public HashMap<pair, Integer> nodes = new HashMap<pair, Integer>();
	public HashMap<pair, Boolean> mrk = new HashMap<pair, Boolean>();

	public long dijkstra(int source, int sink){
		TreeSet<pair> S = new TreeSet<pair>();
		for(int i = 0; i <= node; i++)
			dis[i] = INF;
		dis[source] = 0;
		S.add(new pair(0, source));
		while(S.size() > 0){
			pair cur = S.first();
			long dist = cur.a;
			int v = (int)cur.b;
			S.remove(cur);
			for(int i = 0; i < adj[v].size(); i++){
				int u = (int)adj[v].get(i).a;
				long w = adj[v].get(i).b;
				if(dist + w < dis[u]){
					S.remove(new pair(dis[u], u));
					dis[u] = dist + w;
					S.add(new pair(dis[u], u));
				}
			}
		}
		if(dis[sink] == INF)
			return -1;
		return dis[sink];
	}

	public int add_map(int x, int y){
		if(nodes.get(new pair(x, y)) == null)
			nodes.put(new pair(x, y), ++node);
		return nodes.get(new pair(x, y));
	}

	public void add_edge(int x1, int y1, int x2, int y2){
		int u = add_map(x1, y1);
		int v = add_map(x2, y2);
		if(x1 == x2){
			adj[u].add(new pair(v, Math.abs(y2 - y1)));	
			adj[v].add(new pair(u, Math.abs(y2 - y1)));	
		}
		else{
			adj[u].add(new pair(v, Math.abs(x2 - x1)));	
			adj[v].add(new pair(u, Math.abs(x2 - x1)));	
		}
	}

	public void build_graph(){
		ArrayList<pair> pnt1 = new ArrayList<pair>();
		ArrayList<pair> pnt2 = new ArrayList<pair>();
		for(int i = 0; i < n; i++){
			pnt1.add(new pair(X[i], 0));
			pnt2.add(new pair(0, X[i]));
			for(int j = 0; j < m; j++)
				if(i >= L[j] && i <= R[j] && H[i] >= Y[j]){
					pnt1.add(new pair(X[i], Y[j]));
					pnt2.add(new pair(Y[j], X[i]));
					if(i == R[j]){
						mrk.put(new pair(X[i], Y[j]), true);
					}
				}
		}
		Collections.sort(pnt1);
		Collections.sort(pnt2);
		for(int i = 1; i < pnt1.size(); i++)
			if(pnt1.get(i).a == pnt1.get(i - 1).a)
				add_edge((int)pnt1.get(i - 1).a, (int)pnt1.get(i - 1).b, (int)pnt1.get(i).a, (int)pnt1.get(i).b);
		for(int i = 1; i < pnt2.size(); i++)
			if(pnt2.get(i).a == pnt2.get(i - 1).a && mrk.get(new pair(pnt2.get(i - 1).b, pnt2.get(i - 1).a)) == null && pnt2.get(i - 1).a != 0)
				add_edge((int)pnt2.get(i - 1).b, (int)pnt2.get(i - 1).a, (int)pnt2.get(i).b, (int)pnt2.get(i).a);
	}

	public void init(){
		ArrayList<triple> tmp = new ArrayList<triple>();
		ArrayList<triple> nw = new ArrayList<triple>();
		for(int i = 0; i < MAXN; i++) adj[i] = new ArrayList<pair>();
		for(int i = 0; i < m; i++) tmp.add(new triple(Y[i], L[i], R[i]));
		Collections.sort(tmp);
		for(int i = 0; i < m; i++){
			if(tmp.get(i).b == tmp.get(i).c)continue;
			int j = i;
			int hei = tmp.get(i).a;
			int lo = tmp.get(i).b;
			int hi = tmp.get(i).c;
			while(j < m && tmp.get(j).a == hei && tmp.get(j).b <= hi && tmp.get(j).b >= lo){
				hi = Math.max(hi, tmp.get(j).c);
				j++;
			}
			nw.add(new triple(hei, lo, hi));
			i = j - 1;
		}
		m = nw.size();
		for(int i = 0; i < m; i++){
			L[i] = nw.get(i).b;
			R[i] = nw.get(i).c;
			Y[i] = nw.get(i).a;
		}
	}

	public long min_distance(int[] XX, int[] HH, int[] LL, int[] RR, int[] YY, int S, int G) {
		X = XX; H = HH; L = LL; R = RR; Y = YY;
		n = X.length;
		m = L.length;
		init();
		build_graph();
		return dijkstra(add_map(X[S], 0), add_map(X[G], 0));
	}
}
