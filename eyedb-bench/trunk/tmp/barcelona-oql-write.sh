DATABASE=$1
N=$2
time eyedboql -w -d polepos --commit -c "N := $N; n := 0; while (n < N) { new B4( b4:n, b3:n, b2:n, b1:n, b0:n); n++; };"

