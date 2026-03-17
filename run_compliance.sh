#!/bin/bash

HARNESS="./compliance_harness"
TEST_DIR="../JSONTestSuite/test_parsing" # Adjust path as needed

PASS_COUNT=0
FAIL_COUNT=0

for file in "$TEST_DIR"/*.json; do
    filename=$(basename "$file")
    prefix=${filename:0:2}
    
    # Run the harness
    $HARNESS "$file"
    exit_code=$?

    if [ "$prefix" == "y_" ]; then
        if [ $exit_code -eq 0 ]; then
            ((PASS_COUNT++))
        else
            echo "FAIL (Should Accept): $filename"
            ((FAIL_COUNT++))
        fi
    elif [ "$prefix" == "n_" ]; then
        if [ $exit_code -ne 0 ]; then
            ((PASS_COUNT++))
        else
            echo "FAIL (Should Reject): $filename"
            ((FAIL_COUNT++))
        fi
    elif [ "$prefix" == "i_" ]; then
        # Implementation defined - just log it
        if [ $exit_code -eq 0 ]; then
            echo "INFO (Accepted i_ file): $filename"
        else
            echo "INFO (Rejected i_ file): $filename"
        fi
    fi
done

echo "-----------------------"
echo "Passed: $PASS_COUNT"
echo "Failed: $FAIL_COUNT"

if [ $FAIL_COUNT -eq 0 ]; then
    echo "SUCCESS: Fully RFC 8259 Compliant!"
    exit 0
else
    echo "ERROR: Compliance failures detected."
    exit 1
fi
