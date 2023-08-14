#!/bin/bash
assert() {
	expected="$1"
	input="$2"

	./ccToFreyja "$input" > tmp.s
	gcc -o tmp -z noexecstack tmp.s
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
assert 1 "5 + 1 >= 1 * 4;"
assert 1 "5 + 3 != 2 * 6;"
assert 4 "a = 5; a + (-1);"
assert 1 "foo = 5; var = 1; foo + var == 6;"
assert 50 "mal = 10; meg = 20 * (3 - 1); return mal + meg; return 2;"

echo OK 
