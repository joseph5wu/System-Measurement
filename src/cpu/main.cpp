#include "benchmark.h"
using namespace std;

int main() {
	class CPUBenchmark benchmark;
	fstream file;

	cout << "A) CPU, Scheduling, and OS Services starts:" << endl;
	//benchmark.warmup();
	benchmark.measurementOverhead(file);

	cout << "A) Completes" << endl;

	cout << "B) Procedure call overhead " << endl;
	//benchmark.warmup();
	benchmark.procedureCallOverhead(file);

	cout << "B) Completes" << endl;




	return 0;
}
