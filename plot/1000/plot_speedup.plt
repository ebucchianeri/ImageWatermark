#! /usr/bin/gnuplot

set xlabel "Number of threads"
set ylabel "Speedup"
set autoscale
plot "speedup_THR_1000.txt" title 'C++ threads, active wait' with linespoints ls 1 dt 2 lc rgb "black", "speedup_THRNORR_1000.txt" title 'C++ threads, single queue' with linespoints ls 2 dt 3 lc rgb "black", "speedup_FF_1000.txt" title 'FastFlow' with linespoints ls 4 dt 5 lc rgb "black", "speedup_MODEL_1000.txt" title 'Model' with linespoints ls 7 dt 1 lc rgb "black"
