#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <pthread.h>
#include "input.h"
#include "ex2.h"

using namespace std;

/// @brief the function dissects the input of the client, finds and collects the data we need
/// @param input string given by the user
/// @param k a point to an int representing k, we'll change it accordingly
/// @return {-1,-1} in case of an invalid input, {The k value the user inputs ,distance_algorithm chosen} in case of a valid input
pair<int, int> is_valid(string input, int *k, vector<string> &vect)
{
    split_strings(input, vect, " "); // Filling the vector with the elements from the client's input
    int distance;
    int place = 0;

    // in case the input of the user is too short, it's an invalid input
    if (vect.size() < 3)
    {
        return {-1, -1};
    }

    string end = vect[vect.size() - 1];
    std::cout << end << endl;
    string dis_func = vect[vect.size() - 2];

    // Checking if the last element in the input is a nubmer, if so we'll convert it to an int,
    // otherwise we shall return that the input is invalid
    if (!is_a_number(end))
    {
        return {-1, -1};
    }

    place = stoi(end); // Converting K (from "The closest K elements... etc.") to an int

    //  checks if distance algorithm name appears in the input
    if (!dis_func.compare("AUC"))
    {
        distance = 1;
        goto found;
    }
    if (!dis_func.compare("MAN"))
    {
        distance = 2;
        goto found;
    }
    if (!dis_func.compare("CHB"))
    {
        distance = 3;
        goto found;
    }
    if (!dis_func.compare("CAN"))
    {
        distance = 4;
        goto found;
    }
    if (!dis_func.compare("MIN"))
    {
        distance = 5;
        goto found;
    }

    // if none of the distance algorithms were found in the input (and we didn't goto found),
    // it's invalid
    return {-1, -1};
found:
    return {place, distance};
}

/// @brief This function recives the file name the user sent us and maps the data from the file
/// @param data The map where our mapped data will be stored
/// @param client_sock // the client's socket
void accept_file_name(multimap<vector<double>, string> *data, int client_sock)
{
    char buffer[4096];
    int expected_data_len = sizeof(buffer);
    std::memset(buffer, 0, expected_data_len); // Setting the buffer to 0's

    int read_bytes = recv(client_sock, buffer, expected_data_len, 0);

    // Reciving a message from the client

    if (read_bytes == 0)
    {
        std::cout << "Connection closed" << endl;
    }
    else if (read_bytes < 0)
    {
        std::cout << "Empty input" << endl;
    }
    else
    {
        read_and_map(data, buffer);
    }
    string message = "Name accepted and mapped";
    int sent_bytes = send(client_sock, message.c_str(), sizeof(message), 0); // Telling the client we succsessfully accepeted the name sent
    if (sent_bytes < 0)
    {
        perror("error sending to client");
    }
}

/// @brief this function handles each of the clients connected to the server
/// @param cs a pointer to an int representing the client's socket
/// @return 
void *handle_client(void *cs)
{
    int client_sock = *((int *)cs); // saving the client's socket as a local variable
    free(cs);   // freeing the non longer needed varibale
    multimap<vector<double>, string> data; // initializing the map the data will be written to.
    string file_name;
    string result;

    int clientOn = 1;
    string message = "Please enter a file name:";
    int sent_bytes = send(client_sock, message.c_str(), sizeof(message), 0); // Asking the client to sent us a file's name
    if (sent_bytes < 0)
    {
        perror("error sending to client");
    }

    accept_file_name(&data, client_sock);
    while (clientOn)
    {
        char buffer[4096];
        int expected_data_len = sizeof(buffer);
        std::memset(buffer, 0, expected_data_len); // Setting the buffer to 0's
        int read_bytes = recv(client_sock, buffer, expected_data_len, 0);
        // Reciving a message from the client

        if (read_bytes == 0)
        {
            std::cout << "Connection closed" << endl;
        }
        else if (read_bytes < 0)
        {
            std::cout << "Empty input" << endl;
            continue;
        }
        else
        {
            if (!strcmp(buffer, "-1")) // If the message we recived is "-1". The client wants to close the connection
            {
                result = "Bye"; // We shall return "Bye" to the client so it'll close it's connection
                clientOn = 0;
                goto send_to_client;
            }
            int *k;
            vector<double> user_vector;
            vector<string> user_string;
            pair<int, int> place_and_distance = is_valid(buffer, k, user_string);
            vector<string> vector_of_string;

            if (place_and_distance.first == -1) // If is_valid returned -1, it means the input is invalid
            {
                result = "invalid input";
                goto send_to_client;
            }
            string buffer_string = buffer;
            int vect_size = user_string.size() - 2;

            if (!assign_strings_to_doubles_with_size(user_string, user_vector, data.begin()->first.size(), vect_size))
            {
                // if the function returned false, it's invalid input
                result = "invalid input";
                goto send_to_client;
            }
            result = Knn_classify(data.begin()->first.size(), data, user_vector, place_and_distance.second, place_and_distance.first);
            strcpy(buffer, result.c_str());
        }

    // This label it used to send back a message to the client, we shall get here once we know what to send back
    send_to_client:
        strcpy(buffer, result.c_str());
        //   std::cout << buffer << endl;
        int sent_bytes = send(client_sock, buffer, sizeof(buffer), 0); // Sending the result depending on the client's input to him
        if (sent_bytes < 0)
        {
            perror("error sending to client");
        }
    }
    return 0;
}

/// @brief The main function of server firstly opens and reads a file, then stores the data in multi-map for further usage
/// Afterwards it creates a socket and connects to it, then it awaits for a client to connect.
/// once a client is connected, it recives a message from him, dissects it and finds the closest vector to his input
/// finally it returns what it found to the client (or a diffrent message in some situations), then it awaits for the next message/client

/// @param argc
/// @param argv[0] = csv file path (relative or absolute path)
/// @param argv[1] = port number that the server will listen to
/// @return
int main(int argc, char const *argv[])
{

    const int server_port = stoi(argv[2]);
    int sock;

    while (1)
    {
        int sock = socket(AF_INET, SOCK_STREAM, 0); // Creating a socket to bind to
        if (sock < 0)
        {
            perror("error creating socket");
        }
        struct sockaddr_in sin;
        std::memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = INADDR_ANY;
        sin.sin_port = htons(server_port);

        // Binding to the socket
        if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0)
        {
            perror("error binding socket");
        }
        if (listen(sock, 5) < 0)
        {
            perror("error listening to a socket");
        }
        string result;
        struct sockaddr_in client_sin;
        unsigned int addr_len = sizeof(client_sin);

        int i = 0;

        while (1)
        {
            pthread_t thread_o;
            int client_socket = accept(sock, (struct sockaddr *)&client_sin, &addr_len); // Accepting a new client
            if (client_socket < 0)
            {
                perror("error accepting client");
                return -1;
            }
            int *sclient = (int *)std::malloc(sizeof(int)); // Creating a varibale on the heap that'll store the client's socket 
            *sclient = client_socket; // storing the client's socket 
            pthread_create(&thread_o, NULL, handle_client, (void *)(sclient));
        }
    }
    close(sock);
    return 0;
}
