#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include "DefaultIO.h"

using namespace std;


    string DefaultIO::read(){return " ";}
      void DefaultIO::write(string massage){}

    SocketIO::SocketIO(int *socket) {
        this->socket = socket;
    }
    string SocketIO::read() {
         char buffer[4096];
            int expected_data_len = sizeof(buffer);
            memset(buffer, 0, expected_data_len); // Setting the buffer to 0's

            int read_bytes = recv(*(this->socket), buffer, expected_data_len, 0);
            return buffer;
    }
    void SocketIO::write(string massage) {
        int sent_bytes = send(*(this->socket), massage.c_str(), massage.length(), 0); // Sending the result depending on the client's input to him
            if (sent_bytes < 0)
            {
                perror("error sending to client");
            }
    }
