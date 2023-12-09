# Gnuplot script for plotting SINR and Instantaneous Throughput for UE 0

# Set the output file format and name
set terminal pngcairo enhanced font 'Verdana,12'
set output 'sinr_throughput_ue0_1_0.png'

# Set plot title and labels
set title 'SINR and Instantaneous Throughput for UE 0 (Speed: 0 m/s, Full Buffer)'
set xlabel 'Time (msec)'
set ylabel "SINR"
set y2label "Throughput (Kbps)"


# Set the legend
set key top left

# Plot SINR and Throughput data
plot "sinr_instantaneous_throughput1_10.txt" using 1:3 with lines title "Throughput" axes x1y2, \
     "sinr_instantaneous_throughput1_10.txt" using 1:2 with lines title "SINR"

