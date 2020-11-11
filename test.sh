#!/bin/bash

CompilerName="compiler"

assert() {
    expected="$1"
    input="$2"

    ./$CompilerName "$input" > tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual : Test Passed "
    else
        echo "$input ==> $expected expected, but got $actual"
        exit 1
    fi
}

assert 0 0 
assert 42 42

echo "OK"
