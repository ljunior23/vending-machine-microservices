#!/bin/bash

# Output file
OUTPUT_FILE="output.txt"

# Clear previous output
echo "Vending Machine Test Results" > $OUTPUT_FILE
echo "===========================" >> $OUTPUT_FILE
echo "Date: $(date)" >> $OUTPUT_FILE
echo "===========================" >> $OUTPUT_FILE

# Loop through city IDs 1-100
for i in $(seq 1 100)
do
    echo -n "Testing city ID $i: " >> $OUTPUT_FILE
    
    # Send GET request to the correct endpoint
    RESPONSE=$(curl -s "http://127.0.0.1:8080/order-beverage/city/$i")
    
    # Save response
    echo "$RESPONSE" >> $OUTPUT_FILE
    echo "--------------------------" >> $OUTPUT_FILE
    
    # Optional: Display progress
    echo -n "."
    
    # Add a small delay to prevent overwhelming the service
    sleep 0.2
done

echo ""
echo "Test complete! Results saved to $OUTPUT_FILE"
