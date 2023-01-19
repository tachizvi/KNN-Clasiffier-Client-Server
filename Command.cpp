#include "DefaultIO.cpp"
#include <map>
#include <stdio.h>
#include <vector>
#include "ex2.h"

static map<string, int> Algorithms{
    {"AUC", 1},
    {"MAN", 2},
    {"CHB", 3},
    {"CAN", 4},
    {"MIN", 5}};

using namespace std;
class Command
{
protected:
    string dsecription;
    DefaultIO dio;

public:
    virtual void exectue() = 0;
    string get_description()
    {
        return this->dsecription;
    }
};

class Command_Upload : public Command
{
private:
    multimap<vector<double>, string> *data;
    vector<vector<double>> *unclassified_data;

public:
    Command_Upload(int *socket, multimap<vector<double>, string> *data, vector<vector<double>> *unclassified_data)
    {
        this->dsecription = "1. upload an unclassified csv data file";
        this->dio = SocketIO(socket);
        this->data = data;
        this->unclassified_data = unclassified_data;
    }
    void execute()
    {
        multimap<vector<double>, string> data_temp;
        vector<vector<double>> unclassified_data_temp;
        string classified, unclassified;
        string return_massage = "Plaese upload your local train CSV file.";
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
        *data = data_temp;
        *unclassified_data = unclassified_data_temp;
        return;
    }
};

class Command_settings : public Command
{
private:
    int *k;
    string *distance_function;

public:
    Command_settings(int *socket, int *k, string *distance_function)
    {
        this->dsecription = "2. algorithm settings";
        this->dio = SocketIO(socket);
        this->k = k;
        this->distance_function = distance_function;
    }

    void execute()
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
    Command_classify(int *socket, int *k, string *distance, multimap<vector<double>, string> *data, vector<vector<double>> *unclassified_data, vector<string> *results)
    {
        this->dsecription = "3. classify data";
        this->dio = SocketIO(socket);
        this->k = k;
        this->distance = distance;
        this->data = data;
        this->unclassified_data = unclassified_data;
        this->results = results;
    }

    void execute()
    {
        if (this->data->empty())
        {
            this->dio.write("classifying data complete");
        }
        for (vector<double> vect : *unclassified_data)
        {
            results->push_back(Knn_classify(data->begin()->first.size(), *data, vect, Algorithms[*distance], *k));
        }
        this->dio.write("please upload data");
        return;
    }
};

class Command_display : public Command
{
private:
    vector<string> *results;
    multimap<vector<double>, string> *data;

public:
    Command_display(int *socket, vector<string> *results, multimap<vector<double>, string> *data)
    {
        this->dsecription = "4. display results";
        this->dio = SocketIO(socket);
        this->results = results;
        this->data = data;
    }
    void execute()
    {
        if (this->data->empty())
        {
            this->dio.write("please upload data");
            this->dio.read();
            return;
        }
        if (this->results->empty())
        {
            this->dio.write("please classify the data");
            this->dio.read();
        }
        int i = 1;
        for (string s : *results)
        {
            stringstream ss;
            string massage;
            ss << i << " " << s;
            massage = ss.str();
            this->dio.write(massage);
            this->dio.read();
            i++;
        }
        this->dio.write("Done.");
        this->dio.read();
    }
};

class Command_Download : public Command
{

private:
    vector<string> *results;
    multimap<vector<double>, string> *data;
    char *client_file_path = "0";

public:
    Command_Download(int *socket, vector<string> *results, multimap<vector<double>, string> *data)
    {
        this->dsecription = "4. display results";
        this->dio = SocketIO(socket);
        this->results = results;
        this->data = data;
    }
    void execute()
    {
        if (this->data->empty())
        {
            this->dio.write("please upload data");
            this->dio.read();
            return;
        }
        if (this->results->empty())
        {
            this->dio.write("please classify the data");
            this->dio.read();
        }
        if (strcmp(this->client_file_path, "0"))
        {
            this->dio.write("please specify the file path:");
            strcpy(client_file_path, this->dio.read().c_str());
        }

        int i = 1;
        for (string s : *results)
        {
            stringstream ss;
            string massage;
            ss << i << " " << s;
            massage = ss.str();
            this->dio.write(massage);
            this->dio.read();
            i++;
        }
        this->dio.write("Done.");
        this->dio.read();
    }
};

class Command_client_Upload : public Command
{

    Command_client_Upload(int *socket)
    {
        this->dio = SocketIO(socket);
    }
    void execute()
    {
        string *massage_recieved;
        cout << this->dio.read() << endl;
        string path, upath;
        cin >> path;
        this->dio.write(path);
        cout << this->dio.read() << endl;
        cin >> upath;
        this->dio.write(upath);
    }
};

class Command_Client_Settings : public Command
{
    Command_Client_Settings(int *socket)
    {
        this->dio = SocketIO(socket);
    }
    void execute()
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
    }
};

class Command_Client_Classify : public Command
{
    Command_Client_Classify(int *socket)
    {
        this->dio = SocketIO(socket);
    }

    void execute()
    {
        cout << this->dio.read() << endl;
    }
};

class Command_Client_Display : public Command
{
    Command_Client_Display(int *socket)
    {
        this->dio = SocketIO(socket);
    }

    void execute()
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
};

class Command_Client_Download : public Command
{
private:
    string file_path;
    int* socket;

public:
    Command_Client_Download(int *socket)
    {
        this->dio = SocketIO(socket);
        this->socket = socket;
    }
    void execute()
    {
        string massage_recieved;
        massage_recieved = this->dio.read();
        if (strcmp(massage_recieved.c_str(), "please upload data") == 0 || strcmp(massage_recieved.c_str(), "please classify the data") == 0)
        {
            cout << massage_recieved << endl;
            this->dio.write("OK");
            return;
        }
        if (strcmp(massage_recieved.c_str(), "please specify the file path:") == 0)
        {
            cout << massage_recieved << endl;
            cin >> file_path;
            this->dio.write(file_path);
            pthread_t thread_o;
            pthread_create(&thread_o, NULL, write_to_file,(void*) (file_path.c_str() ,socket));
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
    void write_to_file(void* args){

    }
};
