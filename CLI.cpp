#include "SocketIO.h"
#include <map>
#include <stdio.h>
#include <vector>
#include "ex2.h"
#include "Command.h"
#include "CLI.h"

CLI_Server::CLI_Server(int *socket)
{
    this->socket = socket;
    this->k = 5;
    this->data = {};
    this->unclassified_data = {};
    this->results = {};

    this->distance_function = "AUC";
    this->commands[0] = (new Command_Upload(this->socket, &this->data, &this->unclassified_data));
    this->commands[1] = new Command_settings(this->socket, &this->k, &this->distance_function);
    this->commands[2] = new Command_classify(this->socket, &this->k, &this->distance_function, &this->data, &this->unclassified_data, &this->results);
    this->commands[3] = new Command_display(this->socket, &this->results, &this->data);
    this->commands[4] = new Command_download(this->socket, &this->results, &this->data);
    this->dio = SocketIO(socket);
}

void CLI_Server::execute(int* client_on)
{   
    string mes = this->dio.read();
    if (mes.size() == 0)
    {   
        *client_on = 0;
        return;
    }
    int user_pick_s = stoi(mes);
    (this->commands)[user_pick_s - 1]->execute();
    this->dio.read();
}

void CLI_Server::print_menu()
{
    stringstream ss;
    for (int i = 0; i <= 4; i++)
    {
        ss << "\n" << commands[i]->get_description();
        
    }
    string message = ss.str();
    this->dio.write(message);
    // this->dio.read();
}

CLI_Client::CLI_Client(int *socket)
{
    this->dio = SocketIO(socket);
    this->commands[0] = new Command_client_Upload(socket);
    this->commands[1] = new Command_Client_Settings(socket);
    this->commands[2] = new Command_Client_Classify(socket);
    this->commands[3] = new Command_Client_Display(socket);
    this->commands[4] = new Command_Client_Download(socket);
}

bool CLI_Client::exectue(int user_pick)
{
    if (user_pick < 1 || user_pick > 5)
    {
        return false;
    }
    this->dio.write(to_string(user_pick));
    ((this->commands)[user_pick - 1])->execute();
    this->dio.write("finished_execution");
    return true;
}
void CLI_Client::read_menu()
{
    cout << this->dio.read() << endl;
}
