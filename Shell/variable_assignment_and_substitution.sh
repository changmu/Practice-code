#!/bin/bash

a=375
hello=$a
echo hello
echo $hello
echo ${hello}
echo "$hello"
echo "${hello}"

hello="A B C        D"
echo $hello
echo "$hello"
echo '$hello'

hello=
echo "\$hello (null value) = $hello"

uninitialized_variable=23
unset uninitialized_variable
echo "uninitialized_variable = $uninitialized_variable"

uninitialized_variable2=
if [ -z "$uninitialized_variable2" ]
then
        echo "\$uninitialized_variable2 is null."
fi

echo "$a"
let "a += 5"
echo "a = $a"

echo -n "Enter \"a\" "
# read a
echo "the value of \"a\" is now $a."

for a in 7 8 9    10
do
        echo -n "$a "
done

a=`ls -l`
echo $a
echo "$a"

a="1234353"
let "a += 1"
echo $a
b=${a/3/a}
echo $b

e=''
echo "e = $e"
let "e += 1"
echo "e = $e"

echo $0
echo $10
echo "${10}"
echo "`basename $0`"
echo $*
echo $@
echo "parameters' num not enough: $#"

args=$#
last=${!args}
echo $last
echo ${!$#}
echo ${!#}
