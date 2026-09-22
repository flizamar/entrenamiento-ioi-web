uses Math;


// BEGIN CUT
type TIntArray = array of longint;
// END CUT

function take_photos(n, m, k : longint; x, y : TIntArray) : int64;
var
  ff, ss, fs : array of longint;
  cas, bs : array of int64;
  size, opt : longint;


procedure sort(l, r : longint);
var
  i, j, m, fx, sx, t : longint;
  begin
    if l >= r then exit;
    i := l;
    j := r;
    m := random(r - l + 1) + l;
    fx := ff[m];
    sx := ss[m];
    while i <= j do begin
      while (ff[i] < fx) or ((ff[i] = fx) and (ss[i] > sx)) do inc(i);
      while (fx < ff[j]) or ((fx = ff[j]) and (sx > ss[j])) do dec(j);
      if i <= j then begin
        t := ff[i];
        ff[i] := ff[j];
        ff[j] := t;
        t := ss[i];
        ss[i] := ss[j];
        ss[j] := t;
        inc(i);
        dec(j);
      end;
    end;
    sort(l, j);
    sort(i, r);
  end;

procedure clear_cht;
  begin
    size := 0;
    opt := 0;
  end;

function intersection(a1, b1, a2, b2 : int64) : extended;
  var
    double1: double = 1.0;
  begin
    intersection := double1 * (b2 - b1) / (a1 - a2);
  end;

function get_max(v : int64) : longint;
var
  x : extended;
  begin
    if opt > size - 1 then opt := size - 1;
    while opt + 1 < size do begin
      x := intersection(cas[opt], bs[opt], cas[opt + 1], bs[opt + 1]);
      if v > x then begin
        inc(opt);
      end else begin
        break;
      end;
    end;
    get_max := opt;
  end;

procedure add_line(a, b : int64; bomb : longint);
var
  x1, x2 : extended;
  begin
    while size > 1 do begin
      x1 := intersection(cas[size - 2], bs[size - 2], cas[size - 1], bs[size - 1]);
      x2 := intersection(cas[size - 1], bs[size - 1], a, b);
      if x1 < x2 then break else dec(size);
    end;
    cas[size] := a;
    bs[size] := b;
    fs[size] := bomb;
    inc(size);
  end;

function sqr(x : int64) : int64;
  begin
    sqr := x * x;
  end;

procedure getValue(n: longint; c : int64; var value_res, bombs_res : int64);
var
  line, i: longint;
  value, bombs : int64;
  begin
    clear_cht;
    add_line(-2 * (ff[0] - 1), sqr(ff[0] - 1), 0);
    for i := 1 to n do begin
      line := get_max(ss[i - 1]);
      value := cas[line] * ss[i - 1] + bs[line] + sqr(ss[i - 1]) + c;
      bombs := fs[line] + 1;
      if i < n then begin
        add_line(-2 * (ff[i] - 1), value + sqr(ff[i] - 1) - sqr(max(0, ss[i - 1] - ff[i] + 1)), bombs);
      end else begin
        value_res := value;
        bombs_res := bombs;
        exit;
      end;
    end;
  end;

var
  t, maxRight : longint;
  diff, left, right, mid, value, bombs, value1, bombs1, value2, bombs2, answer1, answer2 : int64;
  i, cn : longint;
  begin
    setlength(ff, n);
    setlength(ss, n);
    for i := 0 to n - 1 do begin
      ff[i] := x[i];
      ss[i] := y[i];
      if ff[i] > ss[i] then begin
        t := ff[i];
        ff[i] := ss[i];
        ss[i] := t;
      end;
    end;
    setlength(cas, n);
    setlength(bs, n);
    setlength(fs, n);
    sort(0, n - 1);
    maxRight := -1;
    cn := 0;
    for i := 0 to n - 1 do begin
      if ss[i] > maxRight then begin
        maxRight := ss[i];
        ff[cn] := ff[i];
        ss[cn] := ss[i];
        inc(cn);
      end;
    end;
    n := cn;
    k := min(k, n);
    clear_cht;
    left := 0;
    right := int64(m) * m + 1;
    if odd(right) then inc(right);
    while left < right - 1 do begin
      mid := (left + right) shr 1;
      getValue(n, mid + 1, value, bombs);
      if bombs < k then right := mid else left := mid;
    end;
    getValue(n, left + 1, value1, bombs1);
    getValue(n, right + 1, value2, bombs2);
    answer1 := value1 - bombs1 * (left + 1);
    answer2 := value2 - bombs2 * (right + 1);
    if bombs1 = k then take_photos := answer1 else
    if bombs2 = k then take_photos := answer2 else begin
      diff := answer1 - answer2;
      take_photos := answer1 + diff div (bombs1 - bombs2) * (k - bombs1);
    end;
  end;

// BEGIN CUT
var
  n, m, k, i : longint;
  r, c : TIntArray;
  ans : int64;
begin
  read(n, m, k);
  setlength(r, n);
  setlength(c, n);
  for i := 0 to n - 1 do
    read(r[i], c[i]);
  ans := take_photos(n, m, k, r, c);

  // BEGIN SECRET
  writeln('098d134608c94f7413faac591054ee35');
  // END SECRET

  writeln(ans);
// END CUT

end.
