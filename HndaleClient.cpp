#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include <unistd.h>
#include <string.h>
#include <sstream>

#include "ex2.h"

#include "Knn.h"
#include "Knn_subClasses.h"
#include "HndaleClient.h"

using namespace std;

static map<string,int> Algorithms
{
    {"AUC",1},
    {"MAN",2},
    {"CHB",3},
    {"CAN",4},
    {"MIN",5}
};

void send_to_client(int client_sock, string massage) {
   
    int sent_bytes = send(client_sock, massage.c_str(), massage.length(), 0); // Sending the result depending on the client's input to him
            if (sent_bytes < 0)
            {
                perror("error sending to client");
            }
}

string recieve_from_client(int client_sock) {
     char buffer[4096];
            int expected_data_len = sizeof(buffer);
            memset(buffer, 0, expected_data_len); // Setting the buffer to 0's

            int read_bytes = recv(client_sock, buffer, expected_data_len, 0);
            return buffer;
}

void print_menu(int client_socket){
    string menu_massage = "Welcome to the KNN Classifier server. Please choose an option:\n1. upload an unclassified csv data file\n2. algorithm settings\n3. classify data\n4. display results\n5. download results\n8. exit\n" ;
    send_to_client(client_socket, menu_massage);
}

void upload_files(multimap<vector<double>,string> *data, vector<vector<double>> *unclassified_data, int client_sock) {
    multimap<vector<double>,string> data_temp;
    vector<vector<double>> unclassified_data_temp;
    string classified, unclassified;
    string return_massage = "Plaese upload your local train CSV file.";
    send_to_client(client_sock, return_massage.c_str());
    classified = recieve_from_client(client_sock);
    read_and_map(&data_temp, classified);
    if (data_temp.size() == 0) {
        send_to_client(client_sock, "invalid input");
        return;
    }
    return_massage = "Upload complete.\nPlaese upload your local test CSV file.";
    send_to_client(client_sock, return_massage);
    unclassified = recieve_from_client(client_sock);
    read_and_map_unclassified(&unclassified_data_temp, unclassified);
    if (unclassified_data_temp.size() == 0) {
        send_to_client(client_sock, "invalid input");
        return;
    }
    send_to_client(client_sock, "upload complete.");
    *data = data_temp;
    *unclassified_data = unclassified_data_temp;
    return;
}

void change_k_and_distance(int *k, string* distance_function, int client_sock) {
    stringstream ss;
    ss << "The current KNN parameters are: K = " << *k << ", distance metric = " << *distance_function << endl;
    string massage = ss.str();
    send_to_client(client_sock, massage);
    string user_input = recieve_from_client(client_sock);
    pair<int, string> temp;
    if (user_input.empty()) {
        return;
    } 
    temp = split_k_and_distance(user_input, " ");
    if(temp.first == 0) {
        return;
    }
    *k = temp.first;
    *distance_function = temp.second;
    return;
        
}

void classify_data(int k, string distance,  multimap<vector<double>,string> data, vector<vector<double>> unclassified_data, vector<string> *results ){
    for (vector<double> vect : unclassified_data) {
        results->push_back(Knn_classify(data.begin()->first.size(), data, vect, Algorithms[distance], k));
    }
    return;
}

void display_results(vector<string> results) {
    int i = 1;
    for (string s: results) {
        cout << i << "  " << s << endl;
        i++;
    }
    cout << "Done." << endl;
}

void menu(int client_socket) {
    int choice = 0;
    multimap<vector<double>,string> data;
    vector<vector<double>> unclassified_data;
    vector<string> results;
    int k = 5;
    string distance = "AUC";
    while(choice != 8){
        print_menu(client_socket);
        choice = stoi(recieve_from_client(client_socket));
        if (choice == 1) {
            upload_files(&data, &unclassified_data, client_socket);
            continue;
        } else if (choice == 2) {
            change_k_and_distance(&k, &distance, client_socket);
        } else if(choice == 3) {
            classify_data(k, distance, data, unclassified_data, &results);
        } else if(choice == 4) {
            if(data.empty()) {
                cout << "please upload data" << endl;
                continue;
            }
            if(results.empty()) {
                cout << "please classify the data" << endl;
                continue;
            }
            display_results(results);
        }

    }
}

