#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include <unistd.h>
#include <string.h>
#include "input.h"
#include "ex2.h"
#include "HndaleClient.h"
#include "CLI.h"

using namespace std;

void *handle_client(void *v)
{
    int client_sock = *((int *)v);
    free(v);
    CLI_Server cli_server = CLI_Server(&client_sock);
    int client_on = 1;
    while (client_on)
    {
        cli_server.print_menu();
        cli_server.execute(&client_on);
    }
    return NULL;
}
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
    string file_name = argv[1];
    multimap<vector<double>, string> data; // initializing the map the data will be written to.

    // using the function from the last exercise in order to
    // read the given file and map the data in it
    // read_and_map(&data, file_name);

    const int server_port = stoi(argv[1]);
    int sock = socket(AF_INET, SOCK_STREAM, 0); // Creating a socket to bind to
    if (sock < 0)
    {
        perror("error creating socket");
    }
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
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

    while (1)
    {

        string result;
        struct sockaddr_in client_sin;
        unsigned int addr_len = sizeof(client_sin);
        pthread_t thread_o;
        int client_sock = accept(sock, (struct sockaddr *)&client_sin, &addr_len); // Accepting a new client
        if (client_sock < 0)
        {
            perror("error accepting client");
            continue;
        }
        int *sclient = (int *)std::malloc(sizeof(int));
        *sclient = client_sock; // storing the client's socket
        pthread_create(&thread_o, NULL, handle_client, (void *)(sclient));
      
    }
    close(sock);
    return 0;
}
