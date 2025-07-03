#ifndef SENSORPACKAGE_H
#define SENSORPACKAGE_H

#include <cstdint>
#include <chrono>
#include <ctime>
#include <vector>

#pragma pack(push,1) // Eliminar padding
struct SensorData {
    int16_t sensor_id;          // ID único del sensor
    std::time_t timestamp;      // Timestamp de la lectura de datos
    float temperature;          // Lectura de temperatura en grados Celsius
    float humidity;             // Lectura de humedad en porcentaje
    float pressure;             // Lectura de presión atmosférica en hPa
};
#pragma pack(pop)

class SensorPackage {
private:
    SensorData data;            // Datos del sensor
    bool isEncrypted = false;   // Estado de encriptación

public:
    SensorPackage& set_id_sensor(int16_t id);
    SensorPackage& set_timestamp();
    SensorPackage& set_sensor_data(float temp, float hum, float press);
    SensorPackage& set_encryption(bool encrypted);
    SensorData get_data() const;


    bool is_encrypted() const;

    std::vector<uint8_t> serialize() const; // Serializa (binariza) SensorData.
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& input) const; // Encripta lo binario
};

#endif // SENSORPACKAGE_H