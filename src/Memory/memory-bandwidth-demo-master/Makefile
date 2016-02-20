memory_profiler: main.c monotonic_timer.c functions.c
	gcc -O3 -march=native $^ -o $@ 

.PHONY: run
run: memory_profiler
	./memory_profiler
