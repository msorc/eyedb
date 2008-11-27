DATABASE=$1
N=$2
time eyedboql -w -d polepos --commit -c "N := $N; n := 0; while (n < N) { select b from B4 as b where b.b2 = (n+1); n++; };"

