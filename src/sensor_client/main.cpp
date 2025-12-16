#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <ctime>
#include <vector>
#include "../common/protocol.h"

#pragma comment(lib, "ws2_32.lib")

// Допоміжна функція для рандому
double getRand(double min, double max) {
    return min + (double)(rand()) / ((double)(RAND_MAX / (max - min)));
}

int main(int argc, char* argv[]) {
    // 1. Налаштування підключення
    std::string ip = "127.0.0.1";
    if (argc > 1) ip = argv[1];

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    srand(time(0));

    // 2. Меню вибору типу сенсора
    int choice = -1;
    std::cout << "=== Weather Sensor Setup ===\n";
    std::cout << "Select Region:\n";
    std::cout << "[0] NORTH (Cold)\n";
    std::cout << "[1] SOUTH (Hot)\n";
    std::cout << "[2] EAST  (Windy)\n";
    std::cout << "[3] WEST  (Humid)\n";
    std::cout << "Enter ID: ";
    std::cin >> choice;

    if (choice < 0 || choice > 3) {
        std::cout << "Invalid ID. Defaulting to NORTH.\n";
        choice = 0;
    }
    RegionID myRegion = (RegionID)choice;

    std::cout << "Sensor [" << choice << "] started. Sending data to " << ip << "...\n";

    while (true) {
        SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(PORT);
        inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

        if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cout << "Connecting...\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            closesocket(sock);
            continue;
        }

        // 3. Генерація даних залежно від регіону
        WeatherPacket pkg;
        pkg.region = myRegion;

        switch (myRegion) {
            case NORTH: // Холодно
                pkg.temperature = getRand(-25.0, 5.0);
                pkg.humidity = getRand(10.0, 50.0);
                pkg.windSpeed = getRand(5.0, 15.0);
                break;
            case SOUTH: // Жарко
                pkg.temperature = getRand(25.0, 45.0);
                pkg.humidity = getRand(40.0, 80.0);
                pkg.windSpeed = getRand(0.0, 10.0);
                break;
            case EAST: // Вітряно
                pkg.temperature = getRand(10.0, 25.0);
                pkg.humidity = getRand(30.0, 60.0);
                pkg.windSpeed = getRand(15.0, 35.0);
                break;
            case WEST: // Волого
                pkg.temperature = getRand(15.0, 30.0);
                pkg.humidity = getRand(70.0, 100.0);
                pkg.windSpeed = getRand(2.0, 12.0);
                break;
        }

        // 4. Відправка
        send(sock, (char*)&pkg, sizeof(pkg), 0);
        std::cout << "Sent Data -> Temp: " << pkg.temperature << "\n";

        closesocket(sock);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Раз в секунду
    }

    WSACleanup();
    return 0;
}