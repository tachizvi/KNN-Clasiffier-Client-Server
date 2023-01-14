#ifndef DISTANCES_H
#define DISTANCES_H
#include <vector>
using namespace std;

// Here we state all the different distance-calcualting algorithms we were asked to implement
    double Oclidic(vector<double> a,vector<double> b);
    double Manhattan(vector<double> a,vector<double> b);
    double Chebyshev(vector<double> a,vector<double> b);
    double Canberra(vector<double> a,vector<double> b);
    double Minkowski(vector<double> a,vector<double> b, int p);
    void runner(vector<double> a,vector<double> b);
#endif