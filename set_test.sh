cd $1
for file in input/*; do
    ./run_no_op "$file"
    file_name="${file##*/}"
    echo "Setting output for $file_name.."
    cd ../../..
    if [[ "$file_name" != "input.expl" ]]; then
        for input_file in tests/$file_name/input*; do
            extracted_part="${input_file##*/input}"
            ./xsm -l library.xsm -e $1/output.xsm < tests/$file_name/input$extracted_part > tests/$file_name/output$extracted_part
        done
    fi
    cd $1
done