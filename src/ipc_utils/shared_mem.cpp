#include "shared_mem.h"
#include <iostream>

SharedMemoryManager::SharedMemoryManager(bool createNew) {
    // 1. Створюємо м'ютекс для синхронізації
    hMutex = CreateMutexW(NULL, FALSE, MUTEX_NAME);

    // 2. Створюємо або відкриваємо пам'ять
    if (createNew) {
        hMapFile = CreateFileMappingW(
            INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 
            0, sizeof(DashboardState), SHM_NAME);
    } else {
        hMapFile = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, SHM_NAME);
    }

    if (!hMapFile) {
        std::cerr << "SHM Error: " << GetLastError() << std::endl;
        return;
    }

    // 3. Мапимо пам'ять у вказівник
    pData = (DashboardState*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(DashboardState));

    // Якщо ми створили нову пам'ять, обнулимо її
    if (createNew && pData) {
        memset(pData, 0, sizeof(DashboardState));
    }
}

SharedMemoryManager::~SharedMemoryManager() {
    if (pData) UnmapViewOfFile(pData);
    if (hMapFile) CloseHandle(hMapFile);
    if (hMutex) CloseHandle(hMutex);
}

void SharedMemoryManager::updateRegion(const WeatherPacket& packet) {
    WaitForSingleObject(hMutex, INFINITE); // Блокуємо
    if (pData) {
        int idx = packet.region;
        if (idx >= 0 && idx < RegionID::COUNT) {
            pData->data[idx] = packet;      // Записуємо нові дані
            pData->isActive[idx] = true;    // Ставимо прапорець, що регіон живий
        }
    }
    ReleaseMutex(hMutex); // Розблокуємо
}

DashboardState SharedMemoryManager::readAll() {
    DashboardState copy = {};
    WaitForSingleObject(hMutex, INFINITE);
    if (pData) {
        copy = *pData; // Копіюємо весь стан миттєво
    }
    ReleaseMutex(hMutex);
    return copy;
}