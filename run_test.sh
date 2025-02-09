cd $1
for file in input/*; do
    ./run_no_op "$file"
    file_name="${file##*/}"
    cd ../../..
    if [[ "$file_name" != "input.expl" ]]; then
        echo "Testing $file_name.."
        ./xsm -l library.xsm -e $1/output.xsm < tests/$file_name/input > tests/temp
        if diff -q tests/$file_name/output tests/temp >/dev/null; then
            echo "OK"
        else
            echo "Test Failed"
            diff tests/$file_name/output tests/temp
            exit 1
        fi
    fi
    cd $1
done
echo "All Tests Passed ! "