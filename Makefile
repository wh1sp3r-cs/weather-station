# --- Змінні ---
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pthread
BUILD    := ./bin
SRC      := ./src
LOGS     := ./logs

# --- Цілі (Targets) ---

# За замовчуванням компілюємо все
all: prepare server sensor

# Створення папок для бінарників та логів, якщо їх немає
prepare:
	mkdir -p $(BUILD)
	mkdir -p $(LOGS)

# --- Компіляція Сервера ---
# Сервер складається з main файлу та логіки обробки
server: $(SRC)/server/main.cpp
	$(CXX) $(CXXFLAGS) $(SRC)/server/main.cpp -o $(BUILD)/server_app
	@echo "Server built successfully!"

# --- Компіляція Сенсора ---
# Сенсор складається з main файлу та генератора
sensor: $(SRC)/client/main.cpp
	$(CXX) $(CXXFLAGS) $(SRC)/client/main.cpp -o $(BUILD)/sensor_node
	@echo "Sensor built successfully!"

# --- Очищення проекту ---
clean:
	rm -rf $(BUILD)/*
	@echo "Cleaned up!"

# --- Додаткові команди для запуску ---
run-server:
	./$(BUILD)/server_app

# Приклад запуску сенсора (Північ, IP localhost, порт 8080)
run-sensor-north:
	./$(BUILD)/sensor_node 127.0.0.1 8080 NORTH_EAST