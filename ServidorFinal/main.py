from flask import Flask, request, jsonify, render_template
import sqlite3
from datetime import datetime
import Database.crear_db as crear_db
import Database.poblar_db as poblar_db

app = Flask(__name__)

# Configuración de la base de datos
DATABASE = 'iot_industrial.db'

def get_db_connection():
    conn = sqlite3.connect(DATABASE)
    conn.row_factory = sqlite3.Row
    return conn
@app.route('/')
def index():
    """Endpoint principal para la interfaz web"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    # Obtener los últimos 50 registros
    cursor.execute('''
    SELECT * FROM sensor_data 
    ORDER BY timestamp DESC 
    LIMIT 50
    ''')
    datos = cursor.fetchall()
    
    # Obtener lista de sensores disponibles
    cursor.execute('SELECT DISTINCT sensor_id FROM sensor_data ORDER BY sensor_id')
    sensores = [row['sensor_id'] for row in cursor.fetchall()]
    
    conn.close()
    
    return render_template('index.html', datos=datos, sensores=sensores)

@app.route('/api/sensor-data', methods=['POST'])
def receive_sensor_data():
    """Endpoint para recibir datos del servidor intermedio"""
    data = request.get_json()
    
    # Validar datos recibidos
    required_fields = ['sensor_id', 'timestamp', 'temperatura', 'presion', 'humedad']
    if not all(field in data for field in required_fields):
        return jsonify({"error": "Datos incompletos"}), 400
    
    try:
        conn = get_db_connection()
        cursor = conn.cursor()
        
        # Insertar datos en la base de datos
        cursor.execute('''
        INSERT INTO sensor_data (sensor_id, timestamp, temperatura, presion, humedad)
        VALUES (?, ?, ?, ?, ?)
        ''', (
            data['sensor_id'],
            data['timestamp'],
            data['temperatura'],
            data['presion'],
            data['humedad']
        ))
        
        conn.commit()
        conn.close()
        
        return jsonify({"message": "Datos almacenados correctamente"}), 201
    
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route('/api/sensor-data', methods=['GET'])
def get_sensor_data():
    """Endpoint para consultar datos de sensores"""
    try:
        sensor_id = request.args.get('sensor_id')
        start_time = request.args.get('start_time')
        end_time = request.args.get('end_time')
        limit = request.args.get('limit', default=100, type=int)
        
        conn = get_db_connection()
        cursor = conn.cursor()
        
        query = 'SELECT * FROM sensor_data'
        conditions = []
        params = []
        
        if sensor_id:
            conditions.append('sensor_id = ?')
            params.append(sensor_id)
        if start_time:
            conditions.append('timestamp >= ?')
            params.append(start_time)
        if end_time:
            conditions.append('timestamp <= ?')
            params.append(end_time)
        
        if conditions:
            query += ' WHERE ' + ' AND '.join(conditions)
        
        query += ' ORDER BY timestamp DESC LIMIT ?'
        params.append(limit)
        
        cursor.execute(query, params)
        results = cursor.fetchall()
        conn.close()
        
        # Convertir resultados a formato JSON
        sensor_data = [dict(row) for row in results]
        
        return jsonify(sensor_data), 200
    
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route('/api/health', methods=['GET'])
def health_check():
    """Endpoint para verificar el estado del servidor"""
    try:
        conn = get_db_connection()
        cursor = conn.cursor()
        cursor.execute('SELECT 1')
        conn.close()
        return jsonify({"status": "healthy"}), 200
    except Exception as e:
        return jsonify({"status": "unhealthy", "error": str(e)}), 500

if __name__ == '__main__':
    # Inicializar la base de datos si no existe
    crear_db.initialize_database()
    poblar_db.poblar_db()
    
    # Iniciar el servidor Flask
    app.run(host='0.0.0.0', port=5000, debug=True)