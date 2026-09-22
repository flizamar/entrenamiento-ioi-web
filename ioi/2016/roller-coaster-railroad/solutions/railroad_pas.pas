{unit railroad;

interface

type
    TIntArray = array of longint;

function plan_roller_coaster(s, t : TIntArray) : Int64;

implementation
}

uses Math;

// BEGIN CUT
type
    TIntArray = array of longint;
// END CUT
const INF = 1000000000;


function plan_roller_coaster(s, t : TIntArray) : Int64;
var 
    id, p, a, b, c, aa, bb, cc : array of longint;

    function get(x : longint) : longint;
      begin
        if x <> p[x] then p[x] := get(p[x]);
        get := p[x];
      end;

    function join(x, y : longint) : boolean;
      begin
        x := get(x);
        y := get(y);
        p[x] := y;
        join := x <> y;
      end;

    function less(x, y : longint) : boolean;
      begin
        if a[x] <> a[y] then begin
          exit(a[x] < a[y]);
        end;
        if b[x] <> b[y] then begin
          exit(b[x] < b[y]);
        end;
        exit(c[x] < c[y]);
      end;

    procedure sort(l, r : longint);
    var
      i, j, t, x : longint;
      begin
        if l >= r then exit;
        i := l;
        j := r;
        x := id[random(r - l + 1) + l];
        while i <= j do begin
          while less(id[i], x) do inc(i);
          while less(x, id[j]) do dec(j);
          if i <= j then begin
            t := id[i];
            id[i] := id[j];
            id[j] := t;
            inc(i);
            dec(j);
          end;
        end;
        sort(i, r);
        sort(l, j);
      end;

var
  i, len, n, cn, delta : longint;
  res : int64;

begin
  n := Length(s);
  setlength(a, (n + 1) * 2);
  setlength(b, (n + 1) * 2);
  setlength(c, (n + 1) * 2);
  setlength(aa, (n + 1) * 2);
  setlength(bb, (n + 1) * 2);
  setlength(cc, (n + 1) * 2);
  setlength(id, (n + 1) * 2);
  len := 0;
  for i := 0 to n - 1 do begin
    a[len] := s[i];
    b[len] := 1;
    c[len] := i;
    inc(len);
    a[len] := t[i];
    b[len] := -1;
    c[len] := i;
    inc(len);
  end;
  a[len] := INF;
  b[len] := 1;
  c[len] := n;
  inc(len);
  a[len] := 1;
  b[len] := -1;
  c[len] := n;
  inc(len);
  inc(n);
  setlength(p, n);
  for i := 0 to n - 1 do p[i] := i;
  for i := 0 to len - 1 do id[i] := i;
  sort(0, len - 1);
  cn := 0;
  res := 0;
  delta := 0;
  for i := 0 to len - 2 do begin
    delta := delta + b[id[i]];
    res := res + max(0, delta) * int64(a[id[i + 1]] - a[id[i]]);
    if (a[id[i + 1]] = a[id[i]]) or (delta <> 0) then join(c[id[i]], c[id[i + 1]]);
    aa[cn] := a[id[i + 1]] - a[id[i]];
    bb[cn] := c[id[i]];
    cc[cn] := c[id[i + 1]];
    inc(cn);
  end;
  for i := 0 to cn - 1 do begin
    a[i] := aa[i];
    b[i] := bb[i];
    c[i] := cc[i];
    id[i] := i;
  end;
  sort(0, cn - 1);
  for i := 0 to cn - 1 do begin
    if (join(b[id[i]], c[id[i]])) then begin
      res := res + a[id[i]];
    end;
  end;
  plan_roller_coaster := res;
end;

// BEGIN CUT
var i, n, needAnswer : Longint;
    s, t : TIntArray;
    answer : int64;

begin
  read(n);
  setLength(s, n);
  setLength(t, n);
  read(needAnswer);
  for i := 0 to n - 1 do
    read(s[i], t[i]);
  
  answer := plan_roller_coaster(s, t);
  
  // BEGIN SECRET
  writeln('14e047d7a2907b9034950b074822b302');
  // END SECRET
  
  writeln(answer);

// END CUT
end.
