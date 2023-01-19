#include "DefaultIO.cpp"
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
    Command commands[5];
    DefaultIO dio;

public:
    CLI_Server(int *socket)
    {
        this->socket = socket;
        *(this->k) = 5;
        *(this->distance_function) = "AUC";
        this->commands[0] = Command_Upload(this->socket, this->data, this->unclassified_data);
        this->commands[1] = Command_settings(this->socket, this->k, this->distance_function);
        this->commands[2] = Command_classify(this->socket, this->k, this->distance_function, this->data, this->unclassified_data, this->results);
        this->commands[3] = Command_display(this->socket, this->results, this->data);
        this->dio = SocketIO(socket);
    }

    void execute()
    {
        int user_pick = stoi(this->dio.read());
        this->commands[user_pick - 1].exectue();
    }
    void print_menu() {
        for(Command c: this->commands) {
            this->dio.write(c.get_description());
            this->dio.read();
        }
    }
};

class CLI_Client {
    private:
    int *Socket;
    DefaultIO dio;
    Command commands[5];

    public:
    CLI_Client(int *socket) {
        this->dio = SocketIO(socket);
        this->commands[0] = Command_client_Upload(socket);
        this->commands[1] = Command_Client_Settings(socket);
        this->commands[2] = Command_Client_Classify(socket);
        this->commands[3] = Command_Client_Display(socket);
    }

    bool exectue(int user_pick) {
        if (user_pick < 1 || user_pick > 5) {
            cout << "invalid option" << endl;
            return false;
        }
        this->dio.write(to_string(user_pick));
        this->commands[user_pick-1].exectue();
        return true;
    }
    void read_menu() {
        for (Command c : this->commands) {
            cout << this->dio.read() << endl;
            this->dio.write("OK");
        }
    }
};