#include "kNN.hpp"
#include <time.h>
void tc1126()
{
    int nRows, nCols;
    Dataset dataset;
    dataset.loadFromCSV("mnist.csv");
    Dataset dataset2 = dataset;
    bool result = dataset2.drop(1, 0, "28x28");
    dataset2.printTail();
    cout << "Drop(1, \"28x28\") result: " << result << endl;
}
int main() {
    clock_t s, e;
    s = clock();

    tc1126();


    e = clock();
    double abc = double(e - s) / CLOCKS_PER_SEC;
    cout << "\n run time: " << abc << endl;
    
    return 0;
}