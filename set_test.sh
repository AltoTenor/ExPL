cd $1
for file in input/*; do
    ./run_no_op "$file"
    file_name="${file##*/}"
    echo "Setting output for $file_name.."
    cd ../../..
    if [[ "$file_name" != "input.expl" ]]; then
        ./xsm -l library.xsm -e $1/output.xsm < tests/$file_name/input > tests/$file_name/output
    fi
    cd $1
done