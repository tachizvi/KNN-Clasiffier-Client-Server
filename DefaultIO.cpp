#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include <unistd.h>
#include <string.h>
#include <sstream>

using namespace std;

class DefaultIO
{
public:
    virtual string read() = 0;
    virtual void write(string massage) = 0;
};

class SocketIO : public DefaultIO
{
private:
    int *socket;

public:
    SocketIO(int *socket)
    {
        this->socket = socket;
    }
    string read()
    {
        char buffer[4096];
        int expected_data_len = sizeof(buffer);
        memset(buffer, 0, expected_data_len); // Setting the buffer to 0's

        int read_bytes = recv(*(this->socket), buffer, expected_data_len, 0);
        return buffer;
    }
    void write(string massage)
    {
        int sent_bytes = send(*(this->socket), massage.c_str(), massage.length(), 0); // Sending the result depending on the client's input to him
        if (sent_bytes < 0)
        {
            perror("error sending to client");
        }
    }
};