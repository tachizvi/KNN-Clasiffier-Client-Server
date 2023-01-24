#pragma once

#ifndef COMMAND_H
#define COMMAND_H
#include "SocketIO.h"
#include <map>
#include <stdio.h>
#include <vector>
#include "ex2.h"

using namespace std;
class Command
{
protected:
     string dsecription;
     SocketIO dio;
     multimap<vector<double>, string> *data ;
     vector<vector<double>> *unclassified_data ;
     int *k;
     string *distance_function;
     vector<string> *results;
     string *distance;
     string client_path;
     stringstream data_to_write;

public:
    Command();
    virtual void execute() = 0;
    string get_description();
    bool data_empty();
    bool result_empty();
    virtual ~Command() = default;
};

class Command_Upload : public Command
{

public:
    Command_Upload(int *socket, multimap<vector<double>, string> *data, vector<vector<double>> *unclassified_data);
    void execute();
};

class Command_settings : public Command
{

public:
    Command_settings(int *socket, int *k, string *distance_function);

    void execute() override;
};

class Command_classify : public Command
{

public:
    Command_classify(int *socket, int *k, string *distance, multimap<vector<double>, string> *data, vector<vector<double>> *unclassified_data, vector<string> *results);

    void execute() override;
};

class Command_display : public Command
{
private:
    vector<string> *results;
    multimap<vector<double>, string> *data;

public:
    Command_display(int *socket, vector<string> *results, multimap<vector<double>, string> *data);
    void execute() override;
};

class Command_download : public Command
{
public:
    Command_download(int *socket, vector<string> *results, multimap<vector<double>, string> *data);
    void execute() override;
};
class Command_client_Upload : public Command
{
public:
    Command_client_Upload(int *socket);
    void execute() override;
};

class Command_Client_Settings : public Command
{
public:
    Command_Client_Settings(int *socket);
    void execute() override;
};

class Command_Client_Classify : public Command
{
public:
    Command_Client_Classify(int *socket);

    void execute() override;
};

class Command_Client_Display : public Command
{
public:
    Command_Client_Display(int *socket);

    void execute() override;
};

class Command_Client_Download : public Command
{
    string file_path;
public:
    Command_Client_Download(int *socket);
    void execute() override;
    stringstream recieve_data();
    void download_data();
};
#endif