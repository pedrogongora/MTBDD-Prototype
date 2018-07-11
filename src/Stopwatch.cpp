/*
 * Stopwatch.cpp
 *
 *  Created on: 16/07/2012
 *      Author: pedrogl
 */

#include <iostream>
#include <stdexcept>
#include "Stopwatch.h"
#include <sys/time.h>

unsigned long Stopwatch::currentTimeMillis(void)
{
    timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}

Stopwatch::Stopwatch()
{
    active = false;
    total = 0;
}

void Stopwatch::start(void)
{
    active = true;
    stops.push_back(currentTimeMillis());
}

void Stopwatch::stop(void)
{
    if ( active ) {
        stops.push_back(currentTimeMillis());
        active = false;
        total += getLastInterval();
    }
}

void Stopwatch::reset(void)
{
    if ( active ) {
        active = false;
    }
    stops.clear();
    total = 0;
}


list<unsigned long> Stopwatch::getStops(void)
{
    return stops;
}

unsigned long Stopwatch::getLastInterval(void)
{
    if ( stops.size() < 2 ) throw logic_error("This Stopwatch doesn't have enough measures");
    list<unsigned long>::iterator it = stops.end();
    unsigned long tmp = *(--it);
    tmp -= *(--it);
    return tmp;
}

double Stopwatch::getLastIntervalSecs(void)
{
    unsigned long tmp = getLastInterval();
    return ((double)tmp / 1000.0);
}

unsigned long Stopwatch::getTotal(void)
{
    return total;
}

double Stopwatch::getTotalSecs(void)
{
    return ((double)total / 1000.0);
}

