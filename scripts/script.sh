#!/bin/bash

readonly program_file=$1
readonly test_file=$2

function run_test {
	var=$(echo "wlcq" | $1 "$2")
	error_code=$(($?-256))
	error_causes_array=()

	error_causes_array[1]="Error $error_code: Test failed, because memory isn't available!"
	error_causes_array[2]="Error $error_code: Test failed, because file is empty!"
	error_causes_array[3]="Error $error_code: Test failed, because file was not found!"
	error_causes_array[5]="Error $error_code: Test failed, file input to the test program has a format which is not supported!"

	if [[ $var == *"Reading File..."* && $var == *"File is ready."* && $var == *"Words count is:"* && $var == *"Number of lines in file:"* && $var == *"Characters without spaces:"* && $var == *"Characters with spaces:"* ]] ; then
		echo
		echo "Result:	Test was successfull!"
		echo
		exit 0
	elif [ $error_code -lt 0 ]; then
		echo
		echo ${error_causes_array[$((-1 * $error_code))]}
		echo
		exit $error_code
	else
		echo
		echo "Result:	Test failed!"
		echo
		exit -20
	fi		
}

function validate_input {

	if [[ $3 != 2 ]]; then
		echo "Usage: $0 (program file path) (test file name)"
		exit 0
	fi
	echo "-------------------LOG START-------------------">./F001.log
	if [[ ! -e $1 ]]; then
		echo "Program to be tested is not found!"
		echo "00: Program to be tested is not found!">>F001.log
		echo "-------------------LOG END---------------------">>./F001.log
		exit -10
	fi

	if [[ ! -e $2 ]]; then
		echo "Test file is not found!"
		echo "01: Test file is not found!">>F001.log
		echo "-------------------LOG END---------------------">>./F001.log
		exit -3
	fi
}

validate_input $program_file "$test_file" $#
run_test $program_file "$test_file"


	


