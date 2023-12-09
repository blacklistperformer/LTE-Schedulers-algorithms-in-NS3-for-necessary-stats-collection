# Gnuplot script for plotting Instantaneous Throughput for UE 0

# Set the output file format and name
set terminal pngcairo enhanced font 'Verdana,12'
set output 'instantaneous_throughput_ue0_1_10.png'

# Set plot title and labels
set title 'Instantaneous Throughput for UE 0 (Speed: 10 m/s, Full Buffer)'
set xlabel 'Time (msec)'
set ylabel "Throughput (Kbps)"

# Set the legend
set key bottom right

# Plot Instantaneous Throughput data
plot "sinr_instantaneous_throughput1_10.txt" using 1:3 with lines title "Throughput"
