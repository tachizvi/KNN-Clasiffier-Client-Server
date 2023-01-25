#include "SocketIO.h"
#include <map>
#include <stdio.h>
#include <vector>
#include "ex2.h"
#include "Command.h"
#include <iostream>
#include <fstream>

using namespace std;

static bool is_vectors_same_size(vector<double> v1, vector<double> v2) {
    if(v1.capacity() == v2.capacity()) 
        return true;
    return false;
}

void *downloader(void *args)
{
    Command_Client_Download *cld = ((Command_Client_Download *)(args));
    cld->download_data();
    return NULL;
}

static void copy_vector(vector<vector<double>> src, vector<vector<double>> *dst)
{
    dst->clear();
    vector<vector<double>>::iterator it;

    for (vector<double> v : src)
    {
        dst->push_back(v);
    }
}

static void copy_vector(multimap<vector<double>, string> src, multimap<vector<double>, string> *dst)
{
    dst->clear();
    multimap<vector<double>, string>::iterator it;

    for (it = src.begin(); it != src.end(); it++)
    {
        vector<double> temp_vector = it->first;
        string temp_string = it->second;

        dst->insert({temp_vector, temp_string});
    }
}

static map<string, int> Algorithms{
    {"AUC", 1},
    {"MAN", 2},
    {"CHB", 3},
    {"CAN", 4},
    {"MIN", 5}};

Command::Command()
{
    return;
}

// void Command::exectue(){}

string Command::get_description()
{
    return this->dsecription;
}

bool Command::data_empty()
{
    return (this->data)->empty();
}

bool Command::result_empty()
{
    return this->results->empty();
}
Command_Upload::Command_Upload(int *socket, multimap<vector<double>, string> *data, vector<vector<double>> *unclassified_data)
{
    this->dsecription = "1. upload an unclassified csv data file";
    this->dio = SocketIO(socket);
    this->data = data;
    this->unclassified_data = unclassified_data;
}
void Command_Upload::execute()
{
    multimap<vector<double>, string> data_temp;
    vector<vector<double>> unclassified_data_temp;
    string classified, unclassified;
    string return_message = "Please upload your local train CSV file.";
    this->dio.write(return_message);
    classified = this->dio.read();
    if(!read_and_map(&data_temp, classified)) {
        this->dio.write("invalid input");
        return;
    }
    if (data_temp.size() == 0)
    {
        this->dio.write("invalid input");
        return;
    }
    return_message = "Upload complete.\nPlease upload your local test CSV file.";
    this->dio.write(return_message);
    unclassified = this->dio.read();
    if(!read_and_map_unclassified(&unclassified_data_temp, unclassified)) {
        this->dio.write("invalid input");
        return;
    }
    if (unclassified_data_temp.size() == 0)
    {
        this->dio.write("invalid input");
        return;
    }
    vector<double> classified_vector = data_temp.begin()->first;
    vector<double> unClassified_vector = *unclassified_data_temp.begin();
    if (!is_vectors_same_size(classified_vector, unClassified_vector)) {
         this->dio.write("invalid input");
        return;
    }
    this->dio.write("upload complete.");

    copy_vector(data_temp, this->data);

    //*(this->data) = data_temp; // THE PROBLEM IS HERE
    //(this->unclassified_data) = &unclassified_data_temp; // FIX IT U FKING DIPSHIT
    copy_vector(unclassified_data_temp, this->unclassified_data);
    return;
}

Command_settings::Command_settings(int *socket, int *k, string *distance_function)
{
    this->dsecription = "2. algorithm settings";
    this->dio = SocketIO(socket);
    this->k = k;
    this->distance_function = distance_function;
}

void Command_settings::execute()
{
    stringstream ss;
    stringstream ss_error;
    ss << "The current KNN parameters are: K = " << *k << ", distance metric = " << *distance_function << endl;
    string message = ss.str();
    this->dio.write(message);
    string user_input = this->dio.read();

    pair<int, string> temp;

    if (user_input.length() == 0)
    {
        this->dio.write("ignore");
        return;
    }
    //
    temp = split_k_and_distance(user_input, " ");
    if (temp.first == 0)
    {
        ss_error << "invalid value for K";
        message = ss_error.str();
        this->dio.write(message);
        return;
    }
    else if (temp.first == -1)
    {
        ss_error << "invalid value for metric";
        message = ss_error.str();
        this->dio.write(message);
        return;
    }
    else if (temp.first == -2)
    {
        ss_error << "invalid value for K\n"
                 << "invalid value for metric";
        message = ss_error.str();
        this->dio.write(message);
        return;
    }
    this->dio.write(" ");
    *k = temp.first;
    *distance_function = temp.second;
    return;
}

Command_classify::Command_classify(int *socket, int *k, string *distance, multimap<vector<double>, string> *data, vector<vector<double>> *unclassified_data, vector<string> *results)
{
    this->dsecription = "3. classify data";
    this->dio = SocketIO(socket);
    this->k = k;
    this->distance = distance;
    (this->data) = data;
    (this->unclassified_data) = unclassified_data;
    this->results = results;
}

void Command_classify::execute()
{

    if (data_empty())
    {
        this->dio.write("please upload data");
        this->dio.write("classifying data complete");
    }
    results->clear();
    for (vector<double> vect : *unclassified_data)
    {
        results->push_back(Knn_classify((data)->begin()->first.size(), *data, vect, Algorithms[*distance], *k));
    }
    this->dio.write("classifying data complete");

    return;
}

Command_display::Command_display(int *socket, vector<string> *results, multimap<vector<double>, string> *data)
{
    this->dsecription = "4. display results";
    this->dio = SocketIO(socket);
    this->results = results;
    this->data = data;
}

void Command_display::execute()
{
    if (this->data->size() == 0)
    {
        this->dio.write("please upload data");
        this->dio.read();
        return;
    }

    if (this->results->size() == 0)
    {
        this->dio.write("please classify the data");
        this->dio.read();
        return;
    }
    int i = 1;
    this->dio.write("1");
    this->dio.read();
    for (string s : *results)
    {

        stringstream ss;
        string message;
        ss << i << " " << s;

        message = ss.str();
        this->dio.write(message);
        this->dio.read();
        i++;
    }
    this->dio.write("Done.");
    this->dio.read();
    return;
}

Command_download::Command_download(int *socket, vector<string> *results, multimap<vector<double>, string> *data)
{
    this->dsecription = "5. download results";
    this->dio = SocketIO(socket);
    this->results = results;
    this->data = data;
}

void Command_download::execute()
{
    if (data_empty())
    {
        this->dio.write("please upload data");
        this->dio.read();
        return;
    }

    if (result_empty())
    {
        this->dio.write("please classify the data");
        this->dio.read();
        return;
    }
    this->dio.write("Please input a file path");
    this->dio.read();

    this->dio.write("ok");
    this->dio.read();
    int i = 1;
    for (string s : *results)
    {

        stringstream ss;
        string message;
        ss << i << " " << s;

        message = ss.str();
        this->dio.write(message);
        this->dio.read();
        i++;
    }
    this->dio.write("Done.");
    this->dio.read();
}

;

Command_client_Upload::Command_client_Upload(int *socket)
{
    this->dio = SocketIO(socket);
}

void Command_client_Upload::execute()
{
    string message_recieved;
    cout << this->dio.read() << endl;
    string path, upath;
    cin >> path;
    this->dio.write(path);
    message_recieved = this->dio.read();
    cout << message_recieved << endl;
    if(message_recieved.compare("invalid input") == 0) {
        return;
    }
    cin >> upath;
    this->dio.write(upath);
    cout << this->dio.read() << endl;
    return;
}

Command_Client_Settings ::Command_Client_Settings(int *socket)
{
    this->dio = SocketIO(socket);
}
void Command_Client_Settings ::execute()
{
    cout << this->dio.read() << endl;
    cin.ignore();
    string recieved_message, message;
    getline(std::cin, message);

    //
    this->dio.write(message);
    recieved_message = this->dio.read();
    if (strcmp(recieved_message.c_str(), "ignore") == 0)
    {
        return;
    }

    if (recieved_message.length() > 1)
    {
        cout << recieved_message << endl;
    }
    return;
}

Command_Client_Classify::Command_Client_Classify(int *socket)
{
    this->dio = SocketIO(socket);
}

void Command_Client_Classify::execute()
{
    cout << this->dio.read() << endl;
    return;
}

Command_Client_Display::Command_Client_Display(int *socket)
{
    this->dio = SocketIO(socket);
}

void Command_Client_Display::execute()
{
    string message_recieved;
    message_recieved = this->dio.read();
    if (strcmp(message_recieved.c_str(), "please upload data") == 0 || strcmp(message_recieved.c_str(), "please classify the data") == 0)
    {
        cout << message_recieved << endl;
        this->dio.write("OK");
        return;
    }
    this->dio.write("OK");
    cout << "" << endl;
    while (true)
    {
        message_recieved = this->dio.read();
        if (strcmp(message_recieved.c_str(), "Done.") == 0)
        {
            this->dio.write("OK");
            return;
        }
        cout << message_recieved << endl;
        this->dio.write("ok");
    }
}
Command_Client_Download::Command_Client_Download(int *socket)
{
    this->dio = SocketIO(socket);
}
void Command_Client_Download::execute()
{
    string message_recieved;
    message_recieved = this->dio.read();
    if (strcmp(message_recieved.c_str(), "please upload data") == 0 || strcmp(message_recieved.c_str(), "please classify the data") == 0)
    {
        cout << message_recieved << endl;
        cout << "" << endl; // used to stall the code, in order to keep things syncronized
        this->dio.write("OK");
        return;
    }
    cout << message_recieved << endl;

    this->dio.write("ok");

    cin.ignore();
    getline(cin, file_path);
    this->dio.write(file_path);
    this->data_to_write << recieve_data().str();

    pthread_t thread_o;
    pthread_create(&thread_o, NULL, downloader, (void *)this);

    return;
}

stringstream Command_Client_Download::recieve_data()
{
    stringstream ss;
    while (true)
    {
        string message = dio.read();
        if (strcmp(message.c_str(), "Done.") == 0)
        {
            this->dio.write("ok");
            return ss;
        }
        ss << message << "\n";
        this->dio.write("ok");
    }
}
void Command_Client_Download::download_data()
{
    ofstream MyFile(this->file_path);
    MyFile << this->data_to_write.str();
    MyFile.close();
}
