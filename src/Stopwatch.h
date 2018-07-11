/*
 * Stopwatch.h
 *
 *  Created on: 16/07/2012
 *      Author: pedrogl
 */

#ifndef STOPWATCH_H_
#define STOPWATCH_H_


#include <list>

using namespace std;

class Stopwatch
{
private:
    bool active;
    list<unsigned long> stops;
    unsigned long total;
    unsigned long currentTimeMillis(void);
public:
    Stopwatch();
    void start(void);
    void stop(void);
    void reset(void);
    list<unsigned long> getStops(void);
    unsigned long getLastInterval(void);
    double getLastIntervalSecs(void);
    unsigned long getTotal(void);
    double getTotalSecs(void);
};


#endif /* STOPWATCH_H_ */
