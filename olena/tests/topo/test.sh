#!/bin/sh

./"$1" 2> "$1".run

if cmp -s "$1".run "$SRCDIR"/"$1".ref; then
   echo "PASS"
   exit 0
else
   echo "FAIL"
   diff -u "$SRCDIR"/"$1".out "$1".run
   exit 1
fi
