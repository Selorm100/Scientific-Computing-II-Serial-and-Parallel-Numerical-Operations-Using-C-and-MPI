#!/bin/bash

# Define the output file name
OUTPUT_FILE="4171220_serial.txt"

# Compile the C files
gcc 4171220_main.c 4171220_vector.c 4171220_matrix.c  -o serr.x

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

# Run the compiled program with an initial matrix size argument (e.g., 20)
./serr.x 100

# Check if the program execution was successful
if [ $? -ne 0 ]; then
    echo "Program execution failed."
    exit 1
fi

# Create the Gnuplot script to plot the data


gnuplot << EOF
set terminal pngcairo enhanced size 800,600
set output 'performance_plot.png'
set title 'Performance Analysis'
set xlabel 'N'
set ylabel 'Time (seconds)'
set grid
set key left top
plot '$OUTPUT_FILE' using 1:2 with linespoints title 'Vector-Vector Time', \
     '$OUTPUT_FILE' using 1:3 with linespoints title 'Matrix-Vector Time', \
     '$OUTPUT_FILE' using 1:4 with linespoints title 'Matrix-Matrix Time'
EOF

 

# Check if Gnuplot ran successfully
if [ $? -ne 0 ]; then
    echo "Gnuplot execution failed."
    exit 1
fi
 
echo "Plot generated: performance_plot.png"
