#include <iostream>
#include <vector>
#include <cmath>
#include "Distances.h"
using namespace std;



// giving the vector iterator a more pleasent name since it's being used a lot in our program
using vector_iterator = std::vector<double>::iterator;

/* This file contais functions that are used to calculate the distance between two vectors
 (assuming they fit the criteria to do so)
 Each function takes two vetors of unknown size (but we made sure they're the same size)
 then it calculates the distance according the measurement of the stated distance-calculating
 algorithm state in the function's name */

// Calculates the oclidic distance between two vectors
double Oclidic(vector<double> a, vector<double> b)
{
    double sum = 0;
    vector_iterator j = b.begin();

    for (vector_iterator i = a.begin(); i != a.end(); i++)
    {
        sum += pow(*i - *j, 2);
        j++;
    }
    return sqrt(sum);
}

// Calculates the manhattan distance between two vectors
double Manhattan(vector<double> a, vector<double> b)
{
    double sum = 0;
    vector_iterator j = b.begin();

    for (vector_iterator i = a.begin(); i != a.end(); i++)
    {
        sum += abs(*i - *j);
        j++;
    }
    return sum;
}

// Calculates the chebyshev distance between two vectors
double Chebyshev(vector<double> a, vector<double> b)
{
    double max = 0;
    vector_iterator j = b.begin();

    for (vector_iterator i = a.begin(); i != a.end(); i++)
    {
        if (abs(*i - *j) > max)
        {
            max = abs(*i - *j);
        }
        j++;
    }
    return max;
}

// Calculates the canberra distance between two vectors
double Canberra(vector<double> a, vector<double> b)
{
    double sum = 0;
    vector_iterator j = b.begin();

    for (vector_iterator i = a.begin(); i != a.end(); i++)
    {
        /* We must make sure we're not dividing by zero
         Notice that if the sum of the absolute value of two values is 0
         each one of them must be zero (since for every x : |x| >= 0) , therefore it won't affect sum */
        if (abs(*i) + abs(*j) == 0)
        {
            continue;
        }
        sum += abs(*i - *j) / (abs(*i) + abs(*j));
        j++;
    }
    return sum;
}

// Calculates the minkowski distance between two vectors
double Minkowski(vector<double> a, vector<double> b, int p = 2)
{
    double sum = 0;
    vector_iterator j = b.begin();

    for (vector_iterator i = a.begin(); i != a.end(); i++)
    {
        sum += pow(abs(*i - *j), p);
        j++;
    }
    return pow(sum, 1 / p);
};

// This function accepts two vector as arguemnts then runs all of
// distance calculating functions we created on those vectors
void runner(vector<double> a, vector<double> b)
{
    std::fixed(std::cout);


    /* Since c++ doesn't suppot adding arguments to an array i've used a vector
     in order to store all of the returned distances from the functions */
    std::vector<double> distances;

    distances.push_back(Oclidic(a, b));
    distances.push_back(Manhattan(a, b));
    distances.push_back(Chebyshev(a, b));
    distances.push_back(Canberra(a, b));
    distances.push_back(Minkowski(a, b));
    for (vector_iterator i = distances.begin(); i != distances.end(); i++)
    {
        /* If the returned distance (x) of some algorithm is an int (whole number)
         we'd like to represent it as x.0 instead of x, therefore we're checking if it's
         a whole number by comparing it to it's floor value, and if it is equal, we're just 
         adding a decimal point followed by zero by setting the percision to 1*/
        if (*i == floor(*i))
        {   std::cout.precision(1);
            continue;
        }
        // incase we're dealing with a double, it prints it with 15 digits after the decimal point
        std::cout.precision(15);
      //  std::cout << (*i) << std::endl;
    }
}
