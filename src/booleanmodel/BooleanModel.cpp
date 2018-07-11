/*  **************************************************************************
 *
 *  Creating MTBDDs from boolean equations.
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
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <stdexcept>

#include "BooleanModel.h"


#include "parser/BooleanModelParser.h"
#include "parser/BooleanModelParserTokenManager.h"
#include "parser/CharStream.h"



Formula::~Formula()
{
	// nothing to delete
}

BooleanConstant::BooleanConstant(bool val)
{
    value = val;
}

BooleanConstant::~BooleanConstant()
{
	// nothing to delete
}

bool BooleanConstant::getValue()
{
    return value;
}


FormulaType BooleanConstant::getType()
{
	return BOOLEAN_CONSTANT_TYPE;
}

string* BooleanConstant::toString(void)
{
    return (value ? new string("true") : new string("false") );
}

Variable::Variable(string* n)
{
    name = n;
    stringstream ss(n->substr(1));
    ss >> varNum;
}

Variable::~Variable()
{
    delete name;
}

string* Variable::getName(void)
{
    return name;
}

int Variable::getVarNum(void)
{
    return varNum;
}

FormulaType Variable::getType()
{
	return VARIABLE_TYPE;
}

string* Variable::toString(void)
{
    return name;
}

Not::Not(Formula* subformula)
{
    (*this).subformula = subformula;
}

Not::~Not()
{
    delete subformula;
}

Formula* Not::getSubformula(void)
{
    return subformula;
}

FormulaType Not::getType()
{
	return NOT_TYPE;
}

string* Not::toString(void)
{
    stringstream ss;
    ss << "~(" << *(subformula->toString()) << ")";
    return new string(ss.str());
}

And::And(Formula* leftSubformula, Formula* rightSubformula)
{
    this->leftSubformula = leftSubformula;
    this->rightSubformula = rightSubformula;
}

And::~And()
{
    delete leftSubformula;
    delete rightSubformula;
}

Formula* And::getLeftSubformula(void)
{
    return leftSubformula;
}

Formula* And::getRightSubformula(void)
{
    return rightSubformula;
}

FormulaType And::getType()
{
	return AND_TYPE;
}

string* And::toString(void)
{
    stringstream ss;
    ss  << "("
        << *(leftSubformula->toString())
        << " & "
        << *(rightSubformula->toString())
        << ")";
    return new string(ss.str());
}

Or::Or(Formula* leftSubformula, Formula* rightSubformula)
{
    this->leftSubformula = leftSubformula;
    this->rightSubformula = rightSubformula;
}

Or::~Or()
{
    delete leftSubformula;
    delete rightSubformula;
}

Formula* Or::getLeftSubformula(void)
{
    return leftSubformula;
}

Formula* Or::getRightSubformula(void)
{
    return rightSubformula;
}

FormulaType Or::getType()
{
	return OR_TYPE;
}

string* Or::toString(void)
{
    stringstream ss;
    ss  << "("
        << *(leftSubformula->toString())
        << " | "
        << *(rightSubformula->toString())
        << ")";
    return new string(ss.str());
}

Implication::Implication(Formula* leftSubformula, Formula* rightSubformula)
{
    this->leftSubformula  = leftSubformula;
    this->rightSubformula  = rightSubformula;
}

Implication::~Implication()
{
    delete leftSubformula;
    delete rightSubformula;
}

Formula* Implication::getLeftSubformula(void)
{
    return leftSubformula;
}

Formula* Implication::getRightSubformula(void)
{
    return rightSubformula;
}

FormulaType Implication::getType()
{
	return IMPLICATION_TYPE;
}

string* Implication::toString(void)
{
    stringstream ss;
    ss  << "("
        << *(leftSubformula->toString())
        << " -> "
        << *(rightSubformula->toString())
        << ")";
    return new string(ss.str());
}

Iff::Iff(Formula* leftSubformula, Formula* rightSubformula)
{
    this-> leftSubformula = leftSubformula;
    this-> rightSubformula = rightSubformula;
}

Iff::~Iff()
{
    delete leftSubformula;
    delete rightSubformula;
}

Formula* Iff::getLeftSubformula(void)
{
    return leftSubformula;
}

Formula* Iff::getRightSubformula(void)
{
    return rightSubformula;
}

FormulaType Iff::getType()
{
	return IFF_TYPE;
}

string* Iff::toString(void)
{
    stringstream ss;
    ss  << "("
        << *(leftSubformula->toString())
        << " = "
        << *(rightSubformula->toString())
        << ")";
    return new string(ss.str());
}

NextStateAssignment::NextStateAssignment(Variable* v, Formula* f, list<double>* c)
{
    variable = v;
    formula = f;
    costs = c;
}

NextStateAssignment::~NextStateAssignment()
{
    delete variable;
    delete formula;
    delete costs;
}

Variable* NextStateAssignment::getVariable(void)
{
    return variable;
}

Formula* NextStateAssignment::getFormula(void)
{
    return formula;
}

list<double>* NextStateAssignment::getCosts(void)
{
    return costs;
}

string* NextStateAssignment::toString(void)
{
    stringstream ss;
    ss  << *(variable->toString())
        << " := "
        << *(formula->toString())
        << " : [";
    int size = costs->size();
    for (list<double>::iterator it=costs->begin(); it!=costs->end(); it++) {
        ss << *it;
        if (--size != 0) ss <<", ";
    }
    ss << "];";
    return new string(ss.str());
}



EquationsReader::EquationsReader(string filename, int initialNodes)
{
	this->initialNodes = initialNodes;
	readFile(filename);
	buildBDD();
}
int EquationsReader::getAdjacencyMatrix(void)
{
    return adjacencyMatrix;
}
int EquationsReader::getAgentsNum(void)
{
    return agentsNum;
}
int EquationsReader::getDelta0(void)
{
    return delta0;
}
int EquationsReader::getEquationNum(void)
{
    return equationNum;
}
int EquationsReader::getMaxVar(void)
{
    return maxVar;
}
NodeTable* EquationsReader::getNodeTable(void)
{
    return nodeTable;
}
TurnsFunction* EquationsReader::getTurns(void)
{
    return turns;
}

void EquationsReader::readFile(string filename)
{
	//cout << "reading file: " << filename << endl;
	fstream in(filename.c_str(), ios::in);
	if ( in.fail() ) {
	    throw runtime_error("Cannot open file");
	}
	stringstream ss;
	string line;
	while ( !in.eof() ) {
		getline(in, line);
		ss << line << endl;
		//cout << "reading line: " << line << endl;
	}
	in.close();
	string file_contents = ss.str();
	//cout << "file readed: " << file_contents << endl;

	basic_string<char> basicstr(file_contents);
	CharStream* cs = new CharStream(basicstr, 0, 0);
	TokenManager* tm = new BooleanModelParserTokenManager(cs);
	BooleanModelParser parser(tm);
	this->equations = parser.equation_list();

	//list<NextStateAssignment*>::iterator it;
	//NextStateAssignment* eqn;
	//for ( it=equations->begin(); it != equations->end(); it++ ) {
	//	eqn = *it;
	//	cout << "parsed equation: " << *eqn->toString() << endl;
	//}

	//delete cs;
	//delete tm; // tm deletes cs, and parser deletes tm
}

void EquationsReader::buildBDD(void)
{
    agentsNum = (*(equations->begin()))->getCosts()->size();
    equationNum= equations->size();
    // init turns function
    turns = new ModTurnsFunction(agentsNum, equationNum);
    // initialize table
    //turns = new ModTurnFunction(agentsNum);
    nodeTable = new NodeTable(initialNodes, turns, agentsNum);
    maxVar = getMaxVarFromEquations();
    nodeTable->addVars(maxVar*2);

    // process equations
    int varNum, bdd, mtbdd, tmp, tmp2;
    double* infty = new double[agentsNum];
    double* costs;
    fillArray(infty, numeric_limits<double>::infinity());
    adjacencyMatrix = nodeTable->getTerminalNode(infty);

	list<NextStateAssignment*>::iterator it;
	NextStateAssignment* eqn;
	for ( it=equations->begin(); it != equations->end(); it++ ) {
		eqn = *it;
		cout << "processing equation: " << *eqn->toString() << endl;
        varNum = eqn->getVariable()->getVarNum();
        costs = new double[agentsNum];
        list<double>::iterator costsIt = eqn->getCosts()->begin();
        for (int i=0; i<agentsNum; i++,costsIt++) costs[i] = *costsIt;

        tmp = formulaBdd(eqn->getFormula());
        nodeTable->ref(tmp);
        tmp2 = nodeTable->getVarNode(varNum*2, 0, 1);
        bdd = nodeTable->apply(tmp, tmp2, AND_OP);
        nodeTable->deref(tmp);
        nodeTable->ref(adjacencyMatrix);
        nodeTable->ref(bdd);
        int zeroMTBDD = nodeTable->getTerminalNode(infty);
        int oneMTBDD = nodeTable->getTerminalNode(costs);
        mtbdd = nodeTable->bddToMtbdd(bdd, zeroMTBDD, oneMTBDD);
        nodeTable->deref(bdd);
        nodeTable->ref(mtbdd);
        tmp = nodeTable->apply(adjacencyMatrix, mtbdd, ACCUMULATE_OP);
        nodeTable->deref(adjacencyMatrix);
        nodeTable->deref(mtbdd);
        adjacencyMatrix = tmp;
        nodeTable->ref(adjacencyMatrix);

        delete [] costs;
	}

    // set diagonal
	BigIntCounter state(this->maxVar);
	string path;
	int previousBDD, pathBDD, nextBDD;
	double* zeroes = new double[agentsNum];
	fillArray(zeroes, 0.0);

    int diagBDD = nodeTable->getTerminalNode(zeroes);
    int fillBDD = nodeTable->getTerminalNode(infty);
	previousBDD = nodeTable->diagonalMatrix(diagBDD, fillBDD);

	nodeTable->ref(adjacencyMatrix);
	nodeTable->ref(previousBDD);
	nextBDD = nodeTable->apply(adjacencyMatrix, previousBDD, ALL_MIN_OP);
	nodeTable->deref(adjacencyMatrix);
	nodeTable->deref(previousBDD);
	adjacencyMatrix = nextBDD;
	nodeTable->ref(adjacencyMatrix);

    // compute first approximation vector (to the last state) (delta0)
    previousBDD = nodeTable->getTerminalNode(infty);
    path = getVectorPath();
    pathBDD = nodeTable->partialPathBasedTree(path, zeroes, infty);
    nodeTable->ref(pathBDD);
    delta0 = nodeTable->apply(previousBDD, pathBDD, ALL_MIN_OP);
    nodeTable->deref(pathBDD);
    nodeTable->deref(adjacencyMatrix); // it's caller responsibility to ref these BDDs

    delete [] infty;
    delete [] zeroes;
}

int EquationsReader::getMaxVarFromEquations(void)
{
	int max = 0;
	int tmp;
	list<NextStateAssignment*>::iterator it = equations->begin();
	while ( it != equations->end() ) {
		tmp = (*it)->getVariable()->getVarNum();
		max = (max > tmp ? max : tmp);
		it++;
	}
	return max;
}

void EquationsReader::fillArray(double* array, double value)
{
	for (int i=0; i<agentsNum; i++) {
		array[i] = value;
	}
}

int EquationsReader::formulaBdd(Formula* f)
{
	int result = -1;

	if (f->getType() == BOOLEAN_CONSTANT_TYPE) {
		BooleanConstant* tf = (BooleanConstant*)f;
		return ( (tf->getValue()) ? 1 : 0 );
	} else if (f->getType() == NOT_TYPE) {
		Not* tf = (Not*)f;
		int tmp = formulaBdd(tf->getSubformula());
		nodeTable->ref(tmp);
		int res = nodeTable->bdd_not(tmp);
		nodeTable->deref(tmp);
		return res;
	} else if (f->getType() == VARIABLE_TYPE) {
		Variable* tf = (Variable*)f;
		int varNum = tf->getVarNum();
		int res = nodeTable->getVarNode(varNum*2-1, 0, 1);
		return res;
	} else if (f->getType() == AND_TYPE) {
		And* tf = (And*)f;
		Formula* f1 = tf->getLeftSubformula();
		Formula* f2 = tf->getRightSubformula();
		int bdd1 = formulaBdd(f1);
		nodeTable->ref(bdd1);
		int bdd2 = formulaBdd(f2);
		nodeTable->deref(bdd1);
		return nodeTable->apply(bdd1, bdd2, AND_OP);
	} else if (f->getType() == OR_TYPE) {
		Or* tf = (Or*)f;
		Formula* f1 = tf->getLeftSubformula();
		Formula* f2 = tf->getRightSubformula();
		int bdd1 = formulaBdd(f1);
		nodeTable->ref(bdd1);
		int bdd2 = formulaBdd(f2);
		nodeTable->deref(bdd1);
		return nodeTable->apply(bdd1, bdd2, OR_OP);
	} else if (f->getType() == IMPLICATION_TYPE) {
		Implication* tf = (Implication*)f;
		Formula* f1 = tf->getLeftSubformula();
		Formula* f2 = tf->getRightSubformula();
		int bdd1 = formulaBdd(f1);
		nodeTable->ref(bdd1);
		int bdd2 = formulaBdd(f2);
		nodeTable->deref(bdd1);
		return nodeTable->apply(bdd1, bdd2, IMP_OP);
	} else if (f->getType() == IFF_TYPE) {
		Iff* tf = (Iff*)f;
		Formula* f1 = tf->getLeftSubformula();
		Formula* f2 = tf->getRightSubformula();
		int bdd1 = formulaBdd(f1);
		nodeTable->ref(bdd1);
		int bdd2 = formulaBdd(f2);
		nodeTable->deref(bdd1);
		return nodeTable->apply(bdd1, bdd2, IFF_OP);
	}

	return result;
}

string EquationsReader::getMatrixCellPath(char* s1, char* s2)
{
	stringstream sb;
    for (int i=0; i<maxVar; i++) {
        sb << s1[i];
        sb << s2[i];
    }
	return sb.str();
}

string EquationsReader::getVectorPath(void)
{
	stringstream sb;
    for (int i=0; i<maxVar; i++) {
        sb << "*1";
    }
	return sb.str();
}

BigIntCounter::BigIntCounter(int numBits)
{
	this->numBits = numBits;
	this->value = new char[numBits + 1];
	for (int i=0; i<numBits; i++) value[i] = '0';
	value[numBits] = '\0';
	lubBit = numBits;
	this->overflow = false;
}

BigIntCounter::~BigIntCounter()
{
	delete [] value;
}

void BigIntCounter::increment(void)
{
	int ii = numBits - 1;

	if ( !overflow ) {
		if ( value[ii] == '0' ) {
			value[ii] = '1'; // no carry
		} else {
			// add carry
			while ( ii >= 0 && value[ii] == '1' ) {
				value[ii] = '0'; // 1+1 = 0
				ii--;
			}
			if ( ii >= 0 ) {
				value[ii] = '1';
				if ( lubBit > ii ) {
					lubBit = ii;
					cout << "counter increased 1 bit: " << value << endl;
				}
			}
			else overflow = true;
		}
	}
}

bool BigIntCounter::leqMax(void)
{
	return !overflow;
}

char* BigIntCounter::getCStr(void)
{
	return value;
}


Equation2ArrayReader::Equation2ArrayReader(string filename)
{
    readFile(filename);
    maxVar = getMaxVarFromEquations();
    buildArray();
    turns = new ModTurnsFunction(agentsNum, equationNum);
}

void Equation2ArrayReader::readFile(string filename)
{
    //cout << "reading file: " << filename << endl;
    fstream in(filename.c_str(), ios::in);
    if ( in.fail() ) {
        throw runtime_error("Cannot open file");
    }
    stringstream ss;
    string line;
    while ( !in.eof() ) {
        getline(in, line);
        ss << line << endl;
        //cout << "reading line: " << line << endl;
    }
    in.close();
    string file_contents = ss.str();
    //cout << "file readed: " << file_contents << endl;

    basic_string<char> basicstr(file_contents);
    CharStream* cs = new CharStream(basicstr, 0, 0);
    TokenManager* tm = new BooleanModelParserTokenManager(cs);
    BooleanModelParser parser(tm);
    this->equations = parser.equation_list();
    this->equationNum = this->equations->size();
    this->agentsNum = (*(equations->begin()))->getCosts()->size();

    list<NextStateAssignment*>::iterator it;
    NextStateAssignment* eqn;
    for ( it=equations->begin(); it != equations->end(); it++ ) {
      eqn = *it;
      cout << "parsed equation: " << *eqn->toString() << endl;
    }
}

void Equation2ArrayReader::buildArray(void)
{
    int matrixDim = 1 << maxVar;
    int nodeDim = agentsNum;

    // build adjacency matrix
    adjacencyMatrix = new double**[matrixDim];
    for (int ii=0; ii<matrixDim; ii++) {
        adjacencyMatrix[ii] = new double*[matrixDim];
        for (int jj=0; jj<matrixDim; jj++) {
            adjacencyMatrix[ii][jj] = new double[nodeDim];
            buildCell(ii, jj);
        }
    }

    // build delta0
    delta0 = new double*[matrixDim];
    for (int i=0; i<matrixDim; i++) {
        delta0[i] = new double[nodeDim];
        fillCellArray(delta0[i], numeric_limits<double>::infinity());
    }
    fillCellArray(delta0[matrixDim-1], 0.0);
}

void Equation2ArrayReader::buildCell(int row, int col) {
    if (row == col) {
        fillCellArray(adjacencyMatrix[row][col], 0.0);
        return;
    }
    fillCellArray(adjacencyMatrix[row][col], numeric_limits<double>::infinity());

    list<NextStateAssignment*>::iterator it = equations->begin();
    bool match_row, match_col;
    bool modified = false;
    double* tmp = new double[agentsNum];
    list<double>* costs;
    while ( it != equations->end() ) {
        match_col = eval((*it)->getVariable(), col);
        match_row = eval((*it)->getFormula(),  row);
        if (match_row && match_col) {
            costs = (*it)->getCosts();
            if (modified) {
                //fillCellArray(adjacencyMatrix[row][col], 0.0);
                fillCellArrayWithList(tmp, costs);
                addToCell(row, col, tmp);
            } else {
                fillCellArrayWithList(adjacencyMatrix[row][col], costs);
            }
            modified = true;
        }
        it++;
    }
    delete tmp;
}

void Equation2ArrayReader::fillCellArrayWithList(double* cell, list<double>* values) {
    list<double>::iterator it = values->begin();
    int i = 0;
    while ( it != values->end() ) {
        cell[i] = (*it);
        i++;
        it++;
    }
}

int Equation2ArrayReader::getMaxVarFromEquations(void)
{
    int max = 0;
    int tmp;
    list<NextStateAssignment*>::iterator it = equations->begin();
    while ( it != equations->end() ) {
        tmp = (*it)->getVariable()->getVarNum();
        max = (max > tmp ? max : tmp);
        it++;
    }
    return max;
}

void Equation2ArrayReader::fillCellArray(double* array, double value)
{
    for (int i=0; i<agentsNum; i++) {
        array[i] = value;
    }
}

void Equation2ArrayReader::addToCell(int row, int col, double* values)
{
    for (int i=0; i<agentsNum; i++) {
        adjacencyMatrix[row][col][i] += values[i];
    }
}

void Equation2ArrayReader::setCellValues(int row, int col, double* values)
{
    for (int i=0; i<agentsNum; i++) {
        adjacencyMatrix[row][col][i] = values[i];
    }
}

bool Equation2ArrayReader::eval(Formula* f, int val) {
    int type = f->getType();
    if (type == BOOLEAN_CONSTANT_TYPE) {
        BooleanConstant* tf = (BooleanConstant*)f;
        return tf->getValue();
    } else if (type == NOT_TYPE) {
        Not* tf = (Not*)f;
        return !eval(tf->getSubformula(),val);
    } else if (type == VARIABLE_TYPE) {
        Variable* tf = (Variable*)f;
        int varNum = tf->getVarNum();
        int mask = 1 << (maxVar - varNum);
        return val & mask;
    } else if (type == AND_TYPE) {
        And* tf = (And*)f;;
        return eval(tf->getLeftSubformula(),val) && eval(tf->getRightSubformula(),val);
    } else if (type == OR_TYPE) {
        Or* tf = (Or*)f;
        return eval(tf->getLeftSubformula(),val) || eval(tf->getRightSubformula(),val);
    } else if (type == IMPLICATION_TYPE) {
        Implication* tf = (Implication*)f;
        return !eval(tf->getLeftSubformula(),val) || eval(tf->getRightSubformula(),val);
    } else if (type == IFF_TYPE) {
        Iff* tf = (Iff*)f;
        return  (!eval(tf->getLeftSubformula(),val)  || eval(tf->getRightSubformula(),val)) &&
                (!eval(tf->getRightSubformula(),val) || eval(tf->getLeftSubformula(),val));
    }
    return false;
}

double*** Equation2ArrayReader::getAdjacencyMatrix() {
    return adjacencyMatrix;
}

int Equation2ArrayReader::getAgentsNum() {
    return agentsNum;
}

int Equation2ArrayReader::getMaxVar() {
    return maxVar;
}

double** Equation2ArrayReader::getDelta0() {
    return delta0;
}

int Equation2ArrayReader::getEquationNum() {
    return equationNum;
}

TurnsFunction* Equation2ArrayReader::getTurns() {
    return turns;
}
