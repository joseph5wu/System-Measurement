all:
	g++ -O3 latency.cpp -o latency
	g++ -mavx -O3 bandwidth.cpp MemoryWriter.c -o bandwidth
	g++ -funroll-loops  MemoryWriter.c -mavx -O3 bandwidth.cpp    -o bandwidth_unroll
clean:
	rm -rf *o latency
	rm -rf *o bandwidth
