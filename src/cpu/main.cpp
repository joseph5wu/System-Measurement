#include "benchmark.h"
using namespace std;

int main() {
	class CPUBenchmark benchmark;
	fstream file;

	cout << "A) CPU, Scheduling, and OS Services starts:" << endl;
	// benchmark.warmup();
<<<<<<< HEAD
	// benchmark.measurementOverhead(file);
	 benchmark.procedureCallOverhead(file);
	// benchmark.systemCallOverhead(file);
	//benchmark.taskCreationTime(file);
    benchmark.contextSwitchOverhead(file);
=======
	benchmark.measurementOverhead(file);
	benchmark.procedureCallOverhead(file);
	benchmark.systemCallOverhead(file);
	benchmark.taskCreationTime(file);
  benchmark.contextSwitchOverhead(file);

>>>>>>> c4aa9e58edd08dc9bb90754d129c2acd2f9de2da
	cout << "A) Completes" << endl;
	return 0;
}
