/*  **************************************************************************
 *
 *  MTBDD node table implementation with mark-and-sweep garbage collection.
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

#ifndef NODETABLE_H_INCLUDED
#define NODETABLE_H_INCLUDED

#include <map>
#include "NodeCache.h"
#include "ApplyOps.h"

using namespace std;

//typedef char BYTE;

class TurnsFunction
{
public:
	virtual 	    ~TurnsFunction() =0;
    virtual int     getStateTurn(unsigned long state) =0;
};

class ModTurnsFunction : public TurnsFunction
{
private:
    int         agentNum;
    int         bitLength;
public:
    ModTurnsFunction(int agentNum, int bitLength);
    ~ModTurnsFunction();
    virtual int  getStateTurn(unsigned long state);
};

class NodeTable
{
    const static int DEFAULT_NODES      = 10000;
    const static int MINIMUM_NODES      = 100;

    const static int TABLE_WIDTH        = 3;
    const static int VAR_OFFSET         = 0;
    const static int LO_OFFSET          = 1;
    const static int HI_OFFSET          = 2;

    const static int LIST_WIDTH         = 2;
    const static int LEFT_OFFSET        = 0;
    const static int RIGHT_OFFSET       = 1;

    const static short MAX_REFERENCES   = 0x3FFF;
    const static short NODE_MASK        = 0x4000;
    const static short NODE_UNMASK      = 0x3FFF;

    int nodeNum;
    int* nodeTable;
    int* nodeList;
    short* referenceCount;
    int firstFreeNode;
    int lastUsedNode;
    int freeNodes;
    int varNum;
    int terminalValueSize;
    NodeCache* nodeCache;
    map<int, double*> terminals;
    int nextTerminalKey;
    TurnsFunction* turns;
    int gcNum;

    double* workArray;
    double* workZeroes;
    double* workInftys;
    int zeroesTerminal;
    int inftysTerminal;

private:
    void initialize(void);

    // node low-level access
    void    setNodeAttributes(int node, int var, int lo, int hi);
    int     newNode(int var, int lo, int hi);

    // list low-level access
    void    setListPointers(int node, int left, int right);
    int     getListLeft(int node);
    int     getListRight(int node);
    void    setListLeft(int node, int left);
    void    setListRight(int node, int right);

    void    saturateNode(int node);
    bool    isSaturated(int node);

    void    doMark();
    void    doSweep();
    void    markRecursive(int node);
    void    markNode(int node);
    void    unmarkNode(int node);
    bool    isNodeMarked(int node);

    int     applyRecursive(int node1, int node2, ApplyOp op);
    int     applyOp(int node1, int node2, ApplyOp op);
    int     applyRelativeOpRecursive(int node1, int node2, unsigned long sourceMask, int currentVariable, int distinguishedIndex, ApplyOp op);
    // FIXME: remove:
    int     applyRelativeOpRecursiveBak(int node1, int node2, unsigned long sourceMask, int distinguishedIndex, ApplyOp op);
    int     relativeOp(int node1, int node2, unsigned long stateMask, int distinguishedIndex, ApplyOp op);
    int     triangleSingleDestRec(int node1, int node2, unsigned long sourceMask, int currentVariable, int distinguishedIndex);
    int     transposeVectorRec(int node);
    double* evalRecursive(int node, bool* input);
    unsigned long accumulateIndex(int var, unsigned long acc, bool isRow);
    unsigned long resetIndex(int var, unsigned long acc, bool isRow);
    int     notRec(int node);
    int     mtbddNotRec(int node);
    int     mtbddAndWithVarRec(int node, int var, bool positiveVar);
    int     bddToMtbddRec(int bdd, int zeroTerminal, int oneTerminal);
    string* intToBits(int n, int numBits);
    string* interleaveBits(string* s1, string* s2);

public:
    NodeTable(int _terminalValueSize);
    NodeTable(int initialNodeNum, int _terminalValueSize);
    NodeTable(int initialNodeNum, TurnsFunction* turns, int _terminalValueSize);
    ~NodeTable();

    int     getNodeVar(int node);
    int     getNodeLo(int node);
    int     getNodeHi(int node);

    void    addVar(void);
    void    addVars(int howMany);
    int     getVarNum(void);

    int     getTerminalNode(double* value);
    double* getTerminalValue(int node);
    bool    isTerminal(int node);
    bool    isBooleanTerminal(int node);
    bool    isNonBooleanTerminal(int node);
    bool    isNodeFree(int node);
    int     getTerminalValueSize(void);

    void    ref(int node);
    void    deref(int node);

    void    garbageCollect();

    int     getVarNode(int var, int terminalLo, int terminalHi);

    int     apply(int node1, int node2, ApplyOp op);
    int     applyRelative(int node1, int node2, unsigned long sourceMask, int currentVariable, int distinguishedIndex, ApplyOp op);
    // FIXME: remove:
    int     applyRelativeBak(int node1, int node2, unsigned long sourceMask, int distinguishedIndex, ApplyOp op);
    int     triangleSingleDest(int node1, int node2, int distinguishedIndex);
    int     transposeVector(int node);
    int     diagonalMatrix(int diagonalTerminal, int fillTerminal);
    double* eval(int node, string* s);
    double* eval(int node, bool* input);

    int     bdd_not(int node);
    int     restrictVar(int node, int var, bool value);
    int     invertTopVar(int node, int var);

    int     mtbddVarNode(int var, int nodeLo, int nodeHi);
    int     mtbbdNot(int node);
    int     mtbddAndWithVar(int node, int var);
    int     mtbddAndWithNegVar(int node, int var);
    int     bddToMtbdd(int bdd, int zeroTerminal, int oneTerminal);
    int     pathBasedTree(bool* path, double* pathValue, double* elseValue);
    int     partialPathBasedTree(string path, double* pathValue, double* elseValue);

    void    printTable();
    void    printTerminalsTable();
    void    printMatrix(int node);
    void    printCacheUsageData();
};

#endif // NODETABLE_H_INCLUDED
