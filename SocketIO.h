#pragma once

#ifndef DEFAULTIO_H
#define DEFAULTIO_H
#include <iostream>
#include <stdio.h>

using namespace std;


class SocketIO 
{
private:
    int *socket;

public:
    SocketIO() = default;
    SocketIO(int *socket);
    string read();
    void write(string massage);
};
#endif