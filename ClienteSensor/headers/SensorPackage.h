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
    uint32_t crc;               // Firma CRC32 del resto del contenido
};
#pragma pack(pop)

class SensorPackage {
private:
    SensorData data;            // Datos del sensor
    bool isEncrypted = false;   // Estado de encriptación

    uint32_t crc32(const uint8_t* data, size_t length);  // Función para calcular CRC32

public:

    // Métodos de configuración
    SensorPackage& set_id_sensor(int16_t id);
    SensorPackage& set_timestamp();
    SensorPackage& set_sensor_data(float temp, float hum, float press);
    SensorPackage& set_encryption(bool encrypted);

    // Acceso a datos
    SensorData get_data() const;


    bool is_encrypted() const;

    // Utilidades
    void clear();  
    void finalize();  
    void pack_float(std::vector<uint8_t>& bf, float value);  
    std::vector<uint8_t> serialize();  
    std::vector<uint8_t> pack(const SensorData& dt);
    
};

#endif // SENSORPACKAGE_H