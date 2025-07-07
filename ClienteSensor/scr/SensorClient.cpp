#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdint>
#include <cstring>
#include <boost/asio.hpp>
#include "SensorPackage.h"

std::vector<uint8_t> encrypt(const std::vector<uint8_t>& input) {
    std::vector<uint8_t> output = input;
    for (auto& byte : output) {
        byte ^= 0xAA; // XOR simple
    }
    return output;
}

int main() {
    // 1. Inicializar Winsock
    WSADATA wsaData;
    int wsInit = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsInit != 0) {
        std::cerr << "[ERROR] WSAStartup falló: " << wsInit << std::endl;
        return 1;
    }

    // 2. Crear paquete del sensor
    SensorPackage package;
    package.set_id_sensor(13)
           .set_timestamp()
           .set_sensor_data(800.5f, 1000.0f, 8.0f)
           .set_encryption(true);

    // 3. Serializar y encriptar
    std::vector<uint8_t> raw = package.serialize();
    std::vector<uint8_t> encrypted = encrypt(raw);
    
    //Corrupcion forzada
    //encrypted[10] ^= 0xFF;  // Cambia el byte 10 arbitrariamente

    // 4. Crear socket TCP
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "[ERROR] No se pudo crear socket." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000);  // Puerto del servidor intermedio
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Dirección local

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "[ERROR] No se pudo conectar al servidor." << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // 5. Enviar datos
    int bytesSent = send(sock, reinterpret_cast<const char*>(encrypted.data()), encrypted.size(), 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "[ERROR] Falló el envío de datos." << std::endl;
    } else {
        std::cout << "[CLIENTE] Datos enviados correctamente (" << bytesSent << " bytes)." << std::endl;
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
