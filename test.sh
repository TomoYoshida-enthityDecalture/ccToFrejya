#!/bin/bash
assert() {
	expected="$1"
	input="$2"

	./ccToFreyja "$input" > tmp.s
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

assert 42 "40+5-3;"
assert 15 "5*(9 - 6);"
assert 224 " 22 * 10 +(4 + 2 /2)  - 1;"
assert 2 "-3 + 5;"
assert 3 "+5 - 2;"
assert 1 "5 + 3 == 2 * 4;"
assert 1 "5 + 3 <= 3 * 4;"
assert 1 "5 + 1 >= 1 * 4;"
assert 1 "5 + 3 != 2 * 6;"
assert 4 "a = 5; a + (-1);"

echo OK 
