db=index_test

oql -d $db -w <<EOF
select C.ia[1];
select C.ia[2];
select C.ia[3] = 103;
\abort
\q

EOF