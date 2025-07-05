#include "SensorPackage.h"
#include <cstring> // Para std::memcpy
#include <cstdint>
#include <iostream>
#include <chrono>
#include <cstdbool>
#include <vector>
#include <boost/endian/conversion.hpp>
using boost::endian::native_to_big;


struct SensorData{
    int16_t sensor_id;
    std::time_t timestamp; // Timestamp of the data reading
    float temperature; // Temperature reading in degrees Celsius
    float humidity; // Humidity reading in percentage
    float pressure; // Atmospheric pressure reading in hPa
};


class SensorPackage{
    private:
        SensorData data;
        bool isEncrypted = false;


    public:
        inline void clear(){
            data.sensor_id = 0;
            data.timestamp = 0;
            data.temperature = 0.0f;
            data.humidity = 0.0f;
            data.pressure = 0.0f;
            isEncrypted = false;
        }
        inline void pack_float(std::vector<uint8_t> &bf, float value){
            static_assert(sizeof(uint32_t) == sizeof(float), "Se deben usar floats de 32 bits");
            uint32_t aux_int;
            std::memcpy(&aux_int, &value, sizeof(float));
            aux_int = native_to_big(aux_int);
            auto aux_bytes = reinterpret_cast<uint8_t*>(&aux_int);
            bf.insert(bf.end(), aux_bytes, aux_bytes + sizeof(aux_int));

        }

        
        std::vector<uint8_t> pack(const SensorData& dt){
            std::vector<uint8_t> buffer;
            buffer.reserve(2+8+3*4);

            {
                uint16_t id = native_to_big(static_cast<uint16_t>(dt.sensor_id));
                auto id_bytes = reinterpret_cast<uint8_t*>(&id);
                buffer.insert(buffer.end(), id_bytes, id_bytes + sizeof(id));
            }
            {
                uint64_t t = static_cast<uint64_t>(dt.timestamp);
                uint64_t t_big = native_to_big(t);
                auto t_bytes = reinterpret_cast<uint8_t*>(&t_big);
                buffer.insert(buffer.end(), t_bytes, t_bytes + sizeof(t_big));
            }
            pack_float(buffer, dt.temperature);
            pack_float(buffer, dt.humidity);
            pack_float(buffer, dt.pressure);


            return buffer;
        }

        SensorPackage& set_id_sensor(int16_t id){
            data.sensor_id = id;
            return *this;
        }
        SensorPackage& set_timestamp(){
            auto now =std::chrono::system_clock::now();
            data.timestamp = std::chrono::system_clock::to_time_t(now);
            return *this;
        }
        SensorPackage& set_sensor_data(float temp, float hum, float press){
            data.temperature = temp;
            data.humidity = hum;
            data.pressure = press;
            return *this;
        }
        SensorPackage& set_encryption(bool encrypted){
            isEncrypted = encrypted;
            return *this;
        }
        SensorData get_data() const {
            return data;
        }
        bool is_encrypted() const {
            return isEncrypted;
        }


};