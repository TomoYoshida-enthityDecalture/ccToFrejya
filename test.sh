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

assert 10 "b = 20; if (b < 10) return b; else return b - 10;"
assert 0 "x = 5; while (x > 0) x = x - 1; return x;"
assert 120 "ans = 1; for (i = 1; i <= 5; i = i + 1) ans = ans * i; return ans;"

echo OK 
