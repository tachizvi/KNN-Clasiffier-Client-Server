#include "SocketIO.h"
#include <map>
#include <stdio.h>
#include <vector>
#include "ex2.h"
#include "Command.h"

using namespace std;



static void copy_vector(vector<vector<double>> src, vector<vector<double>> *dst) {
    dst->clear();
    vector<vector<double>>::iterator it;

for ( vector<double> v : src) {
    print_vector(v);
    dst->push_back(v);
}

}

static void copy_vector(multimap<vector<double>, string> src, multimap<vector<double>, string> *dst) {
    dst->clear();
    cout << "cleared" << endl;
    multimap<vector<double>, string>::iterator it;

for (it = src.begin(); it != src.end(); it++)
{
    vector<double> temp_vector = it ->first;
    print_vector(temp_vector);
    string temp_string = it ->second;
    cout << temp_string << endl;
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

bool Command::data_empty(){
    cout << "HEY" << endl;
    return (this->data)->empty();
}

bool Command::result_empty(){
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
    string return_massage = "Please upload your local train CSV file.";
    this->dio.write(return_massage);
    classified = this->dio.read();
    read_and_map(&data_temp, classified);
    if (data_temp.size() == 0)
    {
        this->dio.write("invalid input");
        return;
    }
    return_massage = "Upload complete.\nPlaese upload your local test CSV file.";
    this->dio.write(return_massage);
    unclassified = this->dio.read();
    read_and_map_unclassified(&unclassified_data_temp, unclassified);
    if (unclassified_data_temp.size() == 0)
    {
        this->dio.write("invalid input");
        return;
    }
    this->dio.write("upload complete.");
        cout << "S" << endl;

    copy_vector(data_temp, this->data);
    //*(this->data) = data_temp; // THE PROBLEM IS HERE
    //(this->unclassified_data) = &unclassified_data_temp; // FIX IT U FKING DIPSHIT
    copy_vector(unclassified_data_temp, this->unclassified_data);
    cout << "S" << endl;
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
    string massage = ss.str();
    this->dio.write(massage);
    string user_input = this->dio.read();
    pair<int, string> temp;
    if (user_input.empty())
    {
        return;
    }
    temp = split_k_and_distance(user_input, " ");
    if (temp.first == 0)
    {
        ss_error << "invalid value for K";
        massage = ss_error.str();
        this->dio.write(massage);
        return;
    }
    else if (temp.first == -1)
    {
        ss_error << "invalid value for metric";
        massage = ss_error.str();
        this->dio.write(massage);
        return;
    }
    else if (temp.first == -2)
    {
        ss_error << "invalid value for K\n"
                 << "invalid value for metric";
        massage = ss_error.str();
        this->dio.write(massage);
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
        this->dio.write("classifying data complete");
    }
    for (vector<double> vect : *unclassified_data)
    {
        results->push_back(Knn_classify((data)->begin()->first.size(), *data, vect, Algorithms[*distance], *k));
    }
    this->dio.write("please upload data");
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
    cout << " Nah"<< endl;

    if (data_empty())
    {
        this->dio.write("please upload data");
        this->dio.read();
        return;
    }
        cout << " Nah"<< endl;

    if (result_empty())
    {
        this->dio.write("please classify the data");
        this->dio.read();
    }
    int i = 1;
    for (string s : *results)
    {
            cout << " Nah"<< endl;

        stringstream ss;
        string massage;
        ss << i << " " << s;
            cout << " Nah"<< endl;

        massage = ss.str();
        this->dio.write(massage);
        this->dio.read();
        i++;
    }
    this->dio.write("Done.");
    this->dio.read();
    return;
}

Command_client_Upload::Command_client_Upload(int *socket)
{
    this->dio = SocketIO(socket);
}

void Command_client_Upload::execute()
{
    string *massage_recieved;
    cout << this->dio.read() << endl;
    string path, upath;
    cin >> path;
    this->dio.write(path);
    cout << this->dio.read() << endl;
    cin >> upath;
    this->dio.write(upath);
    this->dio.read();
    return;
}

Command_Client_Settings ::Command_Client_Settings(int *socket)
{
    this->dio = SocketIO(socket);
}
void Command_Client_Settings ::execute()
{
    cout << this->dio.read() << endl;
    string massage, recieved_massage;
    cin >> massage;
    this->dio.write(massage);
    recieved_massage = this->dio.read();
    if (recieved_massage.length() > 1)
    {
        cout << recieved_massage << endl;
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
    string massage_recieved;
    massage_recieved = this->dio.read();
    if (strcmp(massage_recieved.c_str(), "please upload data") == 0 || strcmp(massage_recieved.c_str(), "please classify the data") == 0)
    {
        cout << massage_recieved << endl;
        this->dio.write("OK");
        return;
    }
    while (true)
    {
        massage_recieved = this->dio.read();
        cout << massage_recieved << endl;
        this->dio.write("OK");
        if (strcmp(massage_recieved.c_str(), "Done.") == 0)
        {
            return;
        }
    }
}
