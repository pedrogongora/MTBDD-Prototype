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

#ifndef BOOLEANMODEL_H_INCLUDED
#define BOOLEANMODEL_H_INCLUDED

#include <string>
#include <list>

#include "../arrayimpl/NodeTable.h"
#include "FormulaType.h"

using namespace std;


class Formula
{
public:
	virtual ~Formula() =0;
	virtual FormulaType getType() =0;
    virtual string* toString(void) =0;
};


class BooleanConstant : public Formula
{
private:
    bool value;
public:
    BooleanConstant(bool val);
    ~BooleanConstant();
    bool getValue(void);
    virtual FormulaType getType();
    virtual string* toString(void);
};


class Variable : public Formula
{
private:
    string* name;
    int varNum;
public:
    Variable(string* n);
    ~Variable();
    string* getName(void);
    int getVarNum(void);
    virtual FormulaType getType();
    virtual string* toString(void);
};


class Not : public Formula
{
private:
    Formula* subformula;
public:
    Not(Formula* subformula);
    ~Not();
    Formula* getSubformula(void);
    virtual FormulaType getType();
    virtual string* toString(void);
};


class And : public Formula
{
private:
    Formula* leftSubformula;
    Formula* rightSubformula;
public:
    And(Formula* leftSubformula, Formula* rightSubformula);
    ~And();
    Formula* getLeftSubformula(void);
    Formula* getRightSubformula(void);
    virtual FormulaType getType();
    virtual string* toString(void);
};


class Or : public Formula
{
private:
    Formula* leftSubformula;
    Formula* rightSubformula;
public:
    Or(Formula* leftSubformula, Formula* rightSubformula);
    ~Or();
    Formula* getLeftSubformula(void);
    Formula* getRightSubformula(void);
    virtual FormulaType getType();
    virtual string* toString(void);
};


class Implication : public Formula
{
private:
    Formula* leftSubformula;
    Formula* rightSubformula;
public:
    Implication(Formula* leftSubformula, Formula* rightSubformula);
    ~Implication();
    Formula* getLeftSubformula(void);
    Formula* getRightSubformula(void);
    virtual FormulaType getType();
    virtual string* toString(void);
};


class Iff : public Formula
{
private:
    Formula* leftSubformula;
    Formula* rightSubformula;
public:
    Iff(Formula* leftSubformula, Formula* rightSubformula);
    ~Iff();
    Formula* getLeftSubformula(void);
    Formula* getRightSubformula(void);
    virtual FormulaType getType();
    virtual string* toString(void);
};


class NextStateAssignment
{
private:
    Variable* variable;
    Formula* formula;
    list<double>* costs;
public:
    NextStateAssignment(Variable* v, Formula* f, list<double>* c);
    ~NextStateAssignment();
    Variable* getVariable(void);
    Formula* getFormula(void);
    list<double>* getCosts(void);
    string* toString(void);
};


class EquationsReader
{
private:
    int initialNodes;
    NodeTable* nodeTable;
    int agentsNum;
    TurnsFunction* turns;
    int maxVar;
    int adjacencyMatrix;
    int delta0;
    int equationNum;
    list<NextStateAssignment*>* equations;
    void readFile(string filename);
    void buildBDD(void);
    int getMaxVarFromEquations(void);
    void fillArray(double* array, double value);
    int formulaBdd(Formula* f);
    string getMatrixCellPath(char* s1, char* s2);
    string getVectorPath(void);
public:
    EquationsReader(string filename, int initialNodes);
    int getAdjacencyMatrix(void);
    int getAgentsNum(void);
    int getMaxVar(void);
    int getDelta0(void);
    int getEquationNum(void);
    NodeTable* getNodeTable(void);
    TurnsFunction* getTurns(void);
};


class BigIntCounter
{
private:
	int numBits;
	char* value; // binary representation using a null-terminated C-string
				 // (big-endian, lower bit is the most significant bit)
	bool overflow;
	int lubBit;
public:
	BigIntCounter(int numBits);
	~BigIntCounter();
	void increment(void);
	bool leqMax(void);
	char* getCStr(void);
};


class Equation2ArrayReader
{
private:
    int initialNodes;
    //NodeTable* nodeTable;
    int agentsNum;
    TurnsFunction* turns;
    int maxVar;
    double*** adjacencyMatrix;
    double** delta0;
    int equationNum;
    list<NextStateAssignment*>* equations;
    void readFile(string filename);
    void buildArray(void);
    void buildCell(int row, int col);
    void fillCellArrayWithList(double* cell, list<double>* values);
    int getMaxVarFromEquations(void);
    void fillCellArray(double* array, double value);
    void addToCell(int row, int col, double* values);
    void setCellValues(int row, int col, double* values);
    bool eval(Formula* f, int val);
public:
    Equation2ArrayReader(string filename);
    double*** getAdjacencyMatrix(void);
    int getAgentsNum(void);
    int getMaxVar(void);
    double** getDelta0(void);
    int getEquationNum(void);
    TurnsFunction* getTurns(void);
};


#endif // BOOLEANMODEL_H_INCLUDED
