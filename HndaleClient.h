#include <iostream>
#include <iostream>
#include "ex2.h"

#include "Knn.h"
#include "Knn_subClasses.h"

void menu(int client_socket);

void print_menu(int client_sock);

void upload_files(multimap<vector<double>,string> *data, vector<vector<double>> *unclassified_data);

void change_k_and_distance(int *k, string* distance_function) ;

void classify_data(int k, string distance,  multimap<vector<double>,string> data, vector<vector<double>> unclassified_data, vector<string> *results );

void display_results(vector<string> results);