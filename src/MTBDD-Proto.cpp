/*  **************************************************************************
 *
 *  Running symbolic Bellman-Ford algorithm
 *
 *  Author: Pedro A. Góngora <pedro.gongora@gmail.com>
 *
 *  Copyright (C) 2012 Pedro A. Góngora
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *  *************************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>

#include "booleanmodel/BooleanModel.h"
#include "arrayimpl/NodeTable.h"
#include "arrayimpl/NodeCache.h"
#include "Stopwatch.h"

using namespace std;

void processFile(const string& filename);
void processFileNonSymbolic(const string& filename);

int main(int argc, const char* argv[]) {

    if ( argc < 2 ) {
        cout << "Usage: "
                << endl
                << "\t./MTBDD-Proto [-ns] model_filename"
                << endl
                << endl
                << "OPTIONS:"
                << endl
                << "\t-ns Run test using non-symbolic algorithm"
                << endl
                << endl;
        return 1;
    }

    //string filename = "/home/pedrogl/progs/java/MTBDD-Prototype/models/n012_k006_a003.eqn";
    //string filename = "/home/pedrogl/Ubuntu One/workspaceCDT/MTBDD-Proto/models/test/test.txt";
    //processFileNonSymbolic(filename);

    if (argc == 2) {
        string filename(argv[1]);
        try {
            processFile(filename);
        } catch (exception& e) {
            cout << "Exception: " << e.what() << endl;
        }
    } else {
        string filename(argv[2]);

        try {
            processFileNonSymbolic(filename);
        } catch (exception& e) {
            cout << "Exception: " << e.what() << endl;
        }
    }

    return 0;
}

void processFile(const string& filename) {
    Stopwatch swatch;
    cout << setw(79) << setfill('-') << "" << endl;
	cout << "Reading file: " << filename << endl;
    cout << setw(79) << setfill('-') << "" << endl << endl;
	swatch.start();
	EquationsReader eqnReader(filename, 20000000);
	swatch.stop();
	cout << endl;
	cout    << "--> Building model matrix BDD took "
	        << swatch.getLastIntervalSecs()
	        << "s"
	        << endl << endl;

	NodeTable* table = eqnReader.getNodeTable();
	int adj = eqnReader.getAdjacencyMatrix();
	int delta0 = eqnReader.getDelta0();
    table->ref(adj);
    table->ref(delta0);

    int delta = delta0;
    int prev, tmp;

    // run Bellman-Ford
    swatch.start();
    BigIntCounter iterations(eqnReader.getEquationNum());
    int distinguishedIndex = 0;
    unsigned long its = 0;
    int pilones=1;
    do {
        prev = delta;
        table->ref(prev);
        tmp = table->triangleSingleDest(adj, prev, distinguishedIndex);
        table->deref(prev);
        table->ref(tmp);
        delta = table->transposeVector(tmp);
        table->deref(tmp);
        if ( iterations.leqMax() ) iterations.increment();
        else pilones--;
        its++;
        cout << "It: "<<its <<": "<<endl;
        //table->printMatrix(delta);
    } while ( prev != delta && (iterations.leqMax() || pilones > 0) );
    //    if (its == 10000) break;
    //} while ( prev != delta );
    table->ref(delta);
    table->ref(prev);
    int result = table->transposeVector(delta);
    table->ref(result);
    swatch.stop();
    cout << endl;
    cout    << "--> Running Bellman-Ford took "
            << swatch.getLastIntervalSecs()
            << "s"
            << endl << endl;

    // compute trajectories (strategy profiles)
    swatch.start();
    //BYTE* mask = new BYTE[eqnReader.getEquationNum()];
    //for (int i=0; i<eqnReader.getEquationNum(); i++) mask[i] = 0;
    unsigned long mask = 0;
    int currentVar = 1;
    int distinguishedIdx = 0;
    tmp = table->apply(adj, delta, PLUS_OP);
    swatch.stop();
    cout    << "--> Computing strategy profiles (step 1/2) took "
            << swatch.getLastIntervalSecs()
            << "s"
            << endl << endl;
    swatch.start();
    int profiles = table->applyRelative(
            tmp,
            result,
            mask,
            currentVar,
            distinguishedIdx,
            REL_BELOW_THRESHOLD_OP);
    table->ref(profiles);
    swatch.stop();
    cout    << "--> Computing strategy profiles (step 2/2) took "
            << swatch.getLastIntervalSecs()
            << "s"
            << endl << endl;

    cout    << "adj: " << adj
            << ",delta0: " << delta0
            << endl;
    cout    << "prev: "         << prev
            << ", delta: "      << delta
            << ", result: "     << result
            << ", profiles: "   << profiles
            << ", its: "        << its
            << endl << endl;

    cout    << "--> Total time "
            << swatch.getTotalSecs()
            << "s"
            << endl << endl;

    if ( eqnReader.getEquationNum() <= 4 ) {
        cout << "adj:" << endl;
        table->printMatrix(adj);
        cout << endl << endl;

        cout << "delta0:" << endl;
        table->printMatrix(delta0);
        cout << endl << endl;

        cout << "result:" << endl;
        table->printMatrix(result);
        cout << endl << endl;

        //cout << "prev:" << endl;
        //table->printMatrix(prev);
        //cout << endl << endl;

        //cout << "delta:" << endl;
        //table->printMatrix(delta);
        //cout << endl << endl;

        cout << "profiles (SPM):" << endl;
        table->printMatrix(profiles);
        cout << endl << endl;

        table->garbageCollect();
        table->printTerminalsTable();
    }
}

int main2() {

	NodeTable table(2);

	double d0[] = {0,0};
	double d1[] = {1,1};
	int t0 = table.getTerminalNode(d0);
	int t1 = table.getTerminalNode(d1);

	table.addVars(4096);
	int diag = table.diagonalMatrix(t1,t0);
	cout << "diag: " << diag << endl;
	table.garbageCollect();
	table.printMatrix(diag);
	table.printTerminalsTable();

	return 0;
}
