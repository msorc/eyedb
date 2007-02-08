
db=index_test
eyedboql -d $db -w <<EOF
for (n := 0; n < 100; n++) {
  c := new C();
  c.i := n + 1;
  for (m := 0; m < 10; m++) 
   c.ia[m] := n + m + 100;
  for (m := 0; m < 4; m++) 
    c.ib[m] := n + m + 1000;
};
\commit
\q
EOF