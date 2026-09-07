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


	public static final int MAXN = 600000 + 10;
	public static final int MAXS = 400000 + 10;
	public static final long INF = 1000000000000000000L;
	public int n, m, node;
	public int[] y_map = new int[MAXN];
	public int[] X, Y, H, L, R;
	public ArrayList<pair>[] adj = new ArrayList[MAXN];
	public ArrayList<Integer>[] neiL = new ArrayList[MAXN];
	public ArrayList<Integer>[] neiR = new ArrayList[MAXN];
	public long[] dis = new long[MAXN];
	public HashMap<pair, Integer> nodes = new HashMap<pair, Integer>();
	public HashMap<pair, Boolean> mrk = new HashMap<pair, Boolean>();
	public ArrayList<Integer> all_y = new ArrayList<Integer>();
	public ArrayList<pair>[] seg = new ArrayList[MAXS];


	public void build(int s, int e, int tag){
		if(s + 1 == e){
			seg[tag] = new ArrayList<pair>();
			seg[tag].add(new pair(H[s], s));
			return;
		}
		int mid = (s + e) / 2;
		build(s, mid, tag * 2);
		build(mid, e, tag * 2 + 1);
		seg[tag] = new ArrayList<pair>();
		int l = 0;
		int r = 0;
		while(l < seg[tag * 2].size() || r < seg[tag * 2 + 1].size()){
			if(l == seg[tag * 2].size()) seg[tag].add(seg[tag * 2 + 1].get(r++));
			else if(r == seg[tag * 2 + 1].size()) seg[tag].add(seg[tag * 2].get(l++));
			else if(seg[tag * 2].get(l).compareTo(seg[tag * 2 + 1].get(r)) == -1) seg[tag].add(seg[tag * 2].get(l++));
			else seg[tag].add(seg[tag * 2 + 1].get(r++));
		}
	}

	public ArrayList<Integer> get(int s, int e, int tag, int l, int r, int val){
		ArrayList<Integer> ret = new ArrayList<Integer>();
		if(s >= r || e <= l) return ret;
		if(s >= l && e <= r){
			int ind = -1 * Collections.binarySearch(seg[tag], new pair(val, -1)) - 1;
			for(int i = ind; i < seg[tag].size(); i++) ret.add((int)seg[tag].get(i).b);
			return ret;
		}
		int mid = (s + e) / 2;
		ArrayList<Integer> v1;
		ret = get(s, mid, tag * 2, l, r, val);
		v1 = get(mid, e, tag * 2 + 1, l, r, val);
		for(int i = 0; i < v1.size(); i++) ret.add(v1.get(i));
		return ret;
	}


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
		build(0, n, 1);
		ArrayList<pair> pnt1 = new ArrayList<pair>();
		ArrayList<pair> pnt2 = new ArrayList<pair>();
		for(int i = 0; i < n; i++){
			pnt1.add(new pair(X[i], 0));
			pnt2.add(new pair(0, X[i]));
		}
		for(int i = 0; i < m; i++){
			ArrayList<Integer> inter = get(0, n, 1, L[i], R[i] + 1, Y[i]);
			for(int j = 0; j < inter.size(); j++){
				int idx = inter.get(j);
				pnt1.add(new pair(X[idx], Y[i]));
				pnt2.add(new pair(Y[i], X[idx]));
			}
			mrk.put(new pair(X[R[i]], Y[i]), true);
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
		for(int i = 0; i < MAXN; i++){
			adj[i] = new ArrayList<pair>();
			neiL[i] = new ArrayList<Integer>();
			neiR[i] = new ArrayList<Integer>();
		}
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
			neiL[L[i]].add(i);
			neiR[R[i]].add(i);
		}
	}

	public long min_distance(int[] XX, int[] HH, int[] LL, int[] RR, int[] YY, int S, int G) {
		X = XX; H = HH; L = LL; R = RR; Y = YY;
		n = X.length;
		m = L.length;
		all_y.add(0);
		for(int i = 0; i < m; i++) all_y.add(Y[i]);
		HashSet<Integer> uniq = new HashSet(all_y);
		all_y = new ArrayList<Integer>(uniq);
		Collections.sort(all_y);
		init();
		build_graph();
		return dijkstra(add_map(X[S], 0), add_map(X[G], 0));
	}
}
