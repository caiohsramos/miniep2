#!/bin/bash

for i in {1..1000}; do ./1 >> a.out; done
sed '/^\s*$$/d' < a.out | sort | uniq -c | sort -nr
rm a.out
