import sqlite3
from datetime import datetime, timedelta
import random

# Configuración
DATABASE_FILE = "iot_industrial.db"

def create_connection():
    """Establecer conexión con la base de datos"""
    try:
        conn = sqlite3.connect(DATABASE_FILE)
        return conn
    except sqlite3.Error as e:
        print(f"Error al conectar a la base de datos: {e}")
        return None

def insert_sensor_data(conn, data):
    """Insertar datos de sensor en la base de datos"""
    sql = '''INSERT INTO sensor_data(sensor_id, timestamp, temperatura, presion, humedad)
             VALUES(?, ?, ?, ?, ?)'''
    try:
        cur = conn.cursor()
        cur.execute(sql, data)
        conn.commit()
        return cur.lastrowid
    except sqlite3.Error as e:
        print(f"Error al insertar datos: {e}")
        return None

def generar_datos_ejemplo():
    """Generar datos de ejemplo para sensores"""
    datos = []
    base_time = datetime.now() - timedelta(days=7)  # Datos de los últimos 7 días
    
    # Valores normales para cada sensor
    rangos = {
        1: {"temp": (18.0, 25.0), "pres": (1010.0, 1015.0), "hum": (40.0, 60.0)},
        2: {"temp": (22.0, 28.0), "pres": (1008.0, 1013.0), "hum": (35.0, 55.0)},
        3: {"temp": (20.0, 26.0), "pres": (1011.0, 1016.0), "hum": (45.0, 65.0)}
    }
    
    # Generar 10 registros por sensor
    for sensor_id in rangos:
        for i in range(10):
            timestamp = base_time + timedelta(hours=i*2)
            temp_range = rangos[sensor_id]["temp"]
            pres_range = rangos[sensor_id]["pres"]
            hum_range = rangos[sensor_id]["hum"]
            
            datos.append((
                sensor_id,
                timestamp.isoformat(),
                round(random.uniform(*temp_range), 2),
                round(random.uniform(*pres_range), 2),
                round(random.uniform(*hum_range), 2)
            ))

    # Datos fuera de rango
    anomalos = [
        # sensor_id, timestamp, temp, pres, hum
        (1, datetime.now().isoformat(), 150.0, 800.0, 10.0),  # temperatura y presion muy alta/baja
        (2, (datetime.now() - timedelta(minutes=10)).isoformat(), 5.0, 1200.0, 90.0),  # valores extremos
        (3, (datetime.now() - timedelta(minutes=5)).isoformat(), -10.0, 1000.0, 5.0)
        # temperatura negativa (fuera de rango)
    ]

    datos.extend(anomalos)

    return datos

def poblar_db():
    """Función principal para poblar la base de datos"""
    conn = create_connection()
    if conn is None:
        return
    
    # Generar datos de ejemplo
    datos_sensores = generar_datos_ejemplo()
    
    # Insertar datos en la base de datos
    print("Insertando datos de ejemplo...")
    for dato in datos_sensores:
        insert_sensor_data(conn, dato)
    
    print(f"Se insertaron {len(datos_sensores)} registros de ejemplo")
    conn.close()

