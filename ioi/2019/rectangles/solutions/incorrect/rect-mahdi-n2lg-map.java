//IOI 2019
//Rect - Java - Version 1 - First Approach
//Current Version: 27 May 2019
//Mahdi Safarnejad Boroujeni

import java.util.ArrayList;
import java.util.Collections;
import java.util.Map;
import java.util.SortedMap;
import java.util.TreeMap;

class Pair implements Comparable<Pair> {
  final int first, second;
  Pair(int first, int second) {
  	this.first=first;
  	this.second=second;
  }
  @Override
    public int compareTo(Pair b) {
    	if (first!=b.first)
    		return first - b.first;
    	return second - b.second;
    }
}

class fenwickTree {
	final int maxn = 10000 + 110;
	int[] fnwk;
	ArrayList<Pair> fnwkHistory;
	public fenwickTree() {
		fnwk = new int[maxn];
		fnwkHistory = new ArrayList<Pair>();
	}
	void fnwkUpdate(int index, int val, boolean silent) {
		if (!silent)
			fnwkHistory.add(new Pair(index, val));
		while (index < maxn) {
			fnwk[index] += val;
			index += index & (-index);
		}
	}

	int fnwkSum(int index) {
		int result = 0;
		while (index > 0) {
			result += fnwk[index];
			index -= index & (-index);
		}
		return result;
	}

	void resetFnwk() {
		for (int i = 0; i < fnwkHistory.size(); i++)
			fnwkUpdate(fnwkHistory.get(i).first, -fnwkHistory.get(i).second, true);
		fnwkHistory.clear();
	}
}

class Tpl implements Comparable<Tpl> {
	int r, c, rc2;
	Tpl(int r, int c, int rc2) {
		this.r = r;
		this.c = c;
		this.rc2 = rc2;
	}
	@Override
      public int compareTo(Tpl b) {
		if (r!=b.r) return r - b.r;
		if (c!=b.c) return c - b.c;
		return rc2 - b.rc2;
	  }
}

class rect {
	TreeMap<Tpl, Integer> horizontal, vertical;

	void addh(int i, int j, int j2) {
		if (j-j2 <= 1) return;
		int val = 1;
		Tpl above = new Tpl(i-1, j, j2);
		if (horizontal.containsKey(above))
			val += horizontal.get(above);
		horizontal.put(new Tpl(i, j, j2), val);
	}

	void addv(int i, int j, int i2) {
		if (i-i2 <= 1) return;
		int val = 1;
		Tpl left = new Tpl(i, j-1, i2);
		if (vertical.containsKey(left))
			val += vertical.get(left);
		vertical.put(new Tpl(i, j, i2), val);
	}

	public long count_rectangles(int[][] a) {
		horizontal = new TreeMap<Tpl, Integer>();
		vertical = new TreeMap<Tpl, Integer>();
		int n = a.length;
		int m = a[0].length;
		for (int i = 0; i < n; i++) {
			ArrayList<Integer> stack = new ArrayList<Integer>();
			for (int j = 0; j < m; j++) {
				int lastElement = Integer.MIN_VALUE;
				while (!stack.isEmpty() && a[i][j] > a[i][stack.get(stack.size()-1)]) {
					if (a[i][stack.get(stack.size()-1)] > lastElement)
						addh(i, j, stack.get(stack.size()-1));
					lastElement = a[i][stack.get(stack.size()-1)];
					stack.remove(stack.size()-1);
				}
				if (stack.size() > 0)
					if (a[i][stack.get(stack.size()-1)] > lastElement)
						addh(i, j, stack.get(stack.size()-1));
				stack.add(j);					
			}
		}
		for (int j = 0; j < m; j++) {
			ArrayList<Integer> stack = new ArrayList<Integer>();
			for (int i = 0; i < n; i++) {
				int lastElement = Integer.MIN_VALUE;
				while (!stack.isEmpty() && a[i][j] > a[stack.get(stack.size()-1)][j]) {
					if (a[stack.get(stack.size()-1)][j] > lastElement)
						addv(i, j, stack.get(stack.size()-1));
					lastElement = a[stack.get(stack.size()-1)][j];
					stack.remove(stack.size()-1);
				}
				if (stack.size() > 0)
					if (a[stack.get(stack.size()-1)][j] > lastElement)
						addv(i, j, stack.get(stack.size()-1));
				stack.add(i);
			}
		}
		fenwickTree fen = new fenwickTree();
		int sum = 0;
		for (int i = 2; i < n; i++)
			for (int j = 2; j < m; j++) {
				ArrayList<Pair> vecHor = new ArrayList<Pair>();
				ArrayList<Pair> vecVer = new ArrayList<Pair>();
				SortedMap<Tpl, Integer> thisHor = horizontal.subMap(new Tpl(i-1,j,-1), new Tpl(i-1,j,Integer.MAX_VALUE));
				for(Map.Entry<Tpl, Integer> entryHor : thisHor.entrySet())
					vecHor.add(new Pair(j-1-(entryHor.getKey()).rc2, entryHor.getValue()));
				SortedMap<Tpl, Integer> thisVer = vertical.subMap(new Tpl(i,j-1,-1), new Tpl(i,j-1,Integer.MAX_VALUE));
				for(Map.Entry<Tpl, Integer> entryVer : thisVer.entrySet())
					vecVer.add(new Pair(entryVer.getValue(), i-1-(entryVer.getKey()).rc2));
				if (vecHor.isEmpty() || vecVer.isEmpty()) continue;
				Collections.sort(vecVer);
				int cur=vecVer.size() -1;
				for (int k = 0; k < vecHor.size(); k++) {
					while (cur>=0 && vecHor.get(k).first <= vecVer.get(cur).first)
						fen.fnwkUpdate(vecVer.get(cur--).second, 1, false);
					sum += fen.fnwkSum(vecHor.get(k).second);
				}
				fen.resetFnwk();
			}
		return sum;
	}
}
