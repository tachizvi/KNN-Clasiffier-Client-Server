#ifndef DEFAULTIO_H
#define DEFAULTIO_H
#include <iostream>
#include <stdio.h>

using namespace std;

class DefaultIO
{
public:
     string read();
     virtual void write(string massage);
};

class SocketIO : public DefaultIO
{
private:
    int *socket;

public:
    SocketIO(int *socket);
    string read();
    void write(string massage);
};
#endif