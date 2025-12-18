# weather-station

Compile on Windows:

Server backend:
g++ -std=c++17 src/server_collector/main.cpp src/ipc_utils/shm_wrapper.cpp -o server_backend.exe -lws2_32

Client sensor:
g++ -std=c++17 src/sensor_client/main.cpp -o sensor_node.exe -lws2_32