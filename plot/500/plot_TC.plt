#! /usr/bin/gnuplot

set xlabel "Number of threads"
set ylabel "Completation time (ms)"
set autoscale
plot "completion_time_THR_500.txt" title 'C++ threads, active wait' with linespoints ls 1 dt 2 lc rgb "black", "completion_time_THRNORR_500.txt" title 'C++ threads, single queue' with linespoints ls 2 dt 3 lc rgb "black", "completion_time_FF_500.txt" title 'FastFlow' with linespoints ls 4 dt 5 lc rgb "black", "completion_time_MODEL_500.txt" title 'Model' with linespoints ls 7 dt 1 lc rgb "black"
