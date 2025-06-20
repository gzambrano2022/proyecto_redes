#include <cstdint>
#include <iostream>
#include <chrono>

struct DataSensors
{
    int16_t sensorID; // Unique identifier for the sensor
    std::time_t timestamp; // Timestamp of the data reading
    float temperature; // Temperature reading in degrees Celsius
    float humidity; // Humidity reading in percentage
    float pressure; // Atmospheric pressure reading in hPa
};
