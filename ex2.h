#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>  
#include <sstream>
#include <map>
#include "input.h"
#include "Knn.h"
#include "Knn_subClasses.h"


// Here i took one of the functions from our Ex1 repository and changed
// it so it'll fit my needs 

/// @brief splits a given string according the a given delimiter
/// then assigns the values in it to a pair where the first value 
/// is the numbers in the string and the seconds value is it's "name"

/// @param str the string to split
/// @param delimiter a delimiter to split with
/// @return a pair in the form of (vector containing the values, a string representing the name)
pair<vector<double>,string> split_string(string str, string delimiter);

/// @brief Opens and reads a given csv file and inserts the data to a given map
/// @param data The destination map the method writes to
/// @param file_name The name of the file it needs to read
void read_and_map(multimap<vector<double>,string> *data, string file_name);

/// @brief the function gets a line of doubles , assigns it as a vector and checks the size of it
/// @param vect a vector to fill with the input
/// @param vect_size - the size the vector should be
void get_vector(vector<double> &vect, int vect_size);

/// @brief Loops through the program , take end-less input
/// @param knn - a Knn object to check with - based on distance algorithm
/// @param size - the size of the object we got from the file
void loop_input(Knn *knn, int size);

/// @brief the function makes the Knn subClass that we get from the user's arguments and runs the loop_input function
/// @param k - the K for Knn algorithm
/// @param choice - the distance function the user picked
/// @param data - the vectors added from the file 
/// @param size - the size of a vector in 
std::string distance_chooser(int k, int choice, multimap<vector<double>,string> data,  vector<double> vect1); 

std::string classify(Knn *knn, vector<double> vect1);


string Knn_classify(int size_of_vector, multimap<vector<double>,string> data, vector<double> user_vector, int distance_function, int k);

bool assign_strings_to_doubles(vector<string> &str, vector<double> &doub, int size);

bool assign_strings_to_doubles_with_size(vector<string> &str, vector<double> &doub, int size, int vect_size);

void read_and_map_unclassified(vector<vector<double>> *unclassified_data, string file_name);

pair<int,string> split_k_and_distance(string str, string delimiter) ;