import socket
import struct 
import time
import requests
import binascii  # Para calcular CRC32

# Estructura binaria del paquete C++ (SensorData + crc)
# int16_t sensor_id; std::time_t timestamp; float temperature, humidity, pressure; uint32_t crc
sensor_struct_format = '>hqfffI'  # Cambiar a big endian # I = uint32 para crc
BUFFER_SIZE = struct.calcsize(sensor_struct_format)
POST_URL = 'http://localhost:5000/api/sensor-data'

def decrypt(data: bytes) -> bytes:
  return bytes(b ^ 0xAA for b in data)

def verify_crc(data: bytes) -> bool:
    # Separar datos sin CRC y CRC recibido
    data_without_crc = data[:-4]
    received_crc_bytes = data[-4:]
    received_crc = struct.unpack('>I', received_crc_bytes)[0]

    # Calcular CRC32 con binascii.crc32 (devuelve int)
    calc_crc = binascii.crc32(data_without_crc) & 0xFFFFFFFF

    print(f"[DEBUG] CRC recibido: {received_crc:#010x}, CRC calculado: {calc_crc:#010x}")
    return calc_crc == received_crc

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

      # Validar CRC
      if not verify_crc(decrypted_data):
        print("[ERROR] Firma CRC32 inválida. Paquete descartado.")
        return

      sensor_id, timestamp, temp, hum, press, crc = deserialize_sensor_data(decrypted_data)

      print(f"[INFO] Datos recibidos:")
      print(f" Sensor ID: {sensor_id}")
      if 0 < timestamp < 2**31:
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