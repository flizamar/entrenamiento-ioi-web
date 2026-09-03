{$R-,S-,Q-}

// BEGIN CUT
type
  TIntArray = array of longint;
// END CUT

function solve_puzzle(S : ansistring; C : TIntArray) : ansistring;

  procedure reverse(var a : TIntArray);
  var
    n, i : longint; t : longint;
    begin
      n := length(a);
      i := 0;
      while i < n - i - 1 do begin
        t := a[i];
        a[i] := a[n - i - 1];
        a[n - i - 1] := t;
        inc(i);
      end;
    end;

var
  n, cb, cw, i, j, k, e, it : longint;
  a, bs, ws, ans, sumb, sumw : TIntArray;
  t : array of boolean;
  dp : array of array of array of boolean;
  res : ansistring;

  begin
    n := length(s);
    cb := 0;
    cw := 0;
    setlength(bs, n);
    setlength(ws, n);
    for i := 1 to n do begin
      if s[i] = 'X' then begin
        bs[cb] := i;
        inc(cb);
      end;
      if s[i] = '_' then begin
        ws[cw] := i;
        inc(cw);
      end;
    end;
    setlength(bs, cb);
    setlength(ws, cw);
    n := n + 2;
    k := length(c);
    setlength(ans, n);
    for i := 0 to n - 1 do ans[i] := 0;
    setlength(dp, 2);
    for i := 0 to 1 do begin
      setlength(dp[i], n);
      for j := 0 to n - 1 do begin
        setlength(dp[i][j], k + 1);
        for e := 0 to k do
          dp[i][j][e] := false;
      end;
    end;
    setlength(sumb, n);
    setlength(sumw, n);
    for it := 0 to 2 do begin
      for i := 0 to n - 1 do sumb[i] := 0;
      for i := 0 to n - 1 do sumw[i] := 0;
      for i := 0 to cb - 1 do inc(sumb[bs[i]]);
      for i := 0 to cw - 1 do inc(sumw[ws[i]]);
      for i := 1 to n - 1 do begin
        sumb[i] := sumb[i] + sumb[i - 1];
        sumw[i] := sumw[i] + sumw[i - 1];
      end;
      if it = 2 then break;
      dp[it][0][0] := true;
      for i := 1 to n - 1 do begin
        for j := 0 to k do begin
          dp[it][i][j] := dp[it][i][j] or (dp[it][i - 1][j] and (sumB[i] - sumB[i - 1] = 0));
          dp[it][i][j] := dp[it][i][j] or ((j > 0) and (i > C[j - 1]) and (sumW[i - 1] - sumW[i - C[j - 1] - 1] = 0) and (sumB[i] - sumB[i - 1] = 0) and dp[it][i - C[j - 1] - 1][j - 1]);
        end;
      end;
      reverse(C);
      reverse(bs);
      reverse(ws);
      for i := 0 to cb - 1 do begin
        bs[i] := n - bs[i] - 1;
      end;
      for i := 0 to cw - 1 do begin
        ws[i] := n - ws[i] - 1;
      end;
    end;
      i := 0;
      while i < n - i - 1 do begin
        t := dp[1][i];
        dp[1][i] := dp[1][n - i - 1];
        dp[1][n - i - 1] := t;
        inc(i);
      end;
    for i := 1 to n - 2 do begin
      for j := 0 to k do begin
        if dp[0][i][j] and dp[1][i][k - j] then 
          ans[i - 1] := ans[i - 1] or 1;
      end;
    end;
    setlength(a, n);
    for i := 0 to n - 1 do a[i] := 0;
    for i := 1 to n - 1 do begin
      for j := 0 to k do begin
        if (j > 0) and (i > C[j - 1]) and (sumW[i - 1] - sumW[i - C[j - 1] - 1] = 0) and (sumB[i] - sumB[i - 1] = 0) and dp[0][i - C[j - 1] - 1][j - 1] and dp[1][i][k - j] then begin
          inc(a[i - C[j - 1]]);
          dec(a[i]);
        end;
      end;
    end;
    for i := 1 to n - 1 do begin
      a[i] := a[i] + a[i - 1];
      if a[i] > 0 then begin
        ans[i - 1] := ans[i - 1] or 2;
      end;
    end;
    setlength(res, n - 2);
    for i := 0 to n - 3 do begin
      if ans[i] = 1 then res[i + 1] := '_' else
      if ans[i] = 2 then res[i + 1] := 'X' else
        res[i + 1] := '?';
    end;
    solve_puzzle := res;
  end;

// BEGIN CUT
var
    s : ansistring;
    c_len, i : longint;
    c : TIntArray;
    ans : ansistring;
begin
    readln(s);
    read(c_len);
    setlength(c, c_len);
    for i := 0 to c_len - 1 do
        read(c[i]);
    ans := solve_puzzle(s, c);

    // BEGIN SECRET
    writeln('098d134608c94f7413faac591054ee35');
    // END SECRET

    writeln(ans);
end.
// END CUT
