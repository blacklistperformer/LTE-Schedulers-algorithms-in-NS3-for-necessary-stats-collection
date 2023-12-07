# Set the terminal to your preferred output format (e.g., png, pdf, svg)
set terminal pngcairo enhanced font 'Verdana,12'

# Set the output file name
set output 'ue_vs_throughput1.png'

# Set the title of the plot
set title 'UE vs Throughput'

# Set the labels for the X and Y axes
set xlabel 'UE'
set ylabel 'Throughput(kbps)'

# Plot the first data set as a line with points
plot 'throughput_cdf1_0.txt' using 1:2 with linespoints title 'Full Buffer Speed=0', \
     'throughput_cdf1_10.txt' using 1:2 with linespoints title 'Full Buffer Speed=10'

