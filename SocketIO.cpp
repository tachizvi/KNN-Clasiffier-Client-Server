#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include "SocketIO.h"

using namespace std;

SocketIO::SocketIO(int *socket)
{
    this->socket = socket;
}
string SocketIO::read()
{
    char buffer[4096];
    int expected_data_len = sizeof(buffer);
    memset(buffer, 0, expected_data_len); // Setting the buffer to 0's
    int read_bytes = recv(*(this->socket), buffer, expected_data_len, 0);
    if (strcmp(buffer ,"empty message") == 0){
        return "";
    }

    
    return buffer;
}
void SocketIO::write(string massage)
{
    int sent_bytes = send(*(this->socket), massage.c_str(), massage.length(), 0); // Sending the result depending on the client's input to him
    if (sent_bytes < 0)
    {
        perror("error sending to client");
    }
    if (sent_bytes == 0)
    {
        write("empty message");
    }
}
