#!/bin/bash
# Name: Chirag Chowdhury
# Roll: 002211001095
# 1.	Create a directory ‘All My Cprogs Files’ in your home directory. 
mkdir ~/'All My Cprogs Files'
# 2.	Move all the C files to the new creating directory.
find ~ -regex '.*\.c$' -exec cp {} 'All My Cprogs Files'/{} +
# 3.	Show the list of files in your current directory.
find -type f
# 4.	Show Current working directory.
pwd
# 5.	Display date in the dd:mm:yy format
date +%d:%m:%y
# 6.	Count the number of files in your home directory
find ~ -maxdepth 1 -type f | wc -l
# 7.	Create a file that lists all of the .C files in your directory.  
find . -regex '.*\.c$' > 'All My Cprogs Files.txt'
# 8.	Write a script for addition of two numbers for real numbers also.
echo -n "Enter two numbers to add: " # -n removes newline at the end
read var1 var2
echo "Sum is: $(("$var1" + "$var2"))"
# 9.	Write a script to convert string lower to upper and upper to lower from a file.
echo -n "Enter file name: "
read file
if [ ! -r "$file" ]; then
    echo "file $file does not exist"
    exit 1
fi
tmpFile=$(mktemp "$file""XXX")
tr '[:lower:][:upper:]' '[:upper:][:lower:]' < "$file" > "$tmpFile"
mv "$tmpFile" "$file"
# 10.	Read ‘n’ and generate a pattern given below.
# 1
# 1 2
# 1 2 3
# 1 2 3 4
echo -n "Enter value of 'n': "
read n
for ((counter=1; counter <= "$n"; counter++))
do
    seq -s ' ' 1 "$counter"
done