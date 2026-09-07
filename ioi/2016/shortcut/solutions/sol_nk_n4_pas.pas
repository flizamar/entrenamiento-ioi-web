{$mode delphi}

uses Math;

var
	N, C, i: longint;
	L, D: array of longint;

	x, dd: array of int64;
	CC, nn: longint;

function calc(a, b : longint): int64;
var i, j: longint;
begin
	result := 0;
	for i := 0 to nn - 1 do begin
		for j := i + 1 to nn - 1 do begin
			result := max(result, d[i] + d[j] + min(abs(x[i] - x[j]), CC + abs(x[i] - x[a]) + abs(x[j] - x[b])));
		end;
	end;
end;

function find_shortcut(n : longint; l, d: array of longint; c : longint) : int64;
var i, j: longint;
	curx: int64;
begin
	nn := n;
	CC := c;
	setlength(dd, nn);
	setlength(x, nn);
	for i := 0 to nn - 1 do dd[i] := d[i];
	curx := 0;
	for i := 0 to nn - 1 do begin
		x[i] := curx;
		if i + 1 < nn then inc(curx, l[i]);
	end;
	result := int64(1000000000) * 1000000000;
	for i := 0 to nn - 1 do begin
		for j := i + 1 to nn - 1 do begin
			result := min(result, calc(i, j));
		end;
	end;
end;

begin
	read(n);
	read(c);
	setlength(l, n - 1);
	for i := 0 to n - 2 do 
		read(l[i]);
	setlength(d, n);
	for i := 0 to n - 1 do 
		read(d[i]);
	
	// BEGIN SECRET
    writeln('14e047d7a2907b9034950b074822b302');
    // END SECRET

	writeln(find_shortcut(n, l, d, c));
end.
