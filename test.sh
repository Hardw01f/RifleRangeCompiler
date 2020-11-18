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
assert 21 '5+20-4'
assert 41 '12+34-5'
assert 150 '100+200-200+100-50'
assert 60 ' 100 + 20 + 30 - 20 - 70'
assert 47 '5+6*7'
assert 15 '5*(9-6)'
assert 23 ' 2 * 4 + ( 3 * 5 ) '
assert 4 '(3+5)/2'
assert 10 '-10+20'
assert 10 '- -10'
assert 10 '- - +10'
assert 0 '0==1'
assert 1 '42==42'
assert 1 '0!=1'
assert 0 '42!=42'

assert 1 '0<1'
assert 0 '1<1'
assert 0 '2<1'
assert 1 '0<=1'
assert 1 '1<=1'
assert 0 '2<=1'

assert 1 '1>0'
assert 0 '1>1'
assert 0 '1>2'
assert 1 '1>=0'
assert 1 '1>=1'
assert 0 '1>=2'

echo "OK : Test ALL Passed"
