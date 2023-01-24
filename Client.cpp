#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "input.h"
#include "CLI.h"

using namespace std;

/// @brief the main function of client creates a socket, then connects to the server according the arguments
/// it recived in the command line, afterwards it awaits for the user of the client to input a message, then it sends it
/// to the server it connected to, and awaits for the server to respond back, and then it goes back to wait for an input untill
/// the users inputs "-1", then the client shuts down the connction

/// @param argv[0] = ip address of the server
/// @param argv[1] = port number that the server listens to
int main(int argc, char const *argv[])
{
    const char
        *ip_address = argv[1];
    const int port_no = stoi(argv[2]);

    // Creating a socket
    int sock = socket(AF_INET, SOCK_STREAM,
                      0);
    if (sock <
        0)
    {
        perror("error creating socket");
    }
    struct sockaddr_in sin;
    std::memset(
        &sin,
        0, sizeof(sin)); // Setting sin to 0's
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ip_address);
    sin.sin_port = htons(port_no);

    // Connecting to the servers
    if (connect(sock,
                (struct sockaddr
                     *)&sin,
                sizeof(sin)) <
        0)
    {
        perror("error connecting to server");
    }
    CLI_Client cli = CLI_Client(&sock);
    while (1)
    {
        cli.read_menu();
        int user_pick;
        std::cin >> user_pick;
        
        cli.exectue(user_pick);
    }
    cout << "Closing" << endl;
    close(sock);
    return 0;
}
