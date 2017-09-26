#!/bin/bash

program_file=$1
test_file=$2

if [ $# != 2 ]; then
	echo "Usage: $0 (program file path) (test file name)"
	exit 0
fi
echo "-------------------LOG START-------------------">./F001.log
if [[ ! -e $program_file ]]; then
	echo "Program to be tested is not found!"
	echo "00: Program to be tested is not found!">>F001.log
	echo "-------------------LOG END---------------------">>./F001.log
	exit -10
fi

if [[ ! -e $test_file ]]; then
	echo "Test file is not found!"
	echo "01: Test file is not found!">>F001.log
	echo "-------------------LOG END---------------------">>./F001.log
	exit -3
fi
	
var=$(echo "wlcq" | $program_file "$test_file")

error_code=$(($?-256))

if [[ $var == *"Reading File..."* && $var == *"File is ready."* && $var == *"Words count is:"* && $var == *"Number of lines in file:"* && $var == *"Characters without spaces:"* && $var == *"Characters with spaces:"* ]] ; then
	echo
	echo "Result:	Test was successfull!"
	echo
	exit 0
elif [ $error_code -eq -1 ]; then
	echo
	echo "Error $error_code: Test failed, because memory isn't available!"
	echo
	exit -1
elif [ $error_code -eq -2 ]; then
	echo
	echo "Error $error_code: Test failed, because file is empty!"
	echo
	exit -2
elif [ $error_code -eq -3 ]; then
	echo
	echo "Error $error_code: Test failed, because file was not found!"
	echo
	exit -3
elif [ $error_code -eq -5 ]; then
	echo
	echo "Error $error_code: Test failed, file input to the test program has a format which is not supported!"
	echo
	exit -5
else
	echo
	echo "Result:	Test failed!"
	echo
	exit -20
fi

