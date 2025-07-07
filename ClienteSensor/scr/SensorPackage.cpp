#include "SensorPackage.h"
#include <cstring> // Para std::memcpy
#include <cstdint>
#include <iostream>
#include <chrono>
#include <cstdbool>
#include <vector>
#include <boost/endian/conversion.hpp>
using boost::endian::native_to_big;

// Calcula firma crc32 de un bloque de datos binarios
uint32_t SensorPackage::crc32(const uint8_t* data, size_t length) {
    static uint32_t table[256];
    static bool have_table = false;
    uint32_t crc;

    // Inicializa la tabla crc32 si no estaba lista
    if (!have_table) {
        for (uint32_t i = 0; i < 256; i++) {
            uint32_t rem = i;
            for (uint32_t j = 0; j < 8; j++) {
                if (rem & 1)
                    rem = (rem >> 1) ^ 0xEDB88320;
                else
                    rem >>= 1;
            }
            table[i] = rem;
        }
        have_table = true;
    }

    // Calcula crc32 con tabla precalculada
    crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; i++) {
        uint8_t octet = data[i];
        crc = (crc >> 8) ^ table[(crc & 0xFF) ^ octet];
    }
    return ~crc;
}

// Liampia los campos del paquete
void SensorPackage::clear() {
    data.sensor_id = 0;
    data.timestamp = 0;
    data.temperature = 0.0f;
    data.humidity = 0.0f;
    data.pressure = 0.0f;
    data.crc = 0;
    isEncrypted = false;
}

// Convierte float a binario y lo agrega al buffer en big endian 
void SensorPackage::pack_float(std::vector<uint8_t>& bf, float value) {
    static_assert(sizeof(uint32_t) == sizeof(float), "Se deben usar floats de 32 bits");
    uint32_t aux_int;
    std::memcpy(&aux_int, &value, sizeof(float));
    aux_int = native_to_big(aux_int);
    auto aux_bytes = reinterpret_cast<uint8_t*>(&aux_int);
    bf.insert(bf.end(), aux_bytes, aux_bytes + sizeof(aux_int));
}

// Serializa el paquete completo y calcula CRC automaticamente
std::vector<uint8_t> SensorPackage::serialize() {
    std::vector<uint8_t> buffer;

    // Serializa el sensor
    {
        uint16_t id_be = native_to_big(data.sensor_id);
        auto p = reinterpret_cast<uint8_t*>(&id_be);
        buffer.insert(buffer.end(), p, p + sizeof(id_be));
    }

    // Serializa el timestamp
    {
        uint64_t ts_be = native_to_big(static_cast<uint64_t>(data.timestamp));
        auto p = reinterpret_cast<uint8_t*>(&ts_be);
        buffer.insert(buffer.end(), p, p + sizeof(ts_be));
    }

    // Serializa los 3 floats
    pack_float(buffer, data.temperature);
    pack_float(buffer, data.humidity);
    pack_float(buffer, data.pressure);

    // Calcular CRC sobre los datos serializados
    uint32_t crc_val = crc32(buffer.data(), buffer.size());

    // Agregar CRC al buffer
    uint32_t crc_be = native_to_big(crc_val);
    auto p_crc = reinterpret_cast<uint8_t*>(&crc_be);
    buffer.insert(buffer.end(), p_crc, p_crc + sizeof(crc_be));

    // Guardar el CRC calculado en el struct
    data.crc = crc_val;

    return buffer;
}

// Setter del id del sensor
SensorPackage& SensorPackage::set_id_sensor(int16_t id) {
    data.sensor_id = id;
    return *this;
}

// Setter para el timestamp 
SensorPackage& SensorPackage::set_timestamp() {
    auto now = std::chrono::system_clock::now();
    data.timestamp = std::chrono::system_clock::to_time_t(now);
    return *this;
}

// Setter para los datos simulados del sensor (temp, humedad, presion)
SensorPackage& SensorPackage::set_sensor_data(float temp, float hum, float press) {
    data.temperature = temp;
    data.humidity = hum;
    data.pressure = press;
    return *this;
}

// Setter para marcar si el paquete se encriptara
SensorPackage& SensorPackage::set_encryption(bool encrypted) {
    isEncrypted = encrypted;
    return *this;
}

// Getter de los datos del paquete
SensorData SensorPackage::get_data() const {
    return data;
}

// Getter para saber si el paquete fue marcado como encriptado
bool SensorPackage::is_encrypted() const {
    return isEncrypted;
}
