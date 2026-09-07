{
  Author: Sergey Kopeliovich (Burunduk30@gmail.com) 

  idea : answer is a segment of sorted weights
  time = O(n)
}

type 
    TIntArray = array of longint;

procedure sort(l, r : longint; w, p: TIntArray);
var
	i, j, x, tmp: longint;
begin
	if l >= r then exit;
	i := l;
	j := r;
	x := w[p[(l + r) div 2]];
	while i <= j do begin
		while w[p[i]] < x do inc(i);
		while w[p[j]] > x do dec(j);
		if i <= j then begin
			tmp := p[i]; p[i] := p[j]; p[j] := tmp;
			inc(i);
			dec(j);
		end;
	end;
	sort(l, j, w, p);
	sort(i, r, w, p);
end;

function find_subset(LOW, UP: longint; w: TIntArray): TIntArray;
var
    res, p: TIntArray;
    n, r, l, i: longint;
    sum: int64;
begin
	n := length(w);
    setLength(p, n);
    for i := 0 to n - 1 do 
    	p[i] := i;
    sort(0, n - 1, w, p);
{
    for i := 0 to n - 1 do begin
    	writeln(i, ' ', p[i], ' ', w[p[i]]);
    end;
}
    r := 0;
    sum := 0;
    for l := 0 to n - 1 do begin
    	while (r < n) and (sum < LOW) do begin
    		sum := sum + w[p[r]];
    		inc(r);
    	end;
		if (LOW <= sum) and (sum <= UP) then begin
			setLength(res, r - l);
			for i := l to r - 1 do
				res[i - l] := p[i];
			find_subset := res;
			exit;
		end;
		sum := sum - w[p[l]];
	end;
	setLength(res, 0);
	find_subset := res;
end;

var
	n, l, u, i: longint;
	w, res: TIntArray;
begin
	read(n, l, u);
	setlength(w, n);
	for i := 0 to n - 1 do begin
		read(w[i]);	 
	end;

	res := find_subset(l, u, w);

	// BEGIN SECRET
	writeln('14e047d7a2907b9034950b074822b302');
	// END SECRET

	writeln(length(res));
	for i := 0 to length(res) - 1 do begin
		write(res[i]); 
		if i + 1 = length(res) then
			writeln
		else
			write(' ')
	end;	
end.
