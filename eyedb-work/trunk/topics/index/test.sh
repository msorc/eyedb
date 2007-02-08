db=index_test

eyedboql -d $db -w <<EOF
n := 0;
c := new C();
c.i := n + 1;
c.ia[0] := n + 100;
c.ia[1] := n + 101;
c.ia[2] := n + 102;
c.ia[1] := n + 108;
c.ic[0] := n + 308;
c.ic[1] := n + 408;
c.ic[2] := n + 508;

c.ib[0] := n + 1000;
c.ib[1] := n + 1010;
c.ib[2] := n + 1020;
c.ib[1] := n + 1080;
\commit
\q
EOF