#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

void recv_msg(int client_socket) {
    char buffer[1024] = {0};
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int len = recv(client_socket, buffer, 1024, 0);
        if (len <= 0) {
            cout << "客户端断开连接" << endl;
            close(client_socket);
            break;
        }
        cout << "客户端：" << buffer << endl;
    }
}

void send_msg(int client_socket) {
    string msg;
    while (true) {
        getline(cin, msg);
        send(client_socket, msg.c_str(), msg.size(), 0);
        if (msg == "exit") break;
    }
    close(client_socket);
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 5);

    cout << "等待客户端连接..." << endl;
    int client_socket = accept(server_fd, nullptr, nullptr);
    cout << "客户端已连接" << endl;

    thread t1(recv_msg, client_socket);
    thread t2(send_msg, client_socket);

    t1.join();
    t2.join();

    close(server_fd);
    return 0;
}
