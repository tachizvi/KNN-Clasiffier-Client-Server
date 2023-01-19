#include "DefaultIO.h"
#include <map>
#include <stdio.h>
#include <vector>
#include "ex2.h"
#include "Command.h"
#include "CLI.h"


   CLI_Server:: CLI_Server(int *socket)
    {
        cout<< "kssssemek" << endl;
        this->socket = socket;
        cout<< "kssssemek" << endl;
        (this->k) = new int(5);
                cout<< "kssssemek" << endl;

                cout<< "kssssemek" << endl;

        this->distance_function = new string("AUC");
        cout << "fine" << endl;
        this->commands[0] = Command_Upload(this->socket, this->data, this->unclassified_data);
                cout << "fine" << endl;

        this->commands[1] = Command_settings(this->socket, this->k, this->distance_function);
                cout << "fine" << endl;

        this->commands[2] = Command_classify(this->socket, this->k, this->distance_function, this->data, this->unclassified_data, this->results);
                cout << "fine" << endl;

        this->commands[3] = Command_display(this->socket, this->results, this->data);
                cout << "fine" << endl;

        this->dio = SocketIO(socket);
                cout << "fine" << endl;

    }

    void CLI_Server::execute()
    {
        string user_pick_s = this->dio.read();
        cout << user_pick_s << endl;
        this->commands[0].exectue();
    }
    void CLI_Server::print_menu() {
        for(Command c: this->commands) {
            this->dio.write(c.get_description());
            this->dio.read();
        }
        this->dio.read();
    }



    CLI_Client::CLI_Client(int *socket) {
        this->dio = SocketIO(socket);
        this->commands[0] = Command_client_Upload(socket);
        this->commands[1] = Command_Client_Settings(socket);
        this->commands[2] = Command_Client_Classify(socket);
        this->commands[3] = Command_Client_Display(socket);
    }

    bool CLI_Client::exectue(int user_pick) {
        if (user_pick < 1 || user_pick > 5) {
            cout << "invalid option" << endl;
            return false;
        }
        this->dio.write(to_string(user_pick));
        this->commands[user_pick-1].exectue();
        return true;
    }
    void CLI_Client::read_menu() {
        for (Command c : this->commands) {
            cout << this->dio.read() << endl;
            this->dio.write("OK");
        }
    }
