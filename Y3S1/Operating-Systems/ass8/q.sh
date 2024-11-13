#!/bin/bash

# ***********************************************************************
# *
# * Name: 
# * Roll: 
# * 
# * Date:
# *
# * Assignment: 8, Shell Script Programming
# *
# * Input Description: In Question 8 & 10, user needs to enter '2 real numbers' & 'a value for n' respectively
# *
# * Output Description: Output of each question
# *
# *
# * Compilation Command: None
# * Execution Sequence: ./q.sh
# *
# *
# * Sample Input:
# * Sample Output:
# /-----------------------------------
# 
# -----------------------------------/
# ***********************************************************************

DIR_PATH="$(pwd)"
# DIR_PATH="$HOME"

# ***********************************************************************
# 1. Create a directory ‘All My Cprogs Files’ in your home directory.

NEW_DIR="All My Cprogs Files"
if [ -d "$DIR_PATH/$NEW_DIR" ]; then
    rm -r "$DIR_PATH/$NEW_DIR"
fi
mkdir "$DIR_PATH/$NEW_DIR"
echo "1. Directory '$NEW_DIR' was created in '$DIR_PATH'"

# ***********************************************************************
# 2. Copy all the C files to the new creating directory.

echo -e "2. \c"
find "$DIR_PATH" -path "$DIR_PATH/$NEW_DIR" -prune -o -name "*.c" -exec cp {} "$DIR_PATH/$NEW_DIR" \;
echo "C files copied to '$DIR_PATH/$NEW_DIR'"

# ***********************************************************************
# 3. Show the list of files in your current directory.
 
echo -e "3. List of files in the current directory"
echo -e "\t\c"; find . -maxdepth 1 -type f

# ***********************************************************************
# 4. Show Current working directory.

echo -e "4. Current working directory: \c"; pwd

# *********************************************************************** 
# 5. Display date in the dd:mm:yy format.

echo -e "5. Date(dd:mm:yy): \c"; date +"%d:%m:%y"

# ***********************************************************************
# 6. Count the number of files in your home directory.

echo "6. Total number of files in '$DIR_PATH' is: $(find $DIR_PATH -maxdepth 1 -type f | wc -l)"

# ***********************************************************************
# 7. Create a file that lists all of the .C files in your directory.

echo "7. Listing all '.c' files in '$DIR_PATH':"
find "$DIR_PATH" -name "*.c" > "$DIR_PATH/c_files_list.txt"
echo -e "\tList of .c files saved to 'c_files_list.txt'"

# ***********************************************************************
# 8. Write a script for addition of two numbers for real numbers also.

echo "8. Enter two numbers for addition:"
echo -e "\t\c"; read -p "First number: " num1
echo -e "\t\c"; read -p "Second number: " num2
sum=$(echo "$num1 + $num2" | bc)
echo -e "\tThe sum of $num1 and $num2 is: $sum"

# ***********************************************************************
# 9. Write a script to convert string lower to upper and upper to lower from a file.

echo "9. Converting case of a string in a file:"
echo -e "\t\c"; read -p "Enter the file name or path (e.g., 'test.txt' './testing/test.txt'): " filename
if [ -f "$filename" ]; then
    mapfile -t lines < "$filename"
    
    # Print the file content before processing
    echo -e "\tFile content before translation: "
    for line in "${lines[@]}"; do
        echo -e "\t\t$line"
    done

    # Process each line (convert case)
    for i in "${!lines[@]}"; do
        lines[$i]=$(echo "${lines[$i]}" | tr 'a-zA-Z' 'A-Za-z')
    done

    # Write the modified lines back to the file
    printf "%s\n" "${lines[@]}" > "$filename"
    
    # Print the file content after processing
    echo -e "\tFile content after translation: "
    for line in "${lines[@]}"; do
        echo -e "\t\t$line"
    done
else
    echo "File not found!"
fi

# ***********************************************************************
# 10. Read ‘n’ and generate a pattern given below.
# 
# 1
# 1 2
# 1 2 3
# 1 2 3 4

echo "10. Enter the number for pattern generation:"
echo -e "\t\c"; read -p "Enter a number: " n
for ((i = 1; i <= n; i++)); do
    echo -e "\t\c";
    for ((j = 1; j <= i; j++)); do
        echo -n "$j "
    done
    echo ""
done

# ***********************************************************************
