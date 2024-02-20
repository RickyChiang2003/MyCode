#!/usr/bin/bash

for x in {1..4}; do
    python3 judge.py sample-testcases/sample-input-${x}.txt -s
done
for x in {1..7}; do
    python3 judge.py mytestcases/input-${x}.txt -s
done
