# Set the terminal to your preferred output format (e.g., png, pdf, svg)
set terminal pngcairo enhanced font 'Verdana,12'
set terminal png size 600, 600

# Set the output filename
set output 'graph6.png'

# Set the plot title, labels for the X and Y axes, and size
set title "Average Aggregate System Throughput for NON Full Buffer Scenario"
set xlabel "Scheduler Algorithm and Speed (m/s)"
set ylabel "Average Aggregate System Throughput"
set size 1.0, 0.95

#set the line and width
set boxwidth 0.5
set style fill solid

plot 'dat_graph6.txt' using 1:3:2:xtic(4) with boxes lc variable notitle, \
	'' using 1:3:(sprintf("%.1f", $3)) with labels offset 0,1 notitle
