#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

struct ClientArgs {
    int client_fd;
};

class Server {
public:
    Server(int port);
    void start();

private:
    int port;
    int server_fd;

    void setupSocket();
    static void* handleClientThread(void* arg);
    static void handleClient(int client_fd);
    static std::string parseRequestPath(const std::string& request);
    static std::string getContentType(const std::string& path);
    static std::string readFile(const std::string& filepath);
    static std::string buildResponse(int status, const std::string& contentType, const std::string& body);
};
