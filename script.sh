#!/bin/bash

time (for i in {1..1000}; do ./frog $1 $2 >> a.out; done)
echo ""
sed '/^\s*$$/d' < a.out | sort | uniq -c | sort -nr
rm a.out
