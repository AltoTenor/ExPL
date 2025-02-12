cd $1
for file in input/*; do
    ./run_no_op "$file"
    file_name="${file##*/}"
    cd ../../..
    if [[ "$file_name" != "input.expl" ]]; then
        echo "Testing $file_name.."
        for input_file in tests/$file_name/input*; do
            extracted_part="${input_file##*/input}"
            ./xsm -l library.xsm -e $1/output.xsm < $input_file > tests/temp
            if diff -q tests/$file_name/output$extracted_part tests/temp >/dev/null; then
                echo "OK $extracted_part"
            else
                echo "Test Failed"
                diff tests/$file_name/output tests/temp
                exit 1
            fi
        done
    fi
    cd $1
done
echo "All Tests Passed ! "