#include "benchmark.h"
using namespace std;

int main() {
	class CPUBenchmark benchmark;
	fstream file;

	cout << "A) CPU, Scheduling, and OS Services starts:" << endl;
	benchmark.measurementOverhead(file);

	cout << "A) Completes" << endl;
	return 0;
}
