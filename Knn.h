#ifndef KNN_H
#define KNN_H
#include <map>
#include <vector>
#include <string>
#include "Distances.h"

class Knn{
    protected:
        int k;  // the k input for knn algorithm
        std::multimap< std::vector<double> , std::string> data; // the data we get to show the result
        std::multimap<double , std::string> nearest_map;    // the map of distances , sorted 

        /// @brief used for constructing the Class
        /// @param k - the k parameter for the Knn
        /// @param data - a map to get the distances data from
        void add_data(int k, std::multimap< std::vector<double> , std::string> data);    // used for constructors


        /// @brief builds the nearest_map using the vector assigned , and the Knn subClass distance
        /// @param unclassified a vector of doubles to check type tp
        void fill_nearest_map(std::vector<double> unclassified) ;    

        /// @brief returns the max used name in the Knn object
        /// @return the maximum used name in the Knn object
        std::string max_used_name() ; 

        /// @brief  returns the largest name from the map assigned based on value
        /// @param count_map a map of string's and their count
        /// @return the maximum used name - larget value
        std::string max_counter(std::map<std::string, int> count_map) ; 

        /// @brief virtual function to get distance - up to subClasses
        /// @param vect1 a vector of doubles
        /// @param vect2 a vector of doubles
        /// @return the distance between the doubles
        virtual double distance(std::vector<double> vect1, std::vector<double> vect2) {return 0;}

    public:

        /// @brief  returns the type of the vector assigned based on data used
        /// @param  unclassified a vector of doubles 
        /// @return the tpye of the vector assigned
        std::string get_type(std::vector<double> unclassified);  
        
};

#endif