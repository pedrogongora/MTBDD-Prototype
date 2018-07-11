
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>

#include "booleanmodel/BooleanModel.h"
#include "Stopwatch.h"

using namespace std;

void printMatrix(double*** matrix, int dim, int cellDim);
void printVector(double** matrix, int dim, int cellDim);
double*** dupMatrix(double*** matrix, int dim, int cellDim);
int bellmanFord(double*** adj, double** delta, int dim, int cellDim, TurnsFunction* turns, int distAg);

void processFileNonSymbolic(const string& filename) {
    Stopwatch swatch;
    cout << setw(79) << setfill('-') << "" << endl;
    cout << "Reading file: " << filename << endl;
    cout << setw(79) << setfill('-') << "" << endl << endl;
    swatch.start();
    Equation2ArrayReader eqnReader(filename);
    swatch.stop();
    cout << endl;
    cout    << "--> Building model matrix BDD took "
            << swatch.getLastIntervalSecs()
            << "s"
            << endl << endl;
    cout << "#eqn: " << eqnReader.getEquationNum() << endl;
    cout << "#ags: " << eqnReader.getAgentsNum() << endl;

    double*** adj = eqnReader.getAdjacencyMatrix();
    double** delta0 = eqnReader.getDelta0();
    int dim = 1 << eqnReader.getEquationNum();
    int cellDim = eqnReader.getAgentsNum();
    TurnsFunction* turns = eqnReader.getTurns();

    // run Bellman-Ford
    //double*** workingMatrix = dupMatrix(adj, dim, cellDim);
    swatch.start();
    int distinguishedIndex = 0;
    int its = bellmanFord(adj, delta0, dim, cellDim, turns, distinguishedIndex);
    swatch.stop();
    cout << endl;
    cout    << "--> Running Bellman-Ford took "
            << swatch.getLastIntervalSecs()
            << "s"
            << ", with "<<its<<" iterations."
            << endl << endl;

    cout    << "--> Total time "
            << swatch.getTotalSecs()
            << "s"
            << endl << endl;

    if ( eqnReader.getEquationNum() <= 4 ) {
        cout << "dim: " << dim << ", cellDim: " << cellDim << endl;

        cout << "adj:" << endl;
        printMatrix(adj, dim, cellDim);
        cout << endl << endl;

        cout << "delta:" << endl;
        printVector(delta0, dim, cellDim);
        cout << endl << endl;

        //cout << "result:" << endl;
        //table->printMatrix(result);
        //cout << endl << endl;

        //cout << "delta:" << endl;
        //table->printMatrix(delta);
        //cout << endl << endl;

        //cout << "profiles (SPM):" << endl;
        //table->printMatrix(profiles);
        cout << endl << endl;
    }
}

void cpArray(double* dst, double* src, int dim) {
    for (int i=0; i<dim; i++) {
        dst[i] = src[i];
    }
}
void fillArray(double* dst, double val, int dim) {
    for (int i=0; i<dim; i++) {
        dst[i] = val;
    }
}
void addArray(double* dst, double* values, int dim) {
    for (int i=0; i<dim; i++) {
        dst[i] += values[i];
    }
}
bool isArrayEqual(double* a1, double* a2, int dim) {
    bool r = true;
    for (int i=0; i<dim; i++) r = r && (a1[i] == a2[i]);
    return r;
}
bool triangle(double*** adj, double** delta, int dim, int cellDim, TurnsFunction* turns, int distAg, double* workingCell) {
    //double* workingCell = new double[cellDim];
    double sum;
    int turn;
    bool change = false;
    //bool changeFirst = false;
    //bool changeLater = false;
    for (int i=0; i<dim; i++) {
        fillArray(workingCell, numeric_limits<double>::infinity(), cellDim);
        turn = turns->getStateTurn(i);
        //changeFirst = false;
        for (int j=0; j<dim; j++) {
            sum = adj[i][j][turn] + delta[j][turn];
            if ( workingCell[turn] > sum ) {
                cpArray(workingCell, delta[j], cellDim);
                addArray(workingCell, adj[i][j], cellDim);
                //if (!changeFirst) changeFirst = true;
                //else changeLater = true;
            } else {
                sum = adj[i][j][distAg] + delta[j][distAg];
                if ( workingCell[distAg] > sum ) {
                    cpArray(workingCell, delta[j], cellDim);
                    addArray(workingCell, adj[i][j], cellDim);
                    //if (!changeFirst) changeFirst = true;
                    //else changeLater = true;
                }
            }
        }
        if (!isArrayEqual(delta[i], workingCell, cellDim)) {
            cpArray(delta[i], workingCell, cellDim);
            change = true;
        }
    }
    //delete [] workingCell;
    return change;
}
int bellmanFord(double*** adj, double** delta, int dim, int cellDim, TurnsFunction* turns, int distAg) {
    int its = 0;
    bool change = false;
    int max = dim*dim;
    double* workingCell = new double[cellDim];
    cout << "dim: " << dim << ", max: " << max << endl;
    do {
        change = triangle(adj, delta, dim, cellDim, turns, distAg, workingCell);
        its++;
        cout << "Iteration #" << its << ":" << endl;
        printVector(delta, dim, cellDim);
    } while (change && its < max);
    delete workingCell;
    return its;
}


double*** dupMatrix(double*** matrix, int dim, int cellDim) {
    double*** dup = new double**[dim];
    for (int i=0; i<dim; i++) {
        dup[i] = new double*[dim];
        for (int j=0; j<dim; j++) {
            dup[i][j] = new double[cellDim];
            for (int k=0; k<cellDim; k++) {
                dup[i][j][k] = matrix[i][j][k];
            }
        }
    }
    return dup;
}


void printMatrix(double*** matrix, int dim, int cellDim) {
    cout << setfill(' '); // << setprecision(2);
    cout << "." << endl;
    for (int row=0; row<dim; row++) {
        for (int col=0; col<dim; col++) {
            cout << "[";
            for (int i=0; i<cellDim; i++) {
                double val = matrix[row][col][i];
                cout << setw(3) << val;
                if (i < cellDim-1 ) cout << ",";
            }
            cout << "]\t";
        }
        cout << endl;
    }
}

void printVector(double** matrix, int dim, int cellDim) {
    cout << setfill(' '); // << setprecision(2);
    for (int row=0; row<dim; row++) {
        cout << "[";
        for (int i=0; i<cellDim; i++) {
            cout << setw(3) << matrix[row][i];
            if (i < cellDim-1 ) cout << ",";
        }
        cout << "]" << endl;
    }
}
