#ifndef TOWERS_ALGO_H
#define TOWERS_ALGO_H

#include <stdlib.h>
#include <iostream>

using namespace std;

typedef struct Tower{

private:
    //type: 0-red, 1-green, 2-yellow, 3-blue
    int power, range, type, cost; //puissance, portée
    float pace; //cadence, 10e de sec

public:
    Tower();
    ~Tower();
    int get_power();
    void set_power(int pow);
    int get_range();
    void set_range(int rng);
    int get_type();
    void set_type(int type);
    int get_cost();
    void set_cost(int c);
    float get_pace();
    void set_pace(float p);
}Tower;



#endif 