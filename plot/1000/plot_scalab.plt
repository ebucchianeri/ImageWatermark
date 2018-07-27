#! /usr/bin/gnuplot

set xlabel "Number of threads"
set ylabel "Scalability"
set autoscale
plot "scalab_THR_1000.txt" title 'C++ threads with active wait' with linespoints ls 1 dt 2 lc rgb "black", "scalab_THRNORR_1000.txt" title 'C++ threads single queue' with linespoints ls 2 dt 3 lc rgb "black", "scalab_FF_1000.txt" title 'FastFlow' with linespoints ls 4 dt 5 lc rgb "black", "scalab_MODEL_1000.txt" title 'Model' with linespoints ls 7 dt 1 lc rgb "black"
