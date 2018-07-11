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

#ifndef NODECACHE_H_INCLUDED
#define NODECACHE_H_INCLUDED

#include <iostream>
#include <list>
#include <map>
#include <set>
#include "ApplyOps.h"

using namespace std;

class NodeTable;


typedef union
{
    struct {
    	int intValue1, intValue2;
    };
    double doubleValue;
} DoubleIntValue;

typedef union
{
    struct {
        int intValue1, intValue2;
    };
    unsigned long longValue;
} LongIntValue;


class FourTuple
{
public:
    int first, second, third, fourth;
    FourTuple(int first, int second, int third, int fourth);
    FourTuple   operator = (FourTuple);
    bool        operator== (const FourTuple& other) const;
    bool        operator!= (const FourTuple& other) const;
    int         hashCode(void);
    string*     toString(void);
};


class FiveTuple
{
public:
    int first, second, third, fourth, fifth;
    FiveTuple(int first, int second, int third, int fourth, int fifth);
    FiveTuple   operator = (FiveTuple);
    bool        operator== (const FiveTuple& other) const;
    bool        operator!= (const FiveTuple& other) const;
    int         hashCode(void);
    string*     toString(void);
};


class SixTuple
{
public:
    int first, second, third, fourth, fifth, sixth;
    SixTuple(int first, int second, int third, int fourth, int fifth, int sixth);
    SixTuple   operator = (SixTuple);
    bool        operator== (const SixTuple& other) const;
    bool        operator!= (const SixTuple& other) const;
    int         hashCode(void);
    string*     toString(void);
};



class NodeCache
{
	const static unsigned int   MAX_CACHE_SIZE = 10000000;

    NodeTable*                  nodetable;
    map<int,list<int> >         nodeCache;
    map<int,int>                nodeCacheKeys;
    map<int,list<int> >         terminalCache;
    //map<int,int>                terminalCacheKeys;
    map<int,list<FourTuple*> >  applyCache;
    map<int,list<SixTuple*> >   relApplyCache;
    //map<int,set<int> >          applyCacheKeys;
    map<int,list<FiveTuple*> >  triangleCache;
    //map<int,set<int> >          triangleCacheKeys;
    int*                        workPair;
    int*                        workTriple;
    int*                        work5Tuple;
    LongIntValue                workLongUnion;
    DoubleIntValue              workDoubleUnion;

    int     hashPair(int a, int b);
    int     hashTriple(int a, int b, int c);
    int     hash4Tuple(int a, int b, int c, int d);
    int     hash5Tuple(int a, int b, int c, int d, int e);
    int     hashTerminalValue(double* v);
    int     hashIntArray(int* array, int len);
    int     hashDoubleArray(double* array, int len);
public:
    NodeCache(NodeTable* table);
    ~NodeCache();
    bool    isNodeCached(int var, int lo, int hi);
    int     getCachedNode(int var, int lo, int hi);
    void    chacheNode(int node, int var, int lo, int hi);
    bool    isTerminalCached(double* val);
    int     getCachedTerminal(double* val);
    void    cacheTerminal(int node);
    int     getApplyCachedNode(int node1, int node2, ApplyOp op);
    void    chacheApply(int node1, int node2, ApplyOp op, int resultNode);
    int     getRelApplyCachedNode(int node1, int node2, unsigned long mask, ApplyOp op);
    void    chacheRelApply(int node1, int node2, unsigned long mask, ApplyOp op, int resultNode);
    int     getTriangleCachedNode(int node1, int node2, int currentVariable, int distinguishedIndex);
    void    chacheTriangle(int node1, int node2, int currentVariable, int distinguishedIndex, int resultNode);
    void    removeNode(int node);
    void    clearAllCaches(void);
    void    printNodeCache();
};


#endif // NODECACHE_H_INCLUDED
