# Set the terminal to your preferred output format (e.g., png, pdf, svg)
set terminal pngcairo enhanced font 'Verdana,12'

# Set the output file name
set output 'sinr_vs_tie_1.png'

# Set the title of the plot
set title 'SINR vs TIME'

# Set the labels for the X and Y axes
set xlabel 'TIME(milli second)'
set ylabel 'SINR(dBm)'

# Plot the first data set as a line with points
plot 'sinr_data1_0.txt' using 1:2 with linespoints title 'Full Buffer Speed=0', \
     'sinr_data1_10.txt' using 1:2 with linespoints title 'Full Buffer Speed=10'

