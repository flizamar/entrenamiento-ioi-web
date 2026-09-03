
{unit shortcut;

interface

type
  TIntArray = array of longint;

function find_shortcut(n_ : longint; l0, l : TIntArray; c_ : longint) : int64;

implementation}
    
uses Math;

// BEGIN CUT
type
  TIntArray = array of longint;
// END CUT

const maxn = 1000005;
const inf = 1000000000000000000;

var
  x, d : array[0 .. maxn] of int64;
  qfi, qse : array[0 .. maxn] of int64;
  ans1, ans2 : longint;
  n, cc : longint;

function max3(a, b, c : int64) : int64;
  begin
    if b > a then a := b;
    if c > a then a := c;
    max3 := a;
  end;

function can(diam : int64) : boolean;
var
  maxsum, minsum, maxdif, mindif : int64;
  mostleftfi, mostleftse, mostrightfi, mostrightse : int64;
  cur, l, r, i, curdif, cursum, wh : longint;
  needleft : boolean;
  begin
    needleft := false;
    maxsum := inf;
    minsum := -inf;
    maxdif := inf;
    mindif := -inf;
    mostleftfi := inf;
    mostleftse := -inf;
    mostrightfi := -inf;
    mostrightse := -inf;
    l := 0;
    r := 0;
    for i := 0 to n - 1 do begin
      while (r > l) and (x[i] - qfi[l] + d[i] > diam) do begin
        wh := qse[l];
        inc(l);
        if (x[wh] - d[wh] < mostleftfi - mostleftse) then begin mostleftfi := x[wh]; mostleftse := d[wh]; end;
        if (x[wh] + d[wh] > mostrightfi + mostrightse) then begin mostrightfi := x[wh]; mostrightse := d[wh]; end;
        needleft := true;
      end;
      if needleft then begin
        maxsum := min(maxsum, (mostleftfi + diam - cc - d[i] - mostleftse) + x[i]);
        minsum := max(minsum, (mostrightfi - (diam - cc - d[i] - mostrightse)) + x[i]);
        maxdif := min(maxdif, x[i] - (mostrightfi - (diam - cc - d[i] - mostrightse)));
        mindif := max(mindif, x[i] - (mostleftfi + diam - cc - d[i] - mostleftse));
      end;
      while (r > l) and (qfi[r - 1] > x[i] - d[i]) do dec(r);
      qfi[r] := x[i] - d[i];
      qse[r] := i;
      inc(r);
    end;
    if (maxsum < minsum) or (maxdif < mindif) then exit(false);
    curdif := 0;
    cursum := n;
    for i := 0 to n - 1 do begin
      while (curdif < n) and (x[curdif] - x[i] < mindif) do inc(curdif);
      while (cursum > 0) and (x[cursum - 1] + x[i] >= minsum) do dec(cursum);
      cur := max3(cursum, curdif, i + 1);
      if (cur < n) and (x[cur] + x[i] <= maxsum) and (x[cur] - x[i] <= maxdif) then begin
        ans1 := i;
        ans2 := cur;
        exit(true);
      end;
    end;
    exit(false);
  end;

function find_shortcut(n_ : longint; l0, l : TIntArray; c_ : longint) : int64;
var
  cursum, left, right, mid : int64;
  i : longint;
  begin
    n := n_;
    cc := c_;
    cursum := 0;
    for i := 0 to n - 1 do begin
      d[i] := l[i];
      x[i] := cursum;
      if i + 1 < n then cursum := cursum + l0[i];
    end;
    left := 0;
    right := inf;
    while right - left > 1 do begin
      mid := (left + right) shr 1;
      if can(mid) then right := mid else left := mid;
    end;
    can(right);
    exit(right);
  end;

// BEGIN CUT
var
  nn, c, i : longint;
  l, dd : TIntArray;

begin
  read(nn);
  read(c);
  setlength(l, nn - 1);
  for i := 0 to nn - 2 do 
    read(l[i]);
  setlength(dd, nn);
  for i := 0 to nn - 1 do 
    read(dd[i]);
    
  // BEGIN SECRET
    writeln('14e047d7a2907b9034950b074822b302');
    // END SECRET
    
  writeln(find_shortcut(nn, l, dd, c));

// END CUT
end.
