#!/usr/bin/env bash


echo_divider(){
    echo "--------------------"
}


gcc -o gen $1
gcc -o a $2
gcc -o b $3
time=$4
for i in { 1 .. $time }; do
    ./gen $i > "in.txt"
    ./a < "in.txt" > out_a
    ./b < "in.txt" > out_b
    read va < out_a
    read vb < out_b
    if [[ $va -ne $vb ]]; then
	echo "Test $i"
	echo "Input: "
	echo_divider
	cat "in.txt"
	echo_divider
	echo "Output of $2:"
	echo_divider
	cat out_a
	echo_divider
	echo "Output of $3:"
	echo_divider
	cat out_b
	echo_divider
	break
    fi
done
rm -f out_a
rm -f out_b

