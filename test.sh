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
assert 15 "5*(9 - 6) "
assert 224 " 22 * 10 +(4 + 2 /2)  - 1"

echo OK 
