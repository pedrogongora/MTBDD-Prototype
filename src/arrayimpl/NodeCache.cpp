/*  **************************************************************************
 *
 *  MTBDD node cache implementation using C++'s STL.
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
#include <algorithm>
#include <stdexcept>
#include "NodeCache.h"
#include "NodeTable.h"

using namespace std;

NodeCache::NodeCache(NodeTable* table)
{
    nodetable   = table;
    workPair    = new int[2];
    workTriple  = new int[3];
    work5Tuple  = new int[5];
}

NodeCache::~NodeCache()
{
    delete [] workPair;
    delete [] workTriple;
    delete [] work5Tuple;
}


bool NodeCache::isNodeCached(int var, int lo, int hi)
{
    int hash = hashTriple(var, lo, hi);
    if ( nodeCache.find(hash) != nodeCache.end() ) {
        bool found;
        list<int>::iterator it;
        for (it=nodeCache[hash].begin(); it!=nodeCache[hash].end(); it++) {
            found =    nodetable->getNodeVar(*it) == var
                    && nodetable->getNodeLo(*it)  == lo
                    && nodetable->getNodeHi(*it)  == hi;
            if (found) return true;
        }
    }
    return false;
}


int NodeCache::getCachedNode(int var, int lo, int hi)
{
    int hash = hashTriple(var, lo, hi);
    if ( nodeCache.find(hash) != nodeCache.end() ) {
        bool found;
        list<int>::iterator it;
        for (it=nodeCache[hash].begin(); it!=nodeCache[hash].end(); it++) {
            found =    nodetable->getNodeVar(*it) == var
                    && nodetable->getNodeLo(*it)  == lo
                    && nodetable->getNodeHi(*it)  == hi;
            if (found) return (*it);
        }
    }
    return -1;
}


void NodeCache::chacheNode(int node, int var, int lo, int hi)
{
	//if ( nodeCache.size() >= MAX_CACHE_SIZE ) {
	//	cout << "cleaning node cache.";
	//	nodeCache.clear();
	//	cout << ".";
	//	//nodeCacheKeys.clear();
	//	cout << "." << endl;
	//}
    int hash = hashTriple(var, lo, hi);
    nodeCache[hash].push_front(node);
    nodeCacheKeys[node] = hash;
}


bool NodeCache::isTerminalCached(double* val)
{
    int hash = hashTerminalValue(val);

    double* tmp;
    bool found;
    if ( terminalCache.find(hash) != terminalCache.end() ) {
        list<int>::iterator it;
        for( it=terminalCache[hash].begin(); it!=terminalCache[hash].end(); it++ ) {
            tmp = nodetable->getTerminalValue(*it);
            found = true;
            for ( int i=0; i<nodetable->getTerminalValueSize(); i++ ) {
                found = found && (val[i] == tmp[i]);
            }
            if (found) return true;
        }
    }
    return false;
}


int NodeCache::getCachedTerminal(double* val)
{
    int hash = hashTerminalValue(val);
    double* tmp;
    bool found;
    if ( terminalCache.find(hash) != terminalCache.end() ) {
        list<int>::iterator it;
        for( it=terminalCache[hash].begin(); it!=terminalCache[hash].end(); it++ ) {
            tmp = nodetable->getTerminalValue(*it);
            found = true;
            for ( int i=0; i<nodetable->getTerminalValueSize(); i++ ) {
                found = found && (val[i] == tmp[i]);
            }
            if (found) return (*it);
        }
    }
    return -1;
}


void NodeCache::cacheTerminal(int node)
{
    int hash = hashTerminalValue(nodetable->getTerminalValue(node));
    //list<int> l =terminalCache[hash];
    //l.insert(l.begin(), node);
    terminalCache[hash].push_front(node);
}


int NodeCache::getApplyCachedNode(int node1, int node2, ApplyOp op)
{
    int hash = hashTriple(node1, node2, op);
    bool found;
    if ( applyCache.find(hash) != applyCache.end() ) {
        list<FourTuple*>::iterator it;
        FourTuple* tuple;
        for ( it=applyCache[hash].begin(); it!=applyCache[hash].end(); it++ ) {
            tuple = (*it);
            found =    tuple->first  == node1
                    && tuple->second == node2
                    && tuple->third  == op;
            if (found){
                // FIXME: dirty trick!!
                if (nodetable->isNodeFree(tuple->fourth)) return -1;
                return tuple->fourth;
            }
        }
    }
    return -1;
}


void NodeCache::chacheApply(int node1, int node2, ApplyOp op, int resultNode)
{
    if ( applyCache.size() >= MAX_CACHE_SIZE ) {
        cout << "cleaning apply cache.";
        applyCache.clear();
        cout << ".";
        //applyCacheKeys.clear();
        cout << "." << endl;
    }
    int hash = hashTriple(node1, node2, op);
    FourTuple* tuple = new FourTuple(node1, node2, op, resultNode);
    applyCache[hash].push_front(tuple);
    //applyCacheKeys[resultNode].insert(hash);
}


int NodeCache::getRelApplyCachedNode(int node1, int node2, unsigned long mask, ApplyOp op)
{
    workLongUnion.longValue = mask;
    int hash = hash5Tuple(node1, node2, workLongUnion.intValue1, workLongUnion.intValue2, op);
    bool found;
    if ( relApplyCache.find(hash) != relApplyCache.end() ) {
        list<SixTuple*>::iterator it;
        SixTuple* tuple;
        for ( it=relApplyCache[hash].begin(); it!=relApplyCache[hash].end(); it++ ) {
            tuple = (*it);
            found =    tuple->first  == node1
                    && tuple->second == node2
                    && tuple->third  == workLongUnion.intValue1
                    && tuple->fourth == workLongUnion.intValue2
                    && tuple->fifth  == op;
            if (found){
                // FIXME: dirty trick!!
                if (nodetable->isNodeFree(tuple->sixth)) return -1;
                return tuple->sixth;
            }
        }
    }
    return -1;
}


void NodeCache::chacheRelApply(int node1, int node2, unsigned long mask, ApplyOp op, int resultNode)
{
    if ( applyCache.size() >= MAX_CACHE_SIZE ) {
        cout << "cleaning relative apply cache.";
        relApplyCache.clear();
        cout << ".";
        cout << "." << endl;
    }
    workLongUnion.longValue = mask;
    int hash = hash5Tuple(node1, node2, workLongUnion.intValue1, workLongUnion.intValue2, op);
    SixTuple* tuple = new SixTuple(node1, node2, workLongUnion.intValue1, workLongUnion.intValue2, op, resultNode);
    relApplyCache[hash].push_front(tuple);
}


int NodeCache::getTriangleCachedNode(int node1, int node2, int currentVariable, int distinguishedIndex)
{
    int hash = hash4Tuple(node1, node2, currentVariable, distinguishedIndex);
    bool found;
    if ( triangleCache.find(hash) != triangleCache.end() ) {
        list<FiveTuple*>::iterator it;
        FiveTuple* tuple;
        for ( it=triangleCache[hash].begin(); it!=triangleCache[hash].end(); it++ ) {
            tuple = (*it);
            found =    tuple->first  == node1
                    && tuple->second == node2
                    && tuple->third  == currentVariable
                    && tuple->fourth == distinguishedIndex;
            if (found) {
                // FIXME: dirty trick!!
                if (nodetable->isNodeFree(tuple->fourth)) return -1;
                return tuple->fifth;
            }
        }
    }
    return -1;
}


void NodeCache::chacheTriangle(int node1, int node2, int currentVariable, int distinguishedIndex, int resultNode)
{
	if ( triangleCache.size() >= MAX_CACHE_SIZE ) {
		cout << "cleaning triangle cache.";
		triangleCache.clear();
		cout << ".";
		//triangleCacheKeys.clear();
		cout << "." << endl;
	}
    int hash = hash4Tuple(node1, node2, currentVariable, distinguishedIndex);
    triangleCache[hash].push_front(new FiveTuple(node1, node2, currentVariable, distinguishedIndex, resultNode));
    //triangleCacheKeys[resultNode].insert(hash);
}


void NodeCache::removeNode(int node)
{
    //unsigned int counter;
    // remove from node cache
    if ( nodeCacheKeys.find(node) != nodeCacheKeys.end() ) {
        int key = nodeCacheKeys[node];
        nodeCacheKeys.erase(node);
        if ( nodeCache[key].size() == 1 ) nodeCache.erase(key);
        else nodeCache[key].remove(node);
    }

    // remove from apply cache
    /*if ( applyCacheKeys.find(node) != applyCacheKeys.end() ) {
        //set<int> hashes = applyCacheKeys[node];
        //set<int>::iterator it;
        int key;
        FourTuple* t1;
        FourTuple* t2;
        //for ( it=hashes.begin(); it!=hashes.end(); it++ ) {
        counter = applyCacheKeys[node].size();
		for (   set<int>::iterator it=applyCacheKeys[node].begin();
				it!=applyCacheKeys[node].end();
				it++ ) {
		    if (--counter < 0) throw runtime_error("error al quitar nodo de applyCache");
                key = (*it);
            if (applyCache[key].size() == 1) applyCache.erase(key);
            else {
                list<FourTuple*>::iterator ite;
                t1 = new FourTuple(0,0,0,node);
                ite = find(applyCache[key].begin(), applyCache[key].end(), t1);
                if ( ite != applyCache[key].end() ) {
                    t2 = *ite;
                    applyCache[key].erase(ite);
                    delete t2;
                    //if (applyCache[key].size() == 0) applyCache.erase(key);
                }
                delete t1;
            }
        }
        applyCacheKeys.erase(node);
    }

    // remove from triangle cache
    if ( triangleCacheKeys.find(node) != triangleCacheKeys.end() ) {
        //set<int> hashes = triangleCacheKeys[node];
        //set<int>::iterator it;
        int key;
        FourTuple* t1;
        FourTuple* t2;
        //for ( it=hashes.begin(); it!=hashes.end(); it++ ) {
        counter = triangleCache[node].size();
        for (   set<int>::iterator it=triangleCacheKeys[node].begin();
        		it!=triangleCacheKeys[node].end();
        		it++ ) {
            if (--counter < 0) throw runtime_error("error al quitar nodo de triangleCache");
            key = (*it);
            if (triangleCache[key].size() == 1) triangleCache.erase(key);
            else {
                list<FourTuple*>::iterator ite;
                t1 = new FourTuple(0,0,0,node);
                ite = find(triangleCache[key].begin(), triangleCache[key].end(), t1);
                if ( ite != triangleCache[key].end() ) {
                    t2 = *ite;
                    triangleCache[key].erase(ite);
                    delete t2;
                }
                delete t1;
            }
        }
        triangleCacheKeys.erase(node);
    }*/
}

void NodeCache::clearAllCaches(void)
{
    //nodeCache.clear();
    //nodeCacheKeys.clear();
    applyCache.clear();
    //applyCacheKeys.clear();
    triangleCache.clear();
    //triangleCacheKeys.clear();
    relApplyCache.clear();
}


void NodeCache::printNodeCache()
{
    cout 	<< "nodeCache usage data: " << endl
            << "Node cache size: "
            << nodeCache.size() << endl
            << "Node cache keys: "
            //<< nodeCacheKeys.size() << endl
            << "Apply cache size: "
            << applyCache.size() << endl
            << "Apply cache keys: "
            //<< applyCacheKeys.size() << endl
            << "Triangle cache size: "
            << triangleCache.size() << endl
            << "Triangle cache keys: "
            //<< triangleCacheKeys.size() << endl
            << "Terminal cache size: "
            << terminalCache.size() << endl
            << "Terminal cache keys: " << endl;
            //<< terminalCacheKeys.size() << endl;
}




int NodeCache::hashPair(int a, int b)
{
    workPair[0] = a;
    workPair[1] = b;
    return hashIntArray(workPair, 2);
}

int NodeCache::hashTriple(int a, int b, int c)
{
    workTriple[0] = a;
    workTriple[1] = b;
    workTriple[2] = c;
    return hashIntArray(workTriple, 3);
}

int NodeCache::hash4Tuple(int a, int b, int c, int d)
{
    work5Tuple[0] = a;
    work5Tuple[1] = b;
    work5Tuple[2] = c;
    work5Tuple[3] = d;
    return hashIntArray(work5Tuple, 4);
}

int NodeCache::hash5Tuple(int a, int b, int c, int d, int e)
{
    work5Tuple[0] = a;
    work5Tuple[1] = b;
    work5Tuple[2] = c;
    work5Tuple[3] = d;
    work5Tuple[4] = e;
    return hashIntArray(work5Tuple, 5);
}

int NodeCache::hashTerminalValue(double* v)
{
    return hashDoubleArray(v, nodetable->getTerminalValueSize());
}


int NodeCache::hashIntArray(int* array, int len)
{
    int hash = 5;
    for (int i=0; i<len; i++) {
        hash = 67 * hash + array[i];
    }
    return hash;
}

int NodeCache::hashDoubleArray(double* array, int len)
{
    //DoubleIntValue tmp;
    int hash = 5;
    for (int i=0; i<len; i++) {
        workDoubleUnion.doubleValue = array[i];
        hash = 67 * hash + workDoubleUnion.intValue2 + workDoubleUnion.intValue2;
    }
    return hash;
}




FourTuple::FourTuple(int first, int second, int third, int fourth)
{
    this->first     = first;
    this->second    = second;
    this->third     = third;
    this->fourth    = fourth;
}

bool FourTuple::operator== (const FourTuple &other) const
{
    return ( fourth == other.fourth );
}

bool FourTuple::operator!= (const FourTuple &other) const
{
    return !(*this == other);
}

int FourTuple::hashCode(void)
{
    int hash = 5;
    hash = 67 * hash + fourth;
    return hash;
}

string* FourTuple::toString(void)
{
    stringstream sb;
    sb  << "FourTuple{"
        << "first="     << first
        << ", second="  << second
        << ", third="   << third
        << ", fourth="  << fourth
        << "}";
    return new string( sb.str() );
}




FiveTuple::FiveTuple(int first, int second, int third, int fourth, int fifth)
{
    this->first   = first;
    this->second  = second;
    this->third   = third;
    this->fourth  = fourth;
    this->fifth   = fifth;
}

bool FiveTuple::operator== (const FiveTuple &other) const
{
    return ( fifth == other.fifth );
}

bool FiveTuple::operator!= (const FiveTuple &other) const
{
    return !(*this == other);
}

int FiveTuple::hashCode(void)
{
    int hash = 5;
    hash = 67 * hash + fifth;
    return hash;
}

string* FiveTuple::toString(void)
{
    stringstream sb;
    sb  << "FiveTuple{"
        << "first="     << first
        << ", second="  << second
        << ", third="   << third
        << ", fourth="  << fourth
        << ", fifth="   << fifth
        << "}";
    return new string( sb.str() );
}




SixTuple::SixTuple(int first, int second, int third, int fourth, int fifth, int sixth)
{
    this->first   = first;
    this->second  = second;
    this->third   = third;
    this->fourth  = fourth;
    this->fifth   = fifth;
    this->sixth   = sixth;
}

bool SixTuple::operator== (const SixTuple &other) const
{
    return ( sixth == other.sixth );
}

bool SixTuple::operator!= (const SixTuple &other) const
{
    return !(*this == other);
}

int SixTuple::hashCode(void)
{
    int hash = 5;
    hash = 67 * hash + sixth;
    return hash;
}

string* SixTuple::toString(void)
{
    stringstream sb;
    sb  << "SixTuple{"
        << "first="     << first
        << ", second="  << second
        << ", third="   << third
        << ", fourth="  << fourth
        << ", fifth="   << fifth
        << ", sixth="   << sixth
        << "}";
    return new string( sb.str() );
}
