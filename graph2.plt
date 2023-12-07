reset
set terminal pngcairo enhanced font 'Verdana,12'
set output 'Graph_2.png'

set title 'Average Aggregate System Throughput for Different Schedulers'
set xlabel 'Speed (m/s)'
set ylabel 'Average Aggregate System Throughput (Kbps)'

# Define the speed categories
speeds = "0 10"

# Define the scheduler categories
schedulers = "PF RR MT PSS"

# Set the style of boxes
set style data histograms
set style histogram clustered gap 1
set style fill solid border rgb "black"

# Loop through each scheduler and create a bar for each speed
do for [scheduler in schedulers] {
    plot for [speed in speeds] 'dat_graph2.txt' using (strcol(1) eq speed && strcol(3) eq scheduler ? $2 : 0):xtic(1) title columnhead
}

# Set the legend
set key top right
set key autotitle columnhead

# Show the plot
replot
