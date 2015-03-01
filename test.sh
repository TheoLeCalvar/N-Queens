#!/bin/bash

# Automatically run tests from test/ directory, and format the output.

tname="*"
if [ $# -ge 1 ]; then
        tname=$1
fi


tests=`ls test/$tname.test | sort`

global_count=0
global_failed=0

echo "-------------------------------------------------------------------------"
echo -e "\033[1m                              Tests report\033[0m"
echo "-------------------------------------------------------------------------"

for test in ${tests[@]}; do
        count=0
        failed=0
        traces=""

        echo -e "\033[1m$test:\033[0m"
        ./$test > .test_tmp.txt 2>&1

        success="true"
        while read line; do
                fail=`echo $line | grep "Failure"`
                assert=`echo $line | grep "assert"`
                ok=`echo $line | grep "Ok"`
                if [ "$fail" != "" ]; then
                        echo -en "    \033[31m$line \033[0m\n"
                        success=""
                        count=$(($count+1))
                        failed=$(($failed+1))
                elif [ "$assert" != "" ]; then
                        echo -en "$traces"
                        traces=""
                        echo -en "    \033[33m>> $line \033[0m\n"
                        success=""
                elif [ "$ok" != "" ]; then
                        echo -en "    \033[32m$line \033[0m\n"
                        count=$(($count+1))
                        echo -en "$traces"
                        traces=""
                else  # Program trace.
                        traces="$traces    > $line\n"
                fi
        done < .test_tmp.txt

        if [ "$success" = "true" ]; then
                echo -en "\033[32;1mSuccess\033[0m, failed "
        else
                echo -en "\033[31;1mFailure\033[0m, failed "
        fi

        ratio=`bc <<< "scale=1;($failed / $count) * 100"`
        echo -e "$failed / $count ($ratio%)"

        global_count=$(($global_count + $count)) 
        global_failed=$(($global_failed + $failed)) 
        echo "-------------------------------------------------------------------------"
done

if [ $global_failed -ge 1 ]; then
        ratio=`bc <<< "scale=2;($global_failed / $global_count) * 100"`
        echo -e "\033[31;1mGlobal tests failed: $global_failed / $global_count ($ratio%)\033[0m"
else
        echo -e "\033[32;1mGlobal success\033[0m"
fi

rm -f .test_tmp.txt
