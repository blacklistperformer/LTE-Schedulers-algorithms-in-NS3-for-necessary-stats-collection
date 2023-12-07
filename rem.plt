# Set the terminal to your preferred output format (e.g., png, pdf, svg)
#set terminal pngcairo enhanced font 'Verdana,12'
#set view map;
#set term x11;

# Set the output file name
#set output 'SINR.png'

# Set the title of the plot
#set title 'SINR Radio Environment Map (REM)'

# Set the labels for the X and Y axes
#set xlabel 'X'
#set ylabel 'Y'
#set cblabel "SINR (dB)"

# Plot the first data set as a line with points
#plot 'plot "rem.out" using ($1):($2):(10*log10($4)) with image'


    set view map;
   set term x11;
   set title "SINR Radio Environment Map (REM)"
    set xlabel "X"
    set ylabel "Y"
    set cblabel "SINR (dB)"
   set terminal png
    set output "SINR.png"
    plot "rem.out" using ($1):($2):(10*log10($4)) with image
