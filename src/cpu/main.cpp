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

>>>>>>> cd86c1b8a437d2634cfd8669383f2fb039a8dc59
	cout << "A) Completes" << endl;




	return 0;
}
