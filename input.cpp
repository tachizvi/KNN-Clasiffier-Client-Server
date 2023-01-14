#include <iostream>
#include <vector>
#include <cstdlib>
#include <sstream>
#include "input.h"

using namespace std;

/// @brief A function that prints the first lines of the main program
void print_prog_start()
{
    cout << "Hello! please insert two lines of vectors." << endl;
    cout << "The vectors shold be the same size (of instances) , and include only real numbers." << endl;
    cout << "Example for valid input :" << endl;
    cout << "-2 4 3.5 6" << endl;
    cout << "7 9 -0.3 399" << endl;
    cout << "Now you try it!" << endl;
}

/// @brief checks if 2 vectors of doubles are in the same size
/// @param v1 first vector
/// @param v2  second vector
/// @return  true - same size , false - not the same size
bool is_same_size(vector<double> &v1, vector<double> &v2)
{
    return v1.size() == v2.size();
}

/// @brief checking if a string is a number of not
/// @param str - the string we check
/// @return 1 - is a number , 0 - not a number
bool is_a_number(string str)
{
    bool has_dot = false; // flag to check there is only one dot
    for (int i = 0; i < str.length(); i++)
    {
        if ((str[i] < 48) || (str[i] > 57))
        { // checks if there is only numbers
            if (str[i] == 101 || str[i] == 69)
            {                                               // enables E and e in a number
                if ((str[i + 1] == 43 || str[i + 1] == 45)) // if there is + or - after E/e
                {
                    i++;
                    continue;
                }
            }
            if (str[i] == 45 && i == 0)
            { // checks if there is a minus and it's before the number or not
                if (str[1] == 46)
                { // in-case we have a ".-" on the input.
                    return false;
                }
                continue;
            }
            if (str[i] == 46)
            { // in-case we have a dot

                if (has_dot)
                { // in-case it's the second dot - invalid
                    return false;
                }
                has_dot = true; // turns the flag on for first dot
                continue;
            }

            return false;
        }
    }
    return true;
}

/// @brief assigning a string that represents a double into a vector
/// @param str - the string
/// @param vect - the vector
void assign_number_to_vector(string str, vector<double> &vect)
{
    double num = stod(str); // using stod function to cast a string to double
    vect.push_back(num);    // pushes the number into the vector refferenced
}

/// @brief prints the vector
/// @param vect  - the vector to print
void print_vector(vector<double> &vect)
{
    for (double i : vect)
    {
        cout << i << endl;
    }
}
void print_vector(vector<string> &vect)
{
    for (string i : vect)
    {
        cout << i << endl;
    }
}

/// @brief splits a string into multiple strings with a delimiter of " "
/// @param str the string to split
/// @param strings a vector of string to work with
/// @param delimiter a delimiter to split with
/// @return true - success, false - else
bool split_string(string str, vector<string> &strings, string delimiter)
{
    int start = 0;
    int end = str.find(delimiter);
    string temp;
    while (end != -1)
    { // loops until we dont have any more spaces in string
        temp = str.substr(start, end - start);
        if (!is_a_number(temp) || temp.empty())
        {
            return false;
        }
        strings.push_back(temp);
        start = end + delimiter.size();
        end = str.find(delimiter, start);
    }
    temp = str.substr(start, end - start);
    if (!is_a_number(temp) || temp.empty())
    {
        return false;
    }
    strings.push_back(temp);
    return true;
}

void split_strings(string str, vector<string> &strings, string delimiter)
{
    int start = 0;
    int end = str.find(delimiter);
    string temp;
    while (end != -1)
    { // loops until we dont have any more spaces in string
        temp = str.substr(start, end - start);
        strings.push_back(temp);
        start = end + delimiter.size();
        end = str.find(delimiter, start);
    }
    temp = str.substr(start, end - start);
    strings.push_back(temp);
    
}

/// @brief inputs a single line from the user into a vector
/// @param vect - the vector we're using
/// @return  1 - success , 0 - fail
bool input_single_line(vector<double> &vect)
{
    double num;
    string str;
    vector<string> strings;
    getline(cin, str);
    if (!split_string(str, strings, " "))
    {
        return false;
    }
    for (string s : strings)
    {
        assign_number_to_vector(s, vect);
    }
    return true;
}

/// @brief getting two lines of vectors using input_single_line function
/// @param vector1 - a refference to the first vector
/// @param vector2 - a refference to the second vector
/// @return 1 - valid input , 0 - else
bool input_two_vectors(vector<double> &vector1, vector<double> &vector2)
{

    if (!input_single_line(vector1))
    {
        cout << "vector number 1 is invalid!" << endl;
        return false;
    }
    if (!input_single_line(vector2))
    {
        cout << "Vector number 2 is invalid!" << endl;
        return false;
    }

    if (vector1.size() != vector2.size())
    {
        cout << "Vectors are not on the same size!" << endl;
        return false;
    }
    return true;
}

/// @brief inputs a single line from the user into a vector
/// @param vect - the vector we're using
/// @param str - using the substring of numbers got from user
/// @return  1 - success , 0 - fail
bool input_single_line(vector<double> &vect, string str)
{
    double num;
    vector<string> strings;

    if (!split_string(str, strings, " "))
    {
        return false;
    }
    for (string s : strings)
    {
        assign_number_to_vector(s, vect);
    }
    return true;
}
