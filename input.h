#include <iostream>
#include <vector>
#include <cstdlib>
#include <sstream>


using namespace std;

void print_prog_start();

/// @brief checks if 2 vectors of doubles are in the same size
/// @param v1 first vector
/// @param v2  second vector
/// @return  true - same size , false - not the same size
bool is_same_size(vector<double>& v1, vector<double>& v2);

/// @brief checking if a string is a number of not
/// @param str - the string we check
/// @return 1 - is a number , 0 - not a number
bool is_a_number(string str);




/// @brief assigning a string that represents a double into a vector
/// @param str - the string
/// @param vect - the vector
void assign_number_to_vector(string str, vector<double>& vect);


/// @brief prints the vector 
/// @param vect  - the vector to print 
void print_vector(vector<double>& vect);
void print_vector(vector<string>& vect);

/// @brief splits a string into multiple strings with a delimiter of " "
/// @param str the string to split
/// @param strings a vector of string to work with
/// @param delimiter a delimiter to split with
/// @return true - success, false - else
bool split_string(string str, vector<string>& strings, string delimiter);


/// @brief inputs a single line from the user into a vector
/// @param vect - the vector we're using
/// @return  1 - success , 0 - fail
bool input_single_line(vector<double>& vect);


bool input_two_vectors(vector<double>& vector1, vector<double>& vector2);

void split_strings(string str, vector<string> &strings, string delimiter);