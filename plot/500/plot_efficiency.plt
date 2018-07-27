#! /usr/bin/gnuplot

set xlabel "Number of threads"
set ylabel "Efficiency"
set autoscale
plot "efficiency_THR_500.txt" title 'C++ threads active wait' with linespoints ls 1 dt 2 lc rgb "black", "efficiency_THRNORR_500.txt" title 'C++ threads single queue' with linespoints ls 2 dt 3 lc rgb "black", "efficiency_FF_500.txt" title 'FastFlow' with linespoints ls 4 dt 5 lc rgb "black", "efficiency_MODEL_500.txt" title 'Model' with linespoints ls 7 dt 1 lc rgb "black"
