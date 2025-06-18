import sqlite3
from sqlite3 import Error
from datetime import datetime

def create_connection(db_file):
    """Crear una conexión a la base de datos SQLite"""
    conn = None
    try:
        conn = sqlite3.connect(db_file)
        print(f"Conexión a SQLite DB {db_file} exitosa")
        return conn
    except Error as e:
        print(f"Error al conectar a {db_file}: {e}")
    return conn

def create_table(conn):
    """Crear tabla para almacenar datos de sensores"""
    try:
        cursor = conn.cursor()
        cursor.execute('''
        CREATE TABLE IF NOT EXISTS sensor_data (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            sensor_id INTEGER NOT NULL,
            timestamp DATETIME NOT NULL,
            temperatura REAL NOT NULL,
            presion REAL NOT NULL,
            humedad REAL NOT NULL,
            recibido_en DATETIME DEFAULT CURRENT_TIMESTAMP
        )
        ''')
        
        # Crear índice para búsquedas por sensor_id
        cursor.execute('CREATE INDEX IF NOT EXISTS idx_sensor_id ON sensor_data(sensor_id)')
        
        # Crear índice para búsquedas por timestamp
        cursor.execute('CREATE INDEX IF NOT EXISTS idx_timestamp ON sensor_data(timestamp)')
        
        conn.commit()
        print("Tabla sensor_data creada exitosamente")
    except Error as e:
        print(f"Error al crear tabla: {e}")

def initialize_database():
    """Inicializar la base de datos"""
    database = "iot_industrial.db"
    conn = create_connection(database)
    if conn is not None:
        create_table(conn)
        conn.close()
    else:
        print("Error! No se puede crear la conexión a la base de datos.")
