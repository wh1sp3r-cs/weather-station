#pragma once
#include <cstdint>

// 1. Ідентифікатори регіонів
enum RegionID {
    NORTH = 0,
    SOUTH = 1,
    EAST  = 2,
    WEST  = 3,
    COUNT = 4 // Кількість регіонів
};

// 2. Пакет, який летить по мережі (від Сенсора до Колектора)
#pragma pack(push, 1)
struct WeatherPacket {
    RegionID region;
    double temperature;
    double humidity;
    double windSpeed;
};
#pragma pack(pop)

// 3. Структура "Дашборд", яка лежить в Shared Memory (Спільна Пам'ять)
// Вона зберігає останній відомий стан ВСІХ регіонів
struct DashboardState {
    WeatherPacket data[RegionID::COUNT]; // Масив з 4-х елементів
    bool isActive[RegionID::COUNT];      // Чи надходили дані від цього регіону?
};

#define PORT 8080
#define SHM_NAME L"GlobalWeatherDashboard"
#define MUTEX_NAME L"GlobalWeatherMutex"