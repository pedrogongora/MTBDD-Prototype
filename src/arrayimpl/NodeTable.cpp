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

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <map>
#include <stack>
#include <sys/time.h>
#include "NodeTable.h"
#include "../Stopwatch.h"

#define MIN2(a,b) (((a)<(b))?(a):(b))
#define MAX2(a,b) (((a)>(b))?(a):(b))

using namespace std;


NodeTable::NodeTable(int _terminalValueSize)
{
    nodeNum = DEFAULT_NODES;
    terminalValueSize = _terminalValueSize;
    initialize();
}

NodeTable::NodeTable(int initialNodeNum, int _terminalValueSize)
{
    if (initialNodeNum >= MINIMUM_NODES)
    {
        nodeNum = initialNodeNum;
    }
    else
    {
        nodeNum = MINIMUM_NODES;
    }
    terminalValueSize = _terminalValueSize;
    initialize();
}

NodeTable::NodeTable(int initialNodeNum, TurnsFunction* turns, int _terminalValueSize)
{
    if (initialNodeNum >= MINIMUM_NODES) {
        nodeNum = initialNodeNum;
    } else {
        nodeNum = MINIMUM_NODES;
    }
    this->turns = turns;
    terminalValueSize = _terminalValueSize;
    initialize();
}

NodeTable::~NodeTable()
{
    // free table and list arrays
    delete [] nodeTable;
    delete [] nodeList;
    delete [] referenceCount;

    // free cache
    delete nodeCache;

    // free terminals arrays
    map<int,double*>::iterator it;
    for(it = terminals.begin(); it != terminals.end(); it++) {
        delete [] (*it).second;
    }

    delete [] workArray;
    delete [] workZeroes;
    delete [] workInftys;
}

void NodeTable::initialize(void)
{
    // allocate memory, create data structures
    nodeTable           = new int[nodeNum * TABLE_WIDTH];
    nodeList            = new int[nodeNum * LIST_WIDTH];
    referenceCount      = new short[nodeNum];
    nodeCache           = new NodeCache(this);
    nextTerminalKey     =  0;

    // initialize arrays
    for (int i=0; i<nodeNum; i++)
    {
        setNodeAttributes(i, 0, -1, -1);
        setListPointers(i, i-1, i+1);
        referenceCount[i] = 0;
    }

    // boolean terminals:
    setNodeAttributes(0, 0, 0, 0);
    setListPointers(0, nodeNum-1, 1);
    saturateNode(0);

    setNodeAttributes(1, 0, 1, 1);
    setListPointers(1, 0, 2);
    saturateNode(1);

    firstFreeNode   = 2;
    lastUsedNode    = 1;
    freeNodes       = nodeNum-2;
    varNum          = 0;
    nextTerminalKey = 1;
    gcNum           = 0;

    workArray       = new double[terminalValueSize];
    workZeroes      = new double[terminalValueSize];
    workInftys      = new double[terminalValueSize];
    for (int i=0; i<terminalValueSize; i++) workZeroes[i] = 0.0;
    for (int i=0; i<terminalValueSize; i++) workInftys[i] = numeric_limits<double>::infinity();
    zeroesTerminal = getTerminalNode(workZeroes);
    inftysTerminal = getTerminalNode(workInftys);
}



// node low-level access
void NodeTable::setNodeAttributes(int node, int var, int lo, int hi)
{
    nodeTable[node * TABLE_WIDTH + VAR_OFFSET] = var;
    nodeTable[node * TABLE_WIDTH + LO_OFFSET]  = lo;
    nodeTable[node * TABLE_WIDTH + HI_OFFSET]  = hi;
}

int NodeTable::getNodeVar(int node)
{
    return nodeTable[node * TABLE_WIDTH + VAR_OFFSET];
}

int NodeTable::getNodeLo(int node)
{
    return nodeTable[node * TABLE_WIDTH + LO_OFFSET];
}

int NodeTable::getNodeHi(int node)
{
    return nodeTable[node * TABLE_WIDTH + HI_OFFSET];
}

int NodeTable::newNode(int var, int lo, int hi)
{
    if ( lo==hi && var>=0 ) {
        return lo;
    }
    if ( nodeCache->isNodeCached(var, lo, hi) && var>=0 ) {
        return nodeCache->getCachedNode(var, lo, hi);
    }

    int newNode = -1;

    if (freeNodes == 2) {
        ref(lo);
        ref(hi);
        garbageCollect();
        deref(lo);
        deref(hi);
        if (freeNodes == 2) {
            // TODO: implement table resizing!
            throw overflow_error("Run out of memory at creating new node, table resizing not implemented yet :( ");
        }
    }

    newNode = firstFreeNode;
    setNodeAttributes(newNode, var, lo, hi);
    freeNodes--;
    firstFreeNode = getListRight(newNode);
    lastUsedNode  = newNode;

    nodeCache->chacheNode(newNode, var, lo, hi);

    return newNode;
}

// list low-level access
void NodeTable::setListPointers(int node, int left, int right)
{
    nodeList[node * LIST_WIDTH + LEFT_OFFSET]  = left;
    nodeList[node * LIST_WIDTH + RIGHT_OFFSET] = right;
}

int NodeTable::getListLeft(int node)
{
    return nodeList[node * LIST_WIDTH + LEFT_OFFSET];
}

int NodeTable::getListRight(int node)
{
    return nodeList[node * LIST_WIDTH + RIGHT_OFFSET];
}

void NodeTable::setListLeft(int node, int left)
{
    nodeList[node * LIST_WIDTH + LEFT_OFFSET] = left;
}

void NodeTable::setListRight(int node, int right)
{
    nodeList[node * LIST_WIDTH + RIGHT_OFFSET] = right;
}



void NodeTable::addVar(void)
{
    addVars(1);
}

void NodeTable::addVars(int howMany)
{
    varNum += howMany;
    if ( (unsigned)(varNum / 2) > sizeof(unsigned long) * 8 ) {
        throw range_error("too many vars :(");
    }
}

int NodeTable::getVarNum(void)
{
    return varNum;
}



/* A new array for storing is value created, and deleted when the destructor
 * of the class instance is called.
 */
int NodeTable::getTerminalNode(double* value)
{
    if ( nodeCache->isTerminalCached(value) ) {
        return nodeCache->getCachedTerminal(value);
    } else {
        double* newValue = new double[terminalValueSize];
        for (int i=0; i<terminalValueSize; i++) newValue[i] = value[i];
        int id = nextTerminalKey;
        nextTerminalKey++;
        int node = newNode(-id, 0, 0);
        setNodeAttributes(node, -id, node, node);
        saturateNode(node);
        terminals[id] = newValue;
        nodeCache->cacheTerminal(node);
        return node;
    }
}

double* NodeTable::getTerminalValue(int node)
{
    return terminals[-getNodeVar(node)];
}

bool NodeTable::isTerminal(int node)
{
    int lo  = getNodeLo(node);
    int hi  = getNodeHi(node);
    int var = getNodeVar(node);
    return node==lo && lo==hi && var<=0;
}

bool NodeTable::isBooleanTerminal(int node)
{
    return node==0 || node==1;
}

bool NodeTable::isNonBooleanTerminal(int node)
{
    int lo  = getNodeLo(node);
    int hi  = getNodeHi(node);
    int var = getNodeVar(node);
    return lo>=0 && hi>=0 && lo==hi && var<=0 && node!=0 && node!=1;
}

bool NodeTable::isNodeFree(int node)
{
    int lo  = getNodeLo(node);
    int hi  = getNodeHi(node);
    int var = getNodeVar(node);
    return lo==-1 && hi==-1 && var==0;
}

int NodeTable::getTerminalValueSize(void)
{
    return terminalValueSize;
}



// reference counter access
void NodeTable::saturateNode(int node)
{
    referenceCount[node] = MAX_REFERENCES;
}

bool NodeTable::isSaturated(int node)
{
    return referenceCount[node] == MAX_REFERENCES;
}

void NodeTable::ref(int node)
{
    if (!isSaturated(node)) {
        referenceCount[node]++;
    }
}

void NodeTable::deref(int node)
{
    if (!isSaturated(node)) {
        if (referenceCount[node] > 0) referenceCount[node]--;
    }
}




void NodeTable::garbageCollect()
{
    Stopwatch swatch;
    cout << "Starting garbage collection #"
    	 << (++gcNum)
    	 << " ("
         << freeNodes
         << " free nodes)."
         << endl;
    int f = freeNodes;
    swatch.start();
    cout << "Marking...";
    doMark();
    cout << "Ok" << endl;
    cout << "Sweeping...";
    doSweep();
    cout << "Ok" << endl;
    cout << "Clearing caches...";
    nodeCache->clearAllCaches();
    cout << "Ok" << endl;
    swatch.stop();
    cout << "GC took "
         << swatch.getLastInterval()
         << " ms, freed "
         << (freeNodes-f)
         << " nodes ("
         << freeNodes
         << " free nodes)."
         << endl;
}

void NodeTable::doMark()
{
    int node = lastUsedNode;
    for (node = 2; node<nodeNum; node++) {
        if (referenceCount[node] > 0 && !isNodeMarked(node) && !isNodeFree(node)) {
            markRecursive(node);
        }
    }
}

void NodeTable::doSweep()
{
    int node = getListLeft(lastUsedNode);
    int left, right, oldLeft, oldRight;//, oldLeftLeft;
    for (node=2; node<nodeNum; node++) {
        if (isNodeMarked(node)) {
            // this node is in use, just remove the mark
            unmarkNode(node);
        } else if (!isTerminal(node) && !isNodeFree(node)) {
            // this node is not in use, free node
            // first, disconnect the node from the list
            oldLeft     = getListLeft(node);
            //oldLeftLeft = getListLeft(oldLeft);
            oldRight    = getListRight(node);
            setListRight(oldLeft, oldRight);
            setListLeft(oldRight, oldLeft);
            // second, free the node
            setNodeAttributes(node, 0, -1, -1);
            referenceCount[node] = 0;
            // third, remove node from cache
            // this is time consuming for large tables, it seems better to
            // completely clear all caches
            // update: inevitable for the node cache (but not for the other caches)
            nodeCache->removeNode(node);
            // finally, make this node the first free node
            if (node == lastUsedNode) {
                left = oldLeft;
            } else {
                left = lastUsedNode;
            }
            lastUsedNode = left;
            if (freeNodes == 0) {
                right = 0;
                oldRight = 1;
            } else {
                right = firstFreeNode;
                oldRight = getListRight(firstFreeNode);
            }
            setListPointers(node, left, right);
            setListPointers(firstFreeNode, node, oldRight);
            setListRight(lastUsedNode, node);
            firstFreeNode = node;
            freeNodes++;
        }
    }
}

void NodeTable::markRecursive(int node)
{
    if ( 	   !isTerminal(node)
    		&& !isNodeMarked(node)
    		&& !isNodeFree(node) ) {
		markNode(node);
		markRecursive(getNodeLo(node));
		markRecursive(getNodeHi(node));
    }
}

void NodeTable::markNode(int node)
{
    if (!isSaturated(node)) {
        referenceCount[node] = (short) (referenceCount[node] | NODE_MASK);
    }
}

void NodeTable::unmarkNode(int node)
{
    referenceCount[node] = (short) (referenceCount[node] & NODE_UNMASK);
}

bool NodeTable::isNodeMarked(int node)
{
    return (referenceCount[node] & NODE_MASK) != 0;
}




int NodeTable::getVarNode(int var, int terminalLo, int terminalHi)
{
    if (isTerminal(terminalLo) && isTerminal(terminalHi)) {
        return newNode(var, terminalLo, terminalHi);
    } else {
        throw invalid_argument("both nodes, lo and hi, must be terminals");
    }
}



int NodeTable::apply(int node1, int node2, ApplyOp op)
{
    ref(node1);
    ref(node2);
    int result = applyRecursive(node1, node2, op);
    deref(node1);
    deref(node2);
    return result;
}

int NodeTable::applyRecursive(int node1, int node2, ApplyOp op)
{
    // look up for a cached node
    int cachedNode = nodeCache->getApplyCachedNode(node1, node2, op);
    if (cachedNode != -1) return cachedNode;

    // if op is not cached, then compute the value
    bool node1Terminal = isTerminal(node1);
    bool node2Terminal = isTerminal(node2);
    int var1 = getNodeVar(node1);
    int var2 = getNodeVar(node2);
    int result, var, lo, hi;

    if (node1Terminal && node2Terminal) {
        // both nodes are terminals
        return applyOp(node1, node2, op);
    }
    if (node1Terminal) {
        // only node1 is terminal
        var = var2;
        lo = applyRecursive(node1, getNodeLo(node2), op);
        ref(lo);
        hi = applyRecursive(node1, getNodeHi(node2), op);
        deref(lo);
    } else if (node2Terminal) {
        // only node2 is terminal
        var = var1;
        lo = applyRecursive(getNodeLo(node1), node2, op);
        ref(lo);
        hi = applyRecursive(getNodeHi(node1), node2, op);
        deref(lo);
    } else if(var1 == var2) { // both are non-terminals
        var = var1;
        lo = applyRecursive(getNodeLo(node1), getNodeLo(node2), op);
        ref(lo);
        hi = applyRecursive(getNodeHi(node1), getNodeHi(node2), op);
        deref(lo);
    } else if (var1 < var2) {
        var = var1;
        lo = applyRecursive(getNodeLo(node1), node2, op);
        ref(lo);
        hi = applyRecursive(getNodeHi(node1), node2, op);
        deref(lo);
    } else {
        var = var2;
        lo = applyRecursive(node1, getNodeLo(node2), op);
        ref(lo);
        hi = applyRecursive(node1, getNodeHi(node2), op);
        deref(lo);
    }
    // create a new node
    result = newNode(var, lo, hi);
    // cache result
    nodeCache->chacheApply(node1, node2, op, result);
    // et voilà
    return result;
}

int NodeTable::applyOp(int node1, int node2, ApplyOp op)
{
    // assumes that both nodes are terminals, and also that
    // either, both are boolean, or both are non-boolean
    int resultNode = -1;
    if (isBooleanTerminal(node1)) {
        bool b1 = node1==1;
        bool b2 = node2==1;
        switch (op) {
            case AND_OP:
                resultNode = (b1 && b2 ? 1 : 0);
                break;
            case OR_OP:
                resultNode = (b1 || b2 ? 1 : 0);
                break;
            case IMP_OP:
                resultNode = (!b1 || b2 ? 1 : 0);
                break;
            case IFF_OP:
                resultNode = (b1 == b2 ? 1 : 0);
                break;
            default:
                throw invalid_argument("Operation not implemented yet :(");
        }
    } else {
        double* value1 = getTerminalValue(node1);
        double* value2 = getTerminalValue(node2);
        // This is now a precondition
        //if (terminalValueSize != value2.length) {
        //    throw new InternalBDDException("Arrays in all terminal nodes"
        //            + " must have the same length");
        //}
        //double* workArray;
        //bool createdNewArray = false;
        double* result;
        bool left;
        switch (op) {
            case PLUS_OP:
                //workArray = new double[terminalValueSize];
                //createdNewArray = true;
                for (int i=0; i<terminalValueSize; i++) { workArray[i] = value1[i] + value2[i]; }
                result = workArray;
                break;
            case MINUS_OP:
                //workArray = new double[terminalValueSize];
                //createdNewArray = true;
                for (int i=0; i<terminalValueSize; i++) { workArray[i] = value1[i] - value2[i]; }
                result = workArray;
                break;
            case TIMES_OP:
                //workArray = new double[terminalValueSize];
                //createdNewArray = true;
                for (int i=0; i<terminalValueSize; i++) { workArray[i] = value1[i] * value2[i]; }
                result = workArray;
                break;
            case DIV_OP:
                //workArray = new double[terminalValueSize];
                //createdNewArray = true;
                for (int i=0; i<terminalValueSize; i++) { workArray[i] = value1[i] / value2[i]; }
                result = workArray;
                break;
            case MOD_OP_OP:
                //workArray = new double[terminalValueSize];
                //createdNewArray = true;
                for (int i=0; i<terminalValueSize; i++) { workArray[i] = (int)value1[i] % (int)value2[i]; }
                result = workArray;
                break;
            case SOME_MIN_OP:
                left = false;
                for (int i=0; i<terminalValueSize; i++) { left = left || (value1[i] < value2[i]); }
                result = (left ? value1 : value2);
                break;
            case SOME_MAX_OP:
                left = false;
                for (int i=0; i<terminalValueSize; i++) { left = left || (value1[i] > value2[i]); }
                result = (left ? value1 : value2);
                break;
            case ALL_MIN_OP:
                left = true;
                for (int i=0; i<terminalValueSize; i++) { left = left && (value1[i] <= value2[i]); }
                result = (left ? value1 : value2);
                break;
            case ALL_MAX_OP:
                left = true;
                for (int i=0; i<terminalValueSize; i++) { left = left && (value1[i] >= value2[i]); }
                result = (left ? value1 : value2);
                break;
            case ACCUMULATE_OP:
                //workArray = new double[terminalValueSize];
                //createdNewArray = true;
                for (int i=0; i<terminalValueSize; i++) {
                    workArray[i] = (value1[i]+value2[i] == numeric_limits<double>::infinity()
                            ? min(value1[i], value2[i])
                            : value1[i]+value2[i] );
                }
                result = workArray;
                break;
            default:
                throw invalid_argument("Operation not implemented yet :(");
        }
        resultNode = getTerminalNode(result);
        //if ( createdNewArray ) {
        //    delete [] workArray;
        //}
    }
    return resultNode;
}



int NodeTable::applyRelativeBak(int node1, int node2,
                             unsigned long sourceMask,
                             int distinguishedIndex,
                             ApplyOp op)
{
    ref(node1);
    ref(node2);
    int result = applyRelativeOpRecursiveBak(node1, node2, sourceMask, distinguishedIndex, op);
    deref(node1);
    deref(node2);
    return result;
}

int NodeTable::applyRelativeOpRecursiveBak(
        int node1, int node2,
        unsigned long sourceMask,
        int distinguishedIndex,
        ApplyOp op)
{
    // look up for a cached node
    int cachedNode = nodeCache->getRelApplyCachedNode(node1, node2, sourceMask, op);
    if (cachedNode != -1) return cachedNode;

    bool node1Terminal = isTerminal(node1);
    bool node2Terminal = isTerminal(node2);
    int var1 = getNodeVar(node1);
    int var2 = getNodeVar(node2);
    //var2 = var2;
    int result, var, lo, hi;
    if (node1Terminal && node2Terminal) {
        // both nodes are terminals
        return relativeOp(node1, node2, sourceMask,distinguishedIndex, op);
    }
    if (node1Terminal) {
        // only node1 is terminal
        var = var2;
        lo = applyRelativeOpRecursiveBak(node1, getNodeLo(node2), sourceMask, distinguishedIndex, op);
        ref(lo);
        //accumulateIndex(var, sourceMask, true);
        hi = applyRelativeOpRecursiveBak(node1, getNodeHi(node2), accumulateIndex(var, sourceMask, true), distinguishedIndex, op);
        //resetIndex(var, sourceMask, true);
        deref(lo);
    } else if (node2Terminal) {
        // only node2 is terminal
        var = var1;
        lo = applyRelativeOpRecursiveBak(getNodeLo(node1), node2, sourceMask, distinguishedIndex, op);
        ref(lo);
        //accumulateIndex(var, sourceMask, true);
        hi = applyRelativeOpRecursiveBak(getNodeHi(node1), node2, accumulateIndex(var, sourceMask, true), distinguishedIndex, op);
        //resetIndex(var, sourceMask, true);
        deref(lo);
    } else if(var1 == var2) { // both are non-terminals
        var = var1;
        lo = applyRelativeOpRecursiveBak(getNodeLo(node1), getNodeLo(node2), sourceMask, distinguishedIndex, op);
        ref(lo);
        //accumulateIndex(var, sourceMask, true);
        hi = applyRelativeOpRecursiveBak(getNodeHi(node1), getNodeHi(node2), accumulateIndex(var, sourceMask, true), distinguishedIndex, op);
        //resetIndex(var, sourceMask, true);
        deref(lo);
    } else if (var1 < var2) {
        var = var1;
        lo = applyRelativeOpRecursiveBak(getNodeLo(node1), node2, sourceMask, distinguishedIndex, op);
        ref(lo);
        //accumulateIndex(var, sourceMask, true);
        hi = applyRelativeOpRecursiveBak(getNodeHi(node1), node2, accumulateIndex(var, sourceMask, true), distinguishedIndex, op);
        //resetIndex(var, sourceMask, true);
        deref(lo);
    } else {
        var = var2;
        lo = applyRelativeOpRecursiveBak(node1, getNodeLo(node2), sourceMask, distinguishedIndex, op);
        ref(lo);
        //accumulateIndex(var, sourceMask, true);
        hi = applyRelativeOpRecursiveBak(node1, getNodeHi(node2), accumulateIndex(var, sourceMask, true), distinguishedIndex, op);
        //resetIndex(var, sourceMask, true);
        deref(lo);
    }
    // create a new node
    result = newNode(var, lo, hi);
    // et voilà
    return result;
}



int NodeTable::applyRelative(int node1, int node2,
                             unsigned long sourceMask,
                             int currentVariable,
                             int distinguishedIndex,
                             ApplyOp op)
{
    ref(node1);
    ref(node2);
    int result = applyRelativeOpRecursive(node1, node2, sourceMask, currentVariable, distinguishedIndex, op);
    deref(node1);
    deref(node2);
    return result;
}

int NodeTable::applyRelativeOpRecursive(
        int node1, int node2,
        unsigned long sourceMask,
        int currentVariable,
        int distinguishedIndex,
        ApplyOp op)
{
    // look up for a cached node
    int cachedNode = nodeCache->getRelApplyCachedNode(node1, node2, sourceMask, op);
    if (cachedNode != -1) return cachedNode;

    bool node1Terminal = isTerminal(node1);
    bool node2Terminal = isTerminal(node2);
    int var1 = getNodeVar(node1);
    int var2 = getNodeVar(node2);
    //var2 = var2;
    int result, var, lo, hi;
    if (node1Terminal && node2Terminal) {
        // both nodes are terminals
        if ( currentVariable > this->varNum ) {
            // we have already assigned all possible values to every variable
            return relativeOp(node1, node2, sourceMask,distinguishedIndex, op);
        } else {
            // we must assign all possible values to the remaining vars
            var = currentVariable;
            lo = applyRelativeOpRecursive(node1, node2, sourceMask, currentVariable+1, distinguishedIndex, op);
            ref(lo);
            hi = applyRelativeOpRecursive(node1, node2, accumulateIndex(var, sourceMask, true), currentVariable+1, distinguishedIndex, op);
            deref(lo);
        }
    } else if (node1Terminal) {
        // only node1 is terminal
        var = MIN2(var2, currentVariable);
        if (currentVariable < var2) {
            // we must assign all possible values to the remaining vars
            var = currentVariable;
            lo = applyRelativeOpRecursive(node1, node2, sourceMask, currentVariable+1, distinguishedIndex, op);
            ref(lo);
            hi = applyRelativeOpRecursive(node1, node2, accumulateIndex(var, sourceMask, true), currentVariable+1, distinguishedIndex, op);
            deref(lo);
        } else {
            lo = applyRelativeOpRecursive(node1, getNodeLo(node2), sourceMask, currentVariable+1, distinguishedIndex, op);
            ref(lo);
            //accumulateIndex(var, sourceMask, true);
            hi = applyRelativeOpRecursive(node1, getNodeHi(node2), accumulateIndex(var, sourceMask, true), currentVariable+1, distinguishedIndex, op);
            //resetIndex(var, sourceMask, true);
            deref(lo);
        }
    } else if (node2Terminal) {
        // only node2 is terminal
        var = MIN2(var1, currentVariable);
        if (currentVariable < var1) {
            // we must assign all possible values to the remaining vars
            var = currentVariable;
            lo = applyRelativeOpRecursive(node1, node2, sourceMask, currentVariable+1, distinguishedIndex, op);
            ref(lo);
            hi = applyRelativeOpRecursive(node1, node2, accumulateIndex(var, sourceMask, true), currentVariable+1, distinguishedIndex, op);
            deref(lo);
        } else {
            lo = applyRelativeOpRecursive(getNodeLo(node1), node2, sourceMask, currentVariable+1, distinguishedIndex, op);
            ref(lo);
            //accumulateIndex(var, sourceMask, true);
            hi = applyRelativeOpRecursive(getNodeHi(node1), node2, accumulateIndex(var, sourceMask, true), currentVariable+1, distinguishedIndex, op);
            //resetIndex(var, sourceMask, true);
            deref(lo);
        }
    } else if(var1 == var2) { // both are non-terminals
        var = MIN2(var1, currentVariable);
        if (currentVariable < var1) {
            // we must assign all possible values to the remaining vars
            var = currentVariable;
            lo = applyRelativeOpRecursive(node1, node2, sourceMask, currentVariable+1, distinguishedIndex, op);
            ref(lo);
            hi = applyRelativeOpRecursive(node1, node2, accumulateIndex(var, sourceMask, true), currentVariable+1, distinguishedIndex, op);
            deref(lo);
        } else {
            lo = applyRelativeOpRecursive(getNodeLo(node1), getNodeLo(node2), sourceMask, currentVariable+1, distinguishedIndex, op);
            ref(lo);
            //accumulateIndex(var, sourceMask, true);
            hi = applyRelativeOpRecursive(getNodeHi(node1), getNodeHi(node2), accumulateIndex(var, sourceMask, true), currentVariable+1, distinguishedIndex, op);
            //resetIndex(var, sourceMask, true);
            deref(lo);
        }
    } else if (var1 < var2) {
        var = MIN2(var1, currentVariable);
        if (currentVariable < var1) {
            // we must assign all possible values to the remaining vars
            var = currentVariable;
            lo = applyRelativeOpRecursive(node1, node2, sourceMask, currentVariable+1, distinguishedIndex, op);
            ref(lo);
            hi = applyRelativeOpRecursive(node1, node2, accumulateIndex(var, sourceMask, true), currentVariable+1, distinguishedIndex, op);
            deref(lo);
        } else {
            lo = applyRelativeOpRecursive(getNodeLo(node1), node2, sourceMask, currentVariable+1, distinguishedIndex, op);
            ref(lo);
            //accumulateIndex(var, sourceMask, true);
            hi = applyRelativeOpRecursive(getNodeHi(node1), node2, accumulateIndex(var, sourceMask, true), currentVariable+1, distinguishedIndex, op);
            //resetIndex(var, sourceMask, true);
            deref(lo);
        }
    } else {
        var = MIN2(var2, currentVariable);
        if (currentVariable < var2) {
            // we must assign all possible values to the remaining vars
            var = currentVariable;
            lo = applyRelativeOpRecursive(node1, node2, sourceMask, currentVariable+1, distinguishedIndex, op);
            ref(lo);
            hi = applyRelativeOpRecursive(node1, node2, accumulateIndex(var, sourceMask, true), currentVariable+1, distinguishedIndex, op);
            deref(lo);
        } else {
            lo = applyRelativeOpRecursive(node1, getNodeLo(node2), sourceMask, currentVariable+1, distinguishedIndex, op);
            ref(lo);
            //accumulateIndex(var, sourceMask, true);
            hi = applyRelativeOpRecursive(node1, getNodeHi(node2), accumulateIndex(var, sourceMask, true), currentVariable+1, distinguishedIndex, op);
            //resetIndex(var, sourceMask, true);
            deref(lo);
        }
    }
    // create a new node
    result = newNode(var, lo, hi);
    // et voilà
    return result;
}

int NodeTable::relativeOp(
        int node1, int node2,
        unsigned long stateMask,
        int distinguishedIndex,
        ApplyOp op)
{
    int index = turns->getStateTurn(stateMask);
    // assumes that both nodes are terminals and non-boolean
    double* value1 = getTerminalValue(node1);
    double* value2 = getTerminalValue(node2);
    // this is now a precondition
    //if (value1.length != value2.length) {
    //    throw new InternalBDDException("Arrays in all terminal nodes"
    //            + " must have the same length");
    //}

    int result = -1;
    //double* zeroes = NULL;
    //double* inftys = NULL;
    switch (op) {
        case REL_MIN_OP:
            if (value1[index] == value2[index] && value1[index] != numeric_limits<double>::infinity()) {
                result = (value1[distinguishedIndex] < value2[distinguishedIndex] ? node1 : node2);
            } else {
                result = (value1[index] < value2[index] ? node1 : node2);
            }
            break;
        case REL_MAX_OP:
            if (value1[index] == value2[index] && value1[index] != numeric_limits<double>::infinity()) {
                result = (value1[distinguishedIndex] > value2[distinguishedIndex] ? node1 : node2);
            } else {
                result = (value1[index] > value2[index] ? node1 : node2);
            }
            break;
        case REL_THRESHOLD_OP:
            //zeroes = new double[terminalValueSize];
            //for (int i=0; i<terminalValueSize; i++) zeroes[i] = 0.0;
            result = (value1[index] > value2[index] ? node1 : zeroesTerminal);
            break;
        case REL_BELOW_THRESHOLD_OP:
            //inftys = new double[terminalValueSize];
            //for (int i=0; i<terminalValueSize; i++) inftys[i] = numeric_limits<double>::infinity();
            result = (value1[index] <= value2[index] ? node1 : inftysTerminal);
            break;
        default:
            throw invalid_argument("Operation not implemented yet :(");
            break;
    }
    //if ( zeroes != NULL ) delete [] zeroes;
    //if ( inftys != NULL ) delete [] inftys;

    return result;
}




/* Modified Bahar et al algorithm
 */
int NodeTable::triangleSingleDest(
        int node1,
        int node2,
        int distinguishedIndex)
{
    //BYTE* smask = new BYTE[(int)(varNum / 2)];
    //for ( int i=0; i<(varNum/2); i++ ) smask[i] = 0;
    unsigned long smask = 0;
    int currentVariable = 1;
    int result = triangleSingleDestRec(node1, node2, smask, currentVariable, distinguishedIndex);
    //delete [] smask;
    return result;
}

int NodeTable::triangleSingleDestRec(
        int node1, int node2,
        unsigned long sourceMask,
        int currentVariable,
        int distinguishedIndex)
{
    // lookup in cache
    int cachedNode = nodeCache->getTriangleCachedNode(node1, node2, currentVariable, distinguishedIndex);
    if (cachedNode != -1) return cachedNode;

    bool node1Terminal = isTerminal(node1);
    bool node2Terminal = isTerminal(node2);
    int var1 = getNodeVar(node1);
    int var2 = getNodeVar(node2);
    int minVar = MIN2(var1, var2);
    minVar = (minVar == 0 ? MAX2(var1, var2) : minVar);
    int result, var, r1, r2, t1, t2;
    //BigInteger newSourceMask;

    if (node1Terminal && node2Terminal) {
        // if both nodes are terminals, then sum
        result = applyOp(node1, node2, PLUS_OP);
    } else {
        if (minVar > currentVariable) {
            var = currentVariable;
            int newMask = accumulateIndex(var, sourceMask, true);
            r1 = triangleSingleDestRec(node1, node2, sourceMask, currentVariable+1, distinguishedIndex);
            ref(r1);
            r2 = triangleSingleDestRec(node1, node2, newMask, currentVariable+1,  distinguishedIndex);
            ref(r2);
            if (var % 2 == 0) {
                // this is a column variable
                result = applyRelative(r1, r2, sourceMask, currentVariable, distinguishedIndex, REL_MIN_OP);
            } else {
                // this is a row variable
                result = newNode(var, r1, r2);
            }
            deref(r1);
            deref(r2);
        } else {
            // which is the highest var?
            if (node1Terminal) {
                var = var2;
            } else if (node2Terminal) {
                var = var1;
            } else if (var1 < var2) {
                var = var1;
            } else {
                var = var2;
            }
            var = MIN2(var, currentVariable);

            int newMask = accumulateIndex(var, sourceMask, true);

            t1 = restrictVar(node1, var, false);
            ref(t1);
            t2 = restrictVar(node2, var, false);
            ref(t2);
            r1 = triangleSingleDestRec(t1, t2, sourceMask, currentVariable+1, distinguishedIndex);
            ref(r1);
            deref(t1);
            deref(t2);

            t1 = restrictVar(node1, var, true);
            ref(t1);
            t2 = restrictVar(node2, var, true);
            ref(t2);
            //accumulateIndex(var, sourceMask, true);
            r2 = triangleSingleDestRec(t1, t2, newMask, currentVariable+1, distinguishedIndex);
            //resetIndex(var, sourceMask, true);
            ref(r2);
            deref(t1);
            deref(t2);
            if (var % 2 == 0) {
                // this is a column variable
                result = applyRelative(r1, r2, sourceMask, currentVariable, distinguishedIndex, REL_MIN_OP);
            } else {
                // this is a row variable
                result = newNode(var, r1, r2);
            }
            deref(r1);
            deref(r2);
        }
    }
    // cache result
    nodeCache->chacheTriangle(node1, node2, currentVariable, distinguishedIndex, result);
    // et voilà
    return result;
}

/*int NodeTable::triangleSingleDestRec(
        int node1, int node2,
        unsigned long sourceMask,
        int currentVariable,
        int distinguishedIndex)
{
    // lookup in cache
    int cachedNode = nodeCache->getTriangleCachedNode(node1, node2, currentVariable, distinguishedIndex);
    if (cachedNode != -1) return cachedNode;

    bool node1Terminal = isTerminal(node1);
    bool node2Terminal = isTerminal(node2);
    int var1 = getNodeVar(node1);
    int var2 = getNodeVar(node2);
    int result, var, r1, r2, t1, t2;
    //BigInteger newSourceMask;

    if (node1Terminal && node2Terminal) {
        if (currentVariable > this->varNum) {
            // if both nodes are terminals, then sum
            result = applyOp(node1, node2, PLUS_OP);
        } else {
            var = currentVariable;
            int newMask = accumulateIndex(var, sourceMask, true);
            r1 = triangleSingleDestRec(node1, node2, sourceMask, currentVariable+1, distinguishedIndex);
            ref(r1);
            r2 = triangleSingleDestRec(node1, node2, newMask, currentVariable+1,  distinguishedIndex);
            ref(r2);
            if (var % 2 == 0) {
                // this is a column variable
                result = applyRelative(r1, r2, sourceMask, currentVariable, distinguishedIndex, REL_MIN_OP);
            } else {
                // this is a row variable
                result = newNode(var, r1, r2);
            }
            deref(r1);
            deref(r2);
        }
    } else {
        // which is the highest var?
        if (node1Terminal) {
            var = var2;
        } else if (node2Terminal) {
            var = var1;
        } else if (var1 < var2) {
            var = var1;
        } else {
            var = var2;
        }
        var = MIN2(var, currentVariable);

        int newMask = accumulateIndex(var, sourceMask, true);

        t1 = restrictVar(node1, var, false);
        ref(t1);
        t2 = restrictVar(node2, var, false);
        ref(t2);
        r1 = triangleSingleDestRec(t1, t2, sourceMask, currentVariable+1, distinguishedIndex);
        ref(r1);
        deref(t1);
        deref(t2);

        t1 = restrictVar(node1, var, true);
        ref(t1);
        t2 = restrictVar(node2, var, true);
        ref(t2);
        //accumulateIndex(var, sourceMask, true);
        r2 = triangleSingleDestRec(t1, t2, newMask, currentVariable+1, distinguishedIndex);
        //resetIndex(var, sourceMask, true);
        ref(r2);
        deref(t1);
        deref(t2);
        if (var % 2 == 0) {
            // this is a column variable
            result = applyRelative(r1, r2, sourceMask, currentVariable, distinguishedIndex, REL_MIN_OP);
        } else {
            // this is a row variable
            result = newNode(var, r1, r2);
        }
        deref(r1);
        deref(r2);
    }
    // cache result
    nodeCache->chacheTriangle(node1, node2, currentVariable, distinguishedIndex, result);
    // et voilà
    return result;
}*/



int NodeTable::transposeVector(int node)
{
    return transposeVectorRec(node);
}

int NodeTable::transposeVectorRec(int node)
{
    if (isTerminal(node)) return node;
    int lo = getNodeLo(node);
    int hi = getNodeHi(node);
    int var = getNodeVar(node);
    int t1 = transposeVectorRec(lo);
    ref(t1);
    int t2 = transposeVectorRec(hi);
    deref(t1);
    if (var % 2 == 0) {
        return newNode(var-1, t1, t2);
    } else {
        return newNode(var+1, t1, t2);
    }
}



int NodeTable::diagonalMatrix(int diagonalTerminal, int fillTerminal)
{
	int res, inv;

	res = diagonalTerminal; //newNode(varNum, fillTerminal, diagonalTerminal);
	for ( int var=varNum; var>0; var-- ) {
		if  ( var % 2 == 0 ) {
			res = newNode(var, res, fillTerminal);
		} else {
		    inv = invertTopVar(res, var+1);
		    res = newNode(var, res, inv);
		}
	}
	return res;
}



// bit order: from most to less significant
double* NodeTable::eval(int node, string* s)
{
    if ( s->size() != (unsigned) varNum ) {
        throw invalid_argument("The input array must coincide with the number of declared (getVarNum()) variables");
    }
    bool* input = new bool[varNum];
    for (int i=0; i<varNum; i++) {
        if (s->at(i) == '0') input[i] = false;
        else if (s->at(i) == '1') input[i] = true;
        else throw invalid_argument("Invalid argument string");
    }
    double* result = evalRecursive(node, input);
    delete [] input;
    return result;
}

double* NodeTable::eval(int node, bool* input)
{
    return evalRecursive(node, input);
}

double* NodeTable::evalRecursive(int node, bool* input)
{
    if (isTerminal(node)) {
        if (isNonBooleanTerminal(node)) {
            return getTerminalValue(node);
        } else {
            throw invalid_argument("eval() is applicable only to MTBDDs");
        }
    } else {
        if (input[getNodeVar(node)-1]) {
            return evalRecursive(getNodeHi(node), input);
        } else {
            return evalRecursive(getNodeLo(node), input);
        }
    }
}



unsigned long NodeTable::accumulateIndex(int var, unsigned long acc, bool isRow)
{
    // this assumes varNum = 2^n for some n>0
    // we enumerate variables from 1 to 2^n (some n>0)
    // odd variables represent rows
    // even variables represent columns

    bool isEven = var % 2 == 0;
    if ( (isEven && isRow) || (!isEven && !isRow) ) {
        return acc;
    }
    /*int index;
    if (isEven) {
        index = var / 2 - 1;
    } else {
        index = var / 2;
    }
    acc[index] = 1;*/
    int shift = varNum/2 - var/2 - 1;
    unsigned long mask = 1 << shift;
    return acc | mask;
}

unsigned long NodeTable::resetIndex(int var, unsigned long acc, bool isRow)
{
    // this assumes varNum = 2^n for some n>0
    // we enumerate variables from 1 to 2^n (some n>0)
    // odd variables represent rows
    // even variables represent columns

    bool isEven = var % 2 == 0;
    if ( (isEven && isRow) || (!isEven && !isRow) ) {
        return acc;
    }
    /*int index;
    if (isEven) {
        index = var / 2 - 1;
    } else {
        index = var / 2;
    }
    acc[index] = 0;*/
    int shift = varNum/2 - var/2 - 1;
    unsigned long mask = ~(1 << shift);
    return acc & mask;
}



int NodeTable::bdd_not(int node)
{
    return notRec(node);
}

int NodeTable::notRec(int node)
{
    if (isTerminal(node)) {
        if (node == 0) return 1;
        if (node == 1) return 0;
        throw invalid_argument("node is not a bdd");
    }

    int var = getNodeVar(node);
    int lo = notRec(getNodeLo(node));
    ref(lo);
    int hi = notRec(getNodeHi(node));
    deref(lo);

    return newNode(var, lo, hi);
}



int NodeTable::restrictVar(int node, int var, bool value)
{
    if (isTerminal(node)) return node;
    int top = getNodeVar(node);
    if (var < top) {
        return node;
    } else if (var == top) {
        return (value ? getNodeHi(node) : getNodeLo(node));
    } else {
        int lo = restrictVar(getNodeLo(node), var, value);
        ref(lo);
        int hi = restrictVar(getNodeHi(node), var, value);
        ref(hi);
        int result = newNode(var, lo, hi);
        deref(lo);
        deref(hi);
        return result;
    }
}



int NodeTable::invertTopVar(int node, int var)
{
	if ( isTerminal(node) )return node;

	int v   = getNodeVar(node);
	int lo  = getNodeLo (node);
	int hi  = getNodeHi (node);

	if ( v != var ) return node;

	ref(node);
	int result = newNode(var, hi, lo);
	deref(node);
	return result;
}



int NodeTable::mtbddVarNode(int var, int nodeLo, int nodeHi)
{
    if (!isTerminal(nodeLo) || !isTerminal(nodeHi)) {
        throw invalid_argument("mtbddVarNode() arguments must be terminal nodes");
    }
    return newNode(var, nodeLo, nodeHi);
}

int NodeTable::mtbbdNot(int node)
{
    return mtbddNotRec(node);
}

int NodeTable::mtbddNotRec(int node)
{
    if (isTerminal(node)) return node;

    int var = getNodeVar(node);
    int lo = mtbddNotRec(getNodeLo(node));
    ref(lo);
    int hi = mtbddNotRec(getNodeHi(node));
    deref(lo);

    return newNode(var, hi, lo);
}

int NodeTable::mtbddAndWithVar(int node, int var)
{
    return mtbddAndWithVarRec(node, var, true);
}

int NodeTable::mtbddAndWithNegVar(int node, int var)
{
    return mtbddAndWithVarRec(node, var, false);
}

int NodeTable::mtbddAndWithVarRec(int node, int var, bool positiveVar)
{
    int topVar = getNodeVar(node);
    int result;
    if (topVar > var) {
        result = node;
    } else if (topVar < var) {
        int lo = mtbddAndWithVarRec(getNodeLo(node), var, positiveVar);
        ref(lo);
        int hi = mtbddAndWithVarRec(getNodeHi(node), var, positiveVar);
        deref(lo);
        result = newNode(var, lo, hi);
    } else { // topVar == var
        if (positiveVar)    result = getNodeHi(node);
        else                result = getNodeLo(node);
    }
    return result;
}



int NodeTable::bddToMtbdd(int bdd, int zeroTerminal, int oneTerminal)
{
    return bddToMtbddRec(bdd, zeroTerminal, oneTerminal);
}

int NodeTable::bddToMtbddRec(int bdd, int zeroTerminal, int oneTerminal)
{
    if (isTerminal(bdd)) {
        if (bdd == 0) return zeroTerminal;
        if (bdd == 1) return oneTerminal;
        throw invalid_argument("node is not a bdd");
    }
    ref(bdd);
    int lo = bddToMtbddRec(getNodeLo(bdd), zeroTerminal, oneTerminal);
    ref(lo);
    int hi = bddToMtbddRec(getNodeHi(bdd), zeroTerminal, oneTerminal);
    deref(lo);
    deref(bdd);
    return newNode(getNodeVar(bdd), lo, hi);
}



int NodeTable::pathBasedTree(bool* path, double* pathValue, double* elseValue)
{
    int top;
    int pathLen = varNum;
    // this is now a precondition
    //int pathLen = path.length;
    //if (pathLen != varNum) {
    //    throw new InternalBDDException("pathBasedTree needs an assignment"
    //            + " for each variable");
    //}
    int pathTerminal = getTerminalNode(pathValue);
    int elseTerminal = getTerminalNode(elseValue);
    top = pathTerminal;
    int var, lo, hi;
    for (int i=pathLen-1; i>=0; i--) {
        var = i+1;
        if (path[i])    { hi = top; lo = elseTerminal; }
        else            { lo = top; hi = elseTerminal; }
        top = newNode(var, lo, hi);
        ref(top);
    }
    int tmp = top;
    for (int i=0; i<pathLen-1; i++) {
        deref(tmp);
        tmp = (path[i] ? getNodeHi(tmp) : getNodeLo(tmp));
    }
    deref(tmp);
    return top;
}

int NodeTable::partialPathBasedTree(string path, double* pathValue, double* elseValue)
{
    const char* charPath = path.c_str();
    int top;
    int pathLen = varNum; //charPath.length;
    stack<int> nodeStack;
    // Now a precondition:
    //if (pathLen != varNum) {
    //    throw new InternalBDDException("pathBasedTree needs an assignment"
    //            + " for each variable");
    //}
    int pathTerminal = getTerminalNode(pathValue);
    int elseTerminal = getTerminalNode(elseValue);
    top = pathTerminal;
    int var, lo, hi;
    for (int i=pathLen-1; i>=0; i--) {
        var = i+1;
        if (charPath[i]=='1') {
            hi = top;
            lo = elseTerminal;
            top = newNode(var, lo, hi);
            ref(top);
            nodeStack.push(top);
        } else if (charPath[i]=='0') {
            lo = top;
            hi = elseTerminal;
            top = newNode(var, lo, hi);
            ref(top);
            nodeStack.push(top);
        }
    }

    int tmp;
    while (!nodeStack.empty()) {
        tmp = nodeStack.top();
        nodeStack.pop();
        deref(tmp);
    }

    return top;
}



void NodeTable::printTable()
{
    cout << setfill(' ');
    cout << setw(6) << ""
         << setw(6) << "node"
         << setw(6) << "var"
         << setw(6) << "lo"
         << setw(6) << "hi"
         << setw(6) << "refcount"
         << endl;
    cout << " ----- -----  -----  -----  ----- ----- " << endl;
    int node = 0; //, count = 0;
    while (node != firstFreeNode) {
        cout << setw(6) << "" //++count,
             << setw(6) << node
             << setw(6) << getNodeVar(node)
             << setw(6) << getNodeLo(node)
             << setw(6) << getNodeHi(node)
             << setw(6) << referenceCount[node]
             << endl;
        node = getListRight(node);
    }
    cout << endl;
    printTerminalsTable();
    nodeCache->printNodeCache();
    cout << endl;
}

void NodeTable::printTerminalsTable()
{
    cout << setfill(' ');
    cout << setw(6) << "terminal"
         << setw(6) << "value"
         << endl;
    cout << " -----  ----- " << endl;
    map<int,double*>::iterator it;
    for(it = terminals.begin(); it != terminals.end(); it++) {
        cout << setw(6) << (*it).first;
        cout << "[";
        double* value = (*it).second;
        for (int idx=0; idx<terminalValueSize; idx++) {
            cout << setw(6) << value[idx];
            //cout << setw(6) << setprecision(2) << value[idx];
            //System.out.format(Locale.US, " %6.2f", v);
        }
        cout << " ]" << endl;
    }
    cout << endl;
}

void NodeTable::printMatrix(int node)
{
    if (varNum / 2 > 15) invalid_argument("Matrix too big");
    int numBits = varNum / 2;
    int numStates = 1 << numBits;
    cout << setfill(' ');
    for (int row=0; row<numStates; row++) {
        for (int col=0; col<numStates; col++) {
            string* s1 = intToBits(row, numBits);
            string* s2 = intToBits(col, numBits);
            string* cellVars = interleaveBits(s1, s2);
            double* cell = eval(node, cellVars);
            cout << "[";
            for (int i=0; i<terminalValueSize; i++) {
            	cout << setw(3) << cell[i];
            	if (i < terminalValueSize-1 ) cout << ",";
            }
            cout << "]\t";
            //cout << getNodeVar(getTerminalNode(cell)) << "\t";
            delete cellVars;
            delete s1;
            delete s2;
        }
        cout << endl;
    }
}

string* NodeTable::intToBits(int n, int numBits)
{
    stringstream sb;
    int mask = 1 << (numBits-1);
    for (int i=0; i<numBits; i++) {
        sb << ((n&mask) != 0 ? '1': '0');
        mask = mask >> 1;
    }
    return new string ( sb.str() );
}

string* NodeTable::interleaveBits(string* s1, string* s2)
{
    stringstream sb;
    for (unsigned i=0; i<s1->size(); i++) {
        sb << s1->at(i);
        sb << s2->at(i);
    }
    return new string ( sb.str() );
}

void NodeTable::printCacheUsageData()
{
	nodeCache->printNodeCache();
}

TurnsFunction::~TurnsFunction()
{
    ///
}

ModTurnsFunction::ModTurnsFunction(int agentNum, int bitLength)
{
    this->agentNum = agentNum;
    this->bitLength = bitLength;
    if ( bitLength > 64 ) throw out_of_range("bitLength < 64");
}

ModTurnsFunction::~ModTurnsFunction()
{
    ///
}

int ModTurnsFunction::getStateTurn(unsigned long state)
{
    //unsigned long s = 0;
    //unsigned long mask;
    //for( int bit=0; bit<bitLength; bit++ ) {
    //    if ( state[bit] == 1 ) {
    //        mask = 1 << (bitLength - bit - 1);
    //        s = s | mask;
    //    }
    //}
    return state % agentNum;
}
