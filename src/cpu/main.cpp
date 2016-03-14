#include "benchmark.h"
using namespace std;

int main() {
	class CPUBenchmark benchmark;
	fstream file;

	cout << "A) CPU, Scheduling, and OS Services starts:" << endl;
	// benchmark.warmup();
	//benchmark.measurementOverhead(file);
	benchmark.procedureCallOverhead(file);
	//benchmark.systemCallOverhead(file);
	//benchmark.taskCreationTime(file);
  //benchmark.contextSwitchOverhead(file);

	cout << "A) Completes" << endl;
	return 0;
}
