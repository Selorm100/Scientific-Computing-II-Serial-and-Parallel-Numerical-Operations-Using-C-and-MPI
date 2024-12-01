#!/bin/bash

# # Compile the MPI program
mpicc 4171220_main.c 4171220_vector.c 4171220_matrix.c -o par.x
 
# Vector size
vector_size=250000000

# Output files
time_output_file="4171220_scale.txt"
speedup_output_file="speedup_results.txt"

 
# Run the program for each number of processes
for np in {1..8}; do
    echo "Running with $np processes..."
    
    mpirun -np $np par.x $vector_size
done

# Calculate speedup and save to file
serial_time=$(awk 'NR==2 {print $3}' $time_output_file)
echo -e "N\tcore\tspeedup" > $speedup_output_file

awk -v st="$serial_time" 'NR>1 {printf "%d\t%d\t%.4f\n", $1, $2, st/$3}' $time_output_file >> $speedup_output_file


# Generate plots using gnuplot
gnuplot << EOF
set terminal png size 800,600
set output 'time_vs_cores.png'
set title "Time vs Number of Cores"
set xlabel "Number of Cores"
set ylabel "Time (seconds)"
set grid
plot "$time_output_file" using 2:3 with linespoints title "Execution Time"

set output 'speedup_vs_cores.png'
set title "Speedup vs Number of Cores"
set xlabel "Number of Cores"
set ylabel "Speedup"
set grid
plot "$speedup_output_file" using 2:3 with linespoints title "Speedup"
EOF
