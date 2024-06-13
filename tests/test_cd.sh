#!/bin/sh

# tests/test_cd.sh

./bin/shell << EOF
cd /usr
pwd
cd /non-existing-directory
cd ~
pwd
exit 0
EOF
