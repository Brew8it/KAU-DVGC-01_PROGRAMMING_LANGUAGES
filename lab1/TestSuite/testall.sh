#!/bin/sh

for i in *.pas; do
	echo "testing $i"
	./parser <  $i
done
