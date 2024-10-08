#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>  
#include <sstream>
#include <map>
#include "input.h"
#include "Knn.h"
#include "Knn_subClasses.h"

using namespace std;

bool isNumber(string s)
{
    for (int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == false && s[i] != '.')
            return false;

    return true;
}



 static map<string,int> Algorithms
{
    {"AUC",1},
    {"MAN",2},
    {"CHB",3},
    {"CAN",4},
    {"MIN",5}
};
// This map is mapping each of the distance-calculating
// algorithms to a number (to use a switch-case later on) 



// Here i took one of the functions from our Ex1 repository and changed
// it so it'll fit my needs 

/// @brief splits a given string according the a given delimiter
/// then assigns the values in it to a pair where the first value 
/// is the numbers in the string and the seconds value is it's "name"

/// @param str the string to split
/// @param delimiter a delimiter to split with
/// @return a pair in the form of (vector containing the values, a string representing the name)
pair<vector<double>,string> split_string(string str, string delimiter) {
    if (str.empty()) {
        return {{0}, "ERROR"};
    }
    int start = 0;
    int end = str.find(delimiter);
    string temp;
    vector<double> v;
    while (end != -1) { // loops until we don't have any more instances of the delimiter in string
        temp = str.substr(start, end - start);
        if (!is_a_number(temp) || temp.empty()) {
             return {{0}, "ERROR"};
        }
        v.push_back(stod(temp));
        start = end + delimiter.size();
        end = str.find(delimiter, start);
    }
    temp = str.substr(start, end);
        if (is_a_number(temp) || temp.empty()) {
             return {{0}, "ERROR"};
        };
    return {v,temp};
}

/// @brief splits a string to k and a metric
/// @param str the string to split
/// @param delimiter 
/// @return a pair of a integer and a string {k, metric}
pair<int,string> split_k_and_distance(string str, string delimiter) {
    bool valid_k = true;
    int start = 0;
    int end = str.find(delimiter);
    string distacne_metric, k_str;
    int k;
    k_str = str.substr(start, end - start);
    start = end + 1; // delimiter.size();
    //end = str.find(delimiter);
    distacne_metric = str.substr(start, str.length() - 1);
    if (is_a_number(k_str)) {
        k = stod(k_str);
    } else {
        valid_k = false;
        
    }
    if (k < 1) {
        valid_k = false;
        
    }
    if (!strcmp(distacne_metric.c_str(), "AUC") || !strcmp(distacne_metric.c_str(), "MIN")|| !strcmp(distacne_metric.c_str(), "MAN") || !strcmp(distacne_metric.c_str(), "CHB") || !strcmp(distacne_metric.c_str(), "CAN")) {
        if (valid_k) {
        return {k, distacne_metric};
        }
        return {0," "};
    } else {
        if(valid_k) {
        
        return {-1, " "};
        } else {
            return {-2, " "};
        }

    }
    
}


/// @brief splits an unclassified file into a vector of doubles
/// @param str - a line
/// @param delimiter - the split delimiter
/// @return a vector of doubles that contains the line's numbers.
vector<double> split_string_doubles(string str, string delimiter) {
    int start = 0;
    int end = str.find(delimiter);
    string temp;
    vector<double> v;
    while (end != -1) { // loops until we don't have any more instances of the delimiter in string
        temp = str.substr(start, end - start);
        if (!is_a_number(temp) || temp.empty()) {
            return v;
        }
        v.push_back(stod(temp));
        start = end + delimiter.size();
        end = str.find(delimiter, start);
    }
    temp = str.substr(start, end);
    double d = atof(temp.c_str());
    if (d == 0 && temp.compare(to_string(d)) != 0) {
        v.clear();
        return v;
    }
     v.push_back(d);
    return v;
}


/// @brief Opens and reads a given csv file and inserts the data to a given vector
/// @param The destination map the method writes to
/// @param file_name The name of the file it needs to read
bool read_and_map_unclassified(vector<vector<double>> *unclassified_data, string file_name) {
    ifstream fin; // initialize a stream
    string line;
    fin.open(file_name); // Open an existing file
    int position = 0;
    // while there still lines to read, continue reading
     while(getline(fin, line)){  
    // inserts the pair returned from the "split_string" function to the map 
            vector<double> v = split_string_doubles(line, ",");
            if (v.size() == 0) {
                return false;
            }
            unclassified_data->push_back(v);
            //unclassified_data[position] = split_string_doubles(line, ",");
            
            if(unclassified_data->begin()->size() == 0) {
                return false;
            }
            }
            return true;
}

/// @brief Opens and reads a given csv file and inserts the data to a given map
/// @param data The destination map the method writes to
/// @param file_name The name of the file it needs to read
bool read_and_map(multimap<vector<double>,string> *data, string file_name)
{
    ifstream fin; // initialize a stream
    string line;
    fin.open(file_name); // Open an existing file
    // while there still lines to read, continue reading
     while(getline(fin, line)){  
    // inserts the pair returned from the "split_string" function to the map 
    pair<vector<double>,string> temp_pair = split_string(line,",");
    if (temp_pair.second.compare("ERROR") == 0) {
        return false;
    }
            data->insert(temp_pair);
            }
            return true;
    }

/// @brief the function gets a line of doubles , assigns it as a vector and checks the size of it
/// @param vect a vector to fill with the input
/// @param vect_size - the size the vector should be
void get_vector(vector<double> &vect, int vect_size) {
    input_single_line(vect);
    if(vect.size() != vect_size) {  // in case they don't have the same size - error

        exit(1);
    }

}

/// @brief gets a vector of string and converts it to doubles
/// @param str a vector of strings
/// @param doub a pointer to an empty vector of doubles
/// @param size - the size of a valid vector
/// @return false - wasn't valid , true - valid
bool assign_strings_to_doubles(vector<string> &str, vector<double> &doub, int size) {
    if (str.size() != size) {
        return false;
    }
    for (string s: str) {
        if (!is_a_number(s)) {
            return false;
        }
        assign_number_to_vector(s, doub);     
    }
    return true;
}

/// @brief gets a vector of string and converts it to doubles
/// @param str a vector of strings
/// @param doub a pointer to an empty vector of doubles
/// @param size - the size of a valid vector
/// @return false - wasn't valid , true - valid
bool assign_strings_to_doubles_with_size(vector<string> &str, vector<double> &doub, int size, int vect_size) {
    if (vect_size != size) {
        return false;
    }
    for (string s: str) {
        
        if(vect_size == 0){
            return true;
        }
        if (!is_a_number(s)) {
            return false;
        }
        assign_number_to_vector(s, doub);  
        vect_size--;   
    }
    return true;
}


/* /// @brief Loops through the program , take end-less input
/// @param knn - a Knn object to check with - based on distance algorithm
/// @param size - the size of the object we got from the file
void loop_input(Knn *knn, int size){
    while (true) {
            vector<double> vect1;
            get_vector(vect1, size);    // assugn vector
            string type = knn->get_type(vect1);     // get the type of the object
            cout << type << endl;       // print the type 
            vect1.clear();      // clears the vector for another use
        }
} */

/// @brief returns the knn get_type function based on the Knn object
/// @param knn - a Knn object
/// @param vect1 - the user vector
/// @return the type of the vector
std::string classify(Knn *knn, vector<double> vect1) {
    string type = knn->get_type(vect1);     // get the type of the object
    return type;
}

/// @brief the function makes the Knn subClass that we get from the user's arguments and runs the loop_input function
/// @param k - the K for Knn algorithm
/// @param choice - the distance function the user picked
/// @param data - the vectors added from the file 
/// @param size - the size of a vector in 
/// @param vect1 - a vector that came from the client to check to
std::string distance_chooser(int k, int choice, multimap<vector<double>,string> data,  vector<double> vect1) {
    if (choice == 1){       // Oclidic case
         Knn_AUC knn = Knn_AUC(k, data);
        return classify(&knn, vect1);
    } else if (choice == 2){    // MAN case
         Knn_MAN knn = Knn_MAN(k, data);
        return classify(&knn, vect1);
    } else if (choice == 3){    // CHB case
         Knn_CHB knn = Knn_CHB(k, data);
        return classify(&knn, vect1);
    } else if (choice == 4) {   // CAN case
         Knn_CAN knn = Knn_CAN(k, data);
        return classify(&knn, vect1);
    } else if (choice == 5) {   // MIN case
         Knn_MIN knn = Knn_MIN(k, data);
        return classify(&knn,  vect1);
    } else {    // in case it's not one of the valid input
        return "invalid input";
    }
}




///@brief runs the classify based on the specific vector
///@param 
string Knn_classify(int size_of_vector, multimap<vector<double>,string> data, vector<double> user_vector, int distance_function, int k)
{
    if ( k < 1 || k > data.size()) {
        return "invalid input";
    }
    return distance_chooser(k, distance_function, data, user_vector); // calling the function that decides which distance-calculating algorithms to use
}


