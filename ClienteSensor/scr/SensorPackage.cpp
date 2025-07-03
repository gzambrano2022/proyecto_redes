#include <cstdint>
#include <iostream>
#include <chrono>
#include <cstdbool>
#include <vector>


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
        std::vector<uint8_t> pack(const SensorData& d){
            std::vector<uint8_t> buffer;
            buffer.reserve(2+8+34);

            // Pack sensor ID
        }
        SensorPackage& set_id_sensor(int16_t id){
            data.sensor_id = id;
            returnthis;
        }
        SensorPackage& set_timestamp(){
            auto now =std::chrono::system_clock::now();
            data.timestamp = std::chrono::system_clock::to_time_t(now);
            return this;
        }
        SensorPackage& set_sensor_data(float temp, float hum, float press){
            data.temperature = temp;
            data.humidity = hum;
            data.pressure = press;
            returnthis;
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

        std::vector<uint8_t> SensorPackage::serialize() const {
            std::vector<uint8_t> buffer(sizeof(SensorData));
            std::memcpy(buffer.data(), &data, sizeof(SensorData));
            return buffer;
        }

        std::vector<uint8_t> SensorPackage::encrypt(const std::vector<uint8_t>& input) const {
            std::vector<uint8_t> output = input;
            for (auto& byte : output) {
                byte ^= 0xAA;
            }
            return output;
        }
