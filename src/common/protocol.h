#pragma once
#include <cstdint>

// Ідентифікатори регіонів
enum RegionID {
    NORTH = 0,
    SOUTH = 1,
    EAST  = 2,
    WEST  = 3,
    COUNT = 4 // Кількість регіонів
};

// Пакет, який летить по мережі (від Сенсора до Колектора)
#pragma pack(push, 1)
struct WeatherPacket {
    RegionID region;
    double temperature;
    double humidity;
    double windSpeed;
};
#pragma pack(pop)

// Структура "Дашборд", яка лежить в Shared Memory
// Вона зберігає останній відомий стан ВСІХ регіонів
struct DashboardState {
    WeatherPacket data[RegionID::COUNT]; // Масив з 4-х елементів
    bool isActive[RegionID::COUNT];      // Чи надходили дані від цього регіону?
};

#define PORT 8080
#define SHM_NAME L"GlobalWeatherDashboard"
#define MUTEX_NAME L"GlobalWeatherMutex"