import socket
import struct 
import time
import requests

# Estructura binaria del paquete C++ (SensorData)
# int16_t sensor_id; std::time_t timestamp; float temperature, humidity, pressure
sensor_struct_format = '<hqfff' # < = little endian, h = int16, q = int64, f = float
BUFFER_SIZE = struct.calcsize(sensor_struct_format)
POST_URL = 'http://localhost:5000/api/sensor-data'

def decrypt(data: bytes) -> bytes:
  return bytes(b ^ 0xAA for b in data)

def deserialize_sensor_data(binary_data: bytes):
  return struct.unpack(sensor_struct_format, binary_data)

def run_intermediate_server():
  HOST = '0.0.0.0'
  SENSOR_PORT = 9000

  # 1. Conexión con sensor
  with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sensor_sock:
    sensor_sock.bind((HOST, SENSOR_PORT))
    sensor_sock.listen(1)
    print(f"[SERVER] Esperando conexión del sensor en el puerto {SENSOR_PORT}...")
    conn_sensor, addr_sensor = sensor_sock.accept()
    with conn_sensor:
      print(f"[SERVER] Conexión desde sensor: {addr_sensor}")
      encrypted_data = conn_sensor.recv(BUFFER_SIZE)
      if len(encrypted_data) != BUFFER_SIZE:
        print("[ERROR] Tamaño de datos incorrecto.")
        return 
      
      # 2. Desencriptar y decodificar
      
      decrypted_data = decrypt(encrypted_data)
      sensor_id, timestamp, temp, hum, press = deserialize_sensor_data(decrypted_data)

      print(f"[INFO] Datos recibidos:")
      print(f" Sensor ID: {sensor_id}")
      if timestamp > 0 and timestamp < 2**31:
        print(f" Timestamp: {time.ctime(timestamp)}")
      else:
        print(f" Timestamp inválido: {timestamp}")

      print(f" Temperatura: {temp} °C")
      print(f" Humedad: {hum} %")
      print(f" Presión: {press} hPa")

      # 3. Preparar JSON

      payload = {
        "sensor_id": int(sensor_id),
        "timestamp": int(timestamp),
        "temperatura": float(temp),
        "humedad": float(hum),
        "presion": float(press)
      }

      # 4. Enviar al servidor final Flask 

      try:
        response = requests.post(POST_URL, json=payload)
        if response.status_code == 201:
          print("[SUCCESS] Datos reenviados al servidor final correctamente.")
        else:
          print(f"[ERROR] Falló el reenvío: {response.status_code} {response.text}")
      except Exception as e:
        print(f"[ERROR] No se pudo conectar al servidor Flask: {str(e)}")


if __name__ == '__main__':
  run_intermediate_server()