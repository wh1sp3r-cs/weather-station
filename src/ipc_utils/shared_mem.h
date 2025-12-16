#pragma once
#include <windows.h>
#include <string>
#include "../common/protocol.h"

class SharedMemoryManager {
    HANDLE hMapFile;
    HANDLE hMutex;
    DashboardState* pData;

public:
    SharedMemoryManager(bool createNew); // true для Колектора, false для UI
    ~SharedMemoryManager();

    // Оновити дані конкретного регіону (викликає Колектор)
    void updateRegion(const WeatherPacket& packet);

    // Отримати повний звіт (викликає UI)
    DashboardState readAll();
};