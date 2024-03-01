#!/bin/bash

# Function to compare files and print result
compare_files() {
    local ref_file="$1"
    local test_file="$2"
    local result
    diff -q "$ref_file" "$test_file" > /dev/null
    if [ $? -eq 0 ]; then
        result="\e[32mPASSED\e[0m"  # GREEN PASSED
        total_points=$((total_points + 1))
    else
        result="\e[31mFAILED\e[0m"  # RED FAILED
    fi
    echo -e "$result: $test_file"
}

make build

# Check if the compilation was successful
if [ $? -ne 0 ]; then
    echo -e "\e[31mError: Compilation failed. Exiting.\e[0m"
    exit 1
fi

# Define directories
tests_dir="../tests"
output_dir="./tests_out"
mkdir -p "$output_dir/t1"
mkdir -p "$output_dir/t2"

run_tests() {
    local test_dir="$1"
    local total_points=0
    local passed_tests=0
    local total_tests=0

    echo -e "\e[35m=========================== TEST SUITE $test_dir ===========================\n\e[0m"

    for test_file in "$tests_dir/$test_dir"/*; do
        # Extract the filename without the path and extension
        test_name=$(basename "$test_file" .txt)
        # Define the output file path
        output_file="$output_dir/$test_dir/${test_name}.txt"

        # Execute the test with bitwise and write the output
        { time ../bitwise < "$test_file" > "$output_file"; } 2> "$output_dir/$test_dir/${test_name}_time.txt"
        echo -e "\e[33mTest executed\e[0m: $test_name, output written to $output_file"  # Orange color for test executed

        # Compare the output with the reference
        compare_files "reference/$test_dir/${test_name}.txt" "$output_file"
        # Check if the test passed and increment the passed_tests counter
        if [ $? -eq 0 ]; then
            passed_tests=$((passed_tests + 1))
        fi
        total_points=$((total_points + 1))
        total_tests=$((total_tests + 1))
    done
    echo -e "\e[33m\nTotal points obtained for $test_dir category: $passed_tests out of ${total_tests}\e[0m"
    # Check if all tests passed for the category
    if [ $passed_tests -eq $total_tests ]; then
        echo -e "\e[32mAll tests passed for $test_dir category\n\e[0m"
    else
        echo -e "\e[31mSome tests failed for $test_dir category\n\e[0m"
    fi
}

run_tests "t1"
run_tests "t2"

# Print timestamp with blue color
echo -e "\e[34mTests completed at $(date +"%Y-%m-%d %H:%M:%S")\e[0m"

make clean
