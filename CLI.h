#pragma once
#ifndef CLI_H
#define CLI_H
#include "SocketIO.h"
#include <map>
#include <stdio.h>
#include <vector>
#include "ex2.h"
#include "Command.h"

class CLI_Server
{
private:
    int *socket;
    multimap<vector<double>, string> *data;
    vector<vector<double>> *unclassified_data;
    int *k;
    string *distance_function;
    vector<string> *results;
    Command commands[4];
    SocketIO dio;

public:
    CLI_Server(int *socket);

    void execute();
    void print_menu();
};

class CLI_Client {
    private:
    int *Socket;
    SocketIO dio;
    Command commands[4];

    public:
    CLI_Client(int *socket) ;

    bool exectue(int user_pick);
    void read_menu();
};

#endif