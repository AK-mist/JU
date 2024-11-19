#!/bin/bash

OUTPUT_FILE="$1"                    # Process name
COUNTER_INTERVAL=15                 # Interval to print the header
COUNTER="$COUNTER_INTERVAL"         # Counter to print header after certain intervals

# Check if the output file (process name) is provided
if [ -z "$OUTPUT_FILE" ]; then
    echo "Usage: ./page_fault_listener <process-name>"
    exit 1
fi

# Get the PID of the process
PID=$(pgrep -n "$OUTPUT_FILE")

# Exit if the process doesn't exist within 15 seconds
for i in $(seq 1 "$COUNTER_INTERVAL"); do
    if [ -z "$PID" ]; then
        echo "Process '$OUTPUT_FILE' not found. Retrying... ($i/$COUNTER_INTERVAL)"
        sleep 1
        PID=$(pgrep -n "$OUTPUT_FILE")
    else
        break
    fi
done

if [ -z "$PID" ]; then
    echo "Process '$OUTPUT_FILE' not found after $COUNTER_INTERVAL seconds. Exiting..."
    exit 1
fi

echo "process '$OUTPUT_FILE' has started with pid-'$PID'"

# indifinitly check for page faults
while true; do
    # check if still running
    if ! kill -0 "$PID" 2>/dev/null; then
        echo "process '$OUTPUT_FILE' has stopped running"
        exit 1
    fi

    CURRENT_TIME=$(date +%H:%M:%S)

    # print header after every interval 
    if [ "$COUNTER" -ge "$COUNTER_INTERVAL" ]; then
        echo -e "\n\n time \t\t state \t minor_fault \t\t major_fault \t virtual_memory_size \t resident_set_size"
        # reset after printing header
        COUNTER=0
    fi

    # extract and print minor and major page faults
    stat=$(awk -v curr_time="$CURRENT_TIME" '{print curr_time, "\t", $3, "\t\t", $10, "\t\t", $12, "\t\t", $23, "\t\t", $24}' /proc/$PID/stat)
    echo -e "$stat"

    # increment the counter
    COUNTER=$((COUNTER + 1))

    # wait for 1 second
    sleep 1
done