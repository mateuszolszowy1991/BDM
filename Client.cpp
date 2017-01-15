#include "Client.hpp"

Client::Client()
{
    //ctor
}

Client::~Client()
{
    //dtor
}

int Client::clientInitialize()
{
    Logger::saveToFile("BDM/INF: Client: Initialising Winsock...");
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        Logger::saveToFile("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 12345 );

}

void Client::connectToIDB()
{
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
    }

    Logger::saveToFile("BDM/INF: Client: Connected");
}

void Client::start()
{
    Logger::saveToFile("BDM/INF: Client: start()" );
    if(size_t l = recv(sock , server_reply , 2000 , 0) < 0)
    {
        cout << "RECEIVED: " << l <<endl;
        Logger::saveToFile("BDM/ERR: Client: recv failed ");

    }
}

char* Client::sendRequest(const char* message)
{
    int recv_size;
    string str(message);
    //Logger::saveToFile("BDM/INF: Client: sendRequest " + str );
    if( send(sock, message, strlen(message), 0) != strlen(message) )
    {
         Logger::saveToFile("BDM/ERR: Client: Send failed " );

    }
    if((recv_size = recv(sock , server_reply , 2000 , 0)) == 0)
    {
        Logger::saveToFile("BDM/ERR: Client: Receive failed " );

    }
    else
    {
        //puts("Reply received\n");
        server_reply[recv_size] = '\0';
        return server_reply;
    }

}

char* Client::receive()
{
    int recv_size;
    if((recv_size = recv(sock , server_reply , 2000 , 0)) == 0)
    {
        Logger::saveToFile("BDM/ERR: Client: Received failed " );

    }
    else
    {
        //puts("Reply received\n");
        cout << "Received: " << server_reply <<endl;
        server_reply[recv_size] = '\0';
        return server_reply;
    }
}
