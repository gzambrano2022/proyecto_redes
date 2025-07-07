import requests
import time

# URL
API_URL = "http://localhost:5000/api/sensor-data"

# Rangos aceptables
RANGOS = {
    "temperatura": (0, 100), 
    "presion": (900, 1100),
    "humedad": (20, 80)
}

# Valida si los datos de estan dentro de el rango aceptable
def validar(dato):
    alertas = []

    # Itera sobre cada campo (temp, presion, humedad)
    for campo, (min_val, max_val) in RANGOS.items():
        valor = dato[campo]

        # Si el valor esta fuera del rango genera una alerta
        if not (min_val <= valor <= max_val):
            alerta = f"[ALERTA] {campo.capitalize()} fuera de rango: {valor} (Sensor {dato['sensor_id']})"
            alertas.append(alerta)
    return alertas

# Consulta a la API, recibe los datos y valida si hay anomalias
def consultar_api():
    try:
        
        # Solicitud GET a la API
        response = requests.get(API_URL)
        if response.status_code == 200:

            # Lista de diccionarios
            datos = response.json()
            for d in datos:

                # Verifica cada dato y muestra alertas si es que existen
                alertas = validar(d)
                for alerta in alertas:
                    print(alerta)
        else:
            print("Error: código de estado", response.status_code)
    except Exception as e:
        print("Error de conexión:", e)

# Ejecuta consultar_api en periodos de 5 segundos
def ejecutar_cliente(periodo_segundos=5):
    while True:
        consultar_api()
        time.sleep(periodo_segundos)

if __name__ == "__main__":
    ejecutar_cliente()
