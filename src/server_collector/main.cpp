#include <iostream>
#include <winsock2.h>
#include "../common/protocol.h"
#include "../ipc_utils/shared_mem.h"

#pragma comment(lib, "ws2_32.lib")

int main() {
    std::cout << "[BACKEND] Starting Server Collector...\n";
    
    // true = ми створюємо пам'ять (Server is Host)
    SharedMemoryManager ipc(true); 

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, SOMAXCONN);

    std::cout << "[BACKEND] Listening on port " << PORT << ". Waiting for sensors...\n";

    while (true) {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) continue;

        WeatherPacket pkg;
        int received = recv(clientSocket, (char*)&pkg, sizeof(pkg), 0);

        if (received == sizeof(WeatherPacket)) {
            std::cout << "[NET] Received from Region ID: " << pkg.region << "\n";
            
            // Оновлюємо конкретну комірку в Shared Memory
            ipc.updateRegion(pkg);
        }

        closesocket(clientSocket);
    }

    WSACleanup();
    return 0;
}