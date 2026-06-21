#include "../include/server.h"

Server::Server(int port) : port(port), server_fd(-1) {}

void Server::setupSocket() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Failed to create socket\n";
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed\n";
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        std::cerr << "Listen failed\n";
        exit(EXIT_FAILURE);
    }
}

void Server::start() {
    setupSocket();
    std::cout << "Server running on http://localhost:" << port << "\n";

    while (true) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0) {
            std::cerr << "Accept failed\n";
            continue;
        }

        ClientArgs* args = new ClientArgs{client_fd};
        pthread_t thread;
        pthread_create(&thread, NULL, handleClientThread, args);
        pthread_detach(thread);
    }
}

void* Server::handleClientThread(void* arg) {
    ClientArgs* args = (ClientArgs*)arg;
    handleClient(args->client_fd);
    delete args;
    return NULL;
}

void Server::handleClient(int client_fd) {
    char buffer[4096] = {0};
    read(client_fd, buffer, sizeof(buffer));

    std::string request(buffer);
    std::string path = parseRequestPath(request);

    std::cout << "Request: " << path << "\n";

    std::string filepath;
    std::string contentType;
    int status = 200;

    if (path == "/" || path == "/index.html") {
        filepath = "static/index.html";
        contentType = "text/html";
    } else if (path == "/about") {
        filepath = "static/about.html";
        contentType = "text/html";
    } else if (path == "/style.css") {
        filepath = "static/style.css";
        contentType = "text/css";
    } else {
        filepath = "static/404.html";
        contentType = "text/html";
        status = 404;
    }

    std::string body = readFile(filepath);
    std::string response = buildResponse(status, contentType, body);
    write(client_fd, response.c_str(), response.size());
    close(client_fd);
}

std::string Server::parseRequestPath(const std::string& request) {
    std::istringstream stream(request);
    std::string method, path, version;
    stream >> method >> path >> version;
    return path;
}

std::string Server::getContentType(const std::string& path) {
    if (path.find(".html") != std::string::npos) return "text/html";
    if (path.find(".css")  != std::string::npos) return "text/css";
    if (path.find(".js")   != std::string::npos) return "application/javascript";
    return "text/plain";
}

std::string Server::readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return "<html><body><h1>404 - Not Found</h1></body></html>";
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

std::string Server::buildResponse(int status, const std::string& contentType, const std::string& body) {
    std::string statusText = (status == 200) ? "OK" : "Not Found";
    std::ostringstream response;
    response << "HTTP/1.1 " << status << " " << statusText << "\r\n";
    response << "Content-Type: " << contentType << "\r\n";
    response << "Content-Length: " << body.size() << "\r\n";
    response << "Connection: close\r\n";
    response << "\r\n";
    response << body;
    return response.str();
}
