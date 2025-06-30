#include <cstdint>
#include <iostream>
#include <chrono>
#include <cstdbool>


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
