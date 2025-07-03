#include "SensorPackage.h"
#include <cstring> // Para std::memcpy
#include <cstdint>
#include <iostream>
#include <chrono>
#include <cstdbool>
#include <vector>


SensorPackage& SensorPackage::set_id_sensor(int16_t id) {
    data.sensor_id = id;
    return *this;
}

SensorPackage& SensorPackage::set_timestamp() {
    auto now = std::chrono::system_clock::now();
    data.timestamp = std::chrono::system_clock::to_time_t(now);
    return *this;
}

SensorPackage& SensorPackage::set_sensor_data(float temp, float hum, float press) {
    data.temperature = temp;
    data.humidity = hum;
    data.pressure = press;
    return *this;
}

SensorPackage& SensorPackage::set_encryption(bool encrypted) {
    isEncrypted = encrypted;
    return *this;
}

SensorData SensorPackage::get_data() const {
    return data;
}

bool SensorPackage::is_encrypted() const {
    return isEncrypted;
}

std::vector<uint8_t> SensorPackage::serialize() const {
    std::vector<uint8_t> buffer(sizeof(SensorData));
    std::memcpy(buffer.data(), &data, sizeof(SensorData));
    return buffer;
}

std::vector<uint8_t> SensorPackage::encrypt(const std::vector<uint8_t>& input) const {
    std::vector<uint8_t> output = input;
    for (auto& byte : output) {
        byte ^= 0xAA;  // Encriptación XOR
    }
    return output;
}
