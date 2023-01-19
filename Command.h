#pragma once

#ifndef COMMAND_H
#define COMMAND_H
#include "DefaultIO.h"
#include <map>
#include <stdio.h>
#include <vector>
#include "ex2.h"

using namespace std;
class Command
{
protected:
    
    string dsecription;
    DefaultIO dio;

public:
    Command();
    void exectue() ;
    string get_description() ;
};

class Command_Upload : public Command
{
private:
    multimap<vector<double>, string> *data;
    vector<vector<double>> *unclassified_data;

public:
    Command_Upload(int *socket, multimap<vector<double>, string> *data, vector<vector<double>> *unclassified_data);
    void execute();
};

class Command_settings : public Command
{
private:
    int *k;
    string *distance_function;

public:
    Command_settings(int *socket, int *k, string *distance_function);

    void execute();
};

class Command_classify : public Command
{
private:
    int *k;
    string *distance;
    multimap<vector<double>, string> *data;
    vector<vector<double>> *unclassified_data;
    vector<string> *results;

public:
    Command_classify(int *socket, int *k, string *distance, multimap<vector<double>, string> *data, vector<vector<double>> *unclassified_data, vector<string> *results);

    void execute();
};

class Command_display : public Command
{
private:
    vector<string> *results;
    multimap<vector<double>, string> *data;

public:
    Command_display(int *socket, vector<string> *results, multimap<vector<double>, string> *data);
    void execute();
};

class Command_client_Upload : public Command {
    public:
    Command_client_Upload(int *socket);
    void execute();
};

class Command_Client_Settings : public Command {
    public:
    Command_Client_Settings(int *socket);
    void execute() ;
};

class Command_Client_Classify : public Command {
    public:
    Command_Client_Classify(int *socket);

    void execute();
};

class Command_Client_Display : public Command {
    public:
    Command_Client_Display(int *socket);
    
    void execute() ;
};
#endif