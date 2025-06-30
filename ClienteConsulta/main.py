import requests
import time

# URL del servidor
API_URL = "http://localhost:5000/api/sensor-data"

# Rangos aceptables
RANGOS = {
    "temperatura": (0, 100),
    "presion": (900, 1100),
    "humedad": (20, 80)
}

def validar(dato):
    alertas = []
    for campo, (min_val, max_val) in RANGOS.items():
        valor = dato[campo]
        if not (min_val <= valor <= max_val):
            alerta = f"[ALERTA] {campo.capitalize()} fuera de rango: {valor} (Sensor {dato['sensor_id']})"
            alertas.append(alerta)
    return alertas

def consultar_api():
    try:
        response = requests.get(API_URL)
        if response.status_code == 200:
            datos = response.json()
            for d in datos:
                alertas = validar(d)
                for alerta in alertas:
                    print(alerta)
        else:
            print("Error: código de estado", response.status_code)
    except Exception as e:
        print("Error de conexión:", e)

def ejecutar_cliente(periodo_segundos=5):
    while True:
        consultar_api()
        time.sleep(periodo_segundos)

if __name__ == "__main__":
    ejecutar_cliente()
