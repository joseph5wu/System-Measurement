all:
	g++ -O3 latency.cpp -o latency
	g++ -mavx -O3 bandwidth.cpp MemoryWriter.c -o bandwidth
	g++ -funroll-loops  MemoryWriter.c -mavx -O3 bandwidth.cpp -o bandwidth_unroll
	g++ page_fault.cpp -o page_fault
clean:
	rm -rf *o latency
	rm -rf *o bandwidth
	rm -rf *o bandwidth_unroll
	rm -rf *o page_fault
