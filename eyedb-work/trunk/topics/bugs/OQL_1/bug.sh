#!/bin/sh
#
# bug.sh
#

db=bug_OQL_1

oql -d $db -w << EOF
new Group(sctgs: new <> set<Supercontig *> (new Supercontig()));
EOF

oql -d $db -w << EOF
s := new Supercontig();
st := new <> set<Supercontig *> (s);
new Group(sctgs: st);

EOF

