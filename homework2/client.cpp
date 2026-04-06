#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

void recv_msg(int sock) {
    char buffer[1024] = {0};
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int len = recv(sock, buffer, 1024, 0);
        if (len <= 0) {
            cout << "服务器断开连接" << endl;
            close(sock);
            break;
        }
        cout << "服务器：" << buffer << endl;
    }
}

void send_msg(int sock) {
    string msg;
    while (true) {
        getline(cin, msg);
        send(sock, msg.c_str(), msg.size(), 0);
        if (msg == "exit") break;
    }
    close(sock);
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    connect(sock, (sockaddr *)&addr, sizeof(addr));
    cout << "已连接服务器" << endl;

    thread t1(recv_msg, sock);
    thread t2(send_msg, sock);

    t1.join();
    t2.join();

    close(sock);
    return 0;
}
