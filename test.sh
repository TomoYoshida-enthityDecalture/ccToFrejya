#!/bin/bash
assert() {
	expected="$1"
	input="$2"

	./ccToFrejya "$input" > tmp.s
	gcc -o tmp tmp.s
	./tmp
	actual="$?"

	if [ "$actual" = "$expected" ]; then
		echo "$input => $actual"
	else
		echo "expected $expected, but got $actual"
		exit 1
	fi
}

assert 42 "40+5-3"
assert 0 " 10 - 9 - 1 "
assert 17 " 41 - 39 +  2 +13 "

echo OK 
