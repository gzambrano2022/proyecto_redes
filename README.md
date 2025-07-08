# proyecto_redes

El sistema está organizado de manera que cada integrante del equipo se responsabiliza de un módulo específico, según la siguiente distribución:

Cliente Servidor: Juan Agustín Umaña Silva

Servidor Intermedio: Gabriela Isidora Zambrano Novoa

Servidor Final: Benjamín Alejandro Jiménez Ruíz

Cliente de Consulta: Joaquín Alejandro Godoy Vergara

Los detalles de implementación, resultados obtenidos y conclusiones del proyecto, se desarrollarán con mayor profundidad en el informe técnico correspondiente. 

### Instrucciones de compilación:

En cada directorio se encontrarán los módulos mencionados anteriormente, a continuación se entregará el orden en el cual compilar cada uno de estos. 

Por otro lado, es ESENCIAL que usted descargue en su computadora la libreria Boost_Asio, que facilita la programación de entrada y salidad asíncrona. 


### 1. Compilar Cliente Servidor

Para compilar Cliente Servidor, ingresar a la carpeta *ClienteSensor/scr*. Luego abrir una terminal de VSC o el editor de texto que usted posea, y escribir el siguiente comando:

 ```
 g++ SensorClient.cpp SensorPackage.cpp -I"[LUGAR DONDE USTED DESCARGÓ LA LIBRERIA]" -I"../headers" -std=c++17 -lws2_32 -o sensor.exe
  ```

Podrán notar dos cosas del código entregado. 

1. En un sector del código está entre corchetes ([LUGAR DONDE USTED DESCARGÓ LA LIBRERIA]), es evidente que significa, pero usted deberá cambiar esa parte del código con lo que dice ahí, es decir, indicar la dirección de donde se almacenó la descarga de la libreria Boost_Asio. 

En nuestro caso, la parte entre corchetes se vería así: [C:\Users\kurukuruh\Desktop\boost_1_88_0]. 

2. El comando arroja un archivo .exe, este archivo NO se debe abrir hasta que el Servidor Intermedio y Servidor Final estén compilando.


### 2. Compilar Servidor Final

Para compilar Servidor Intermedio, ingresar a la carpeta *ServidorFinal*.
Luego abrir una terminal de VSC o el editor de texto que usted posea, y escribir el siguiente comando:

```
python main.py
```
OBS. En caso de que le arroje error la compilación, deberá abrir el entorno virtual, y se hace compilando el siguiente comando.

```
.venv\Scripts\activate
```
Posterior a eso, compilar nuevamente el archivo *main.py*

Finalmente en su terminal, mostrará un link de la siguiente manera:
```
Running on http://127.0.0.1:5000
```
Presione el link y NO cierre esa ventana. 


### 3. Compilar Servidor Intermedio

Para compilar Servidor Intermedio, ingresar a la carpeta *ServidorIntermedio*.
Luego abrir una nueva terminal de VSC o el editor de texto que usted posea, y escribir el siguiente comando:

```
python serv_intermedio.py
```

OBS. Si nuevamente le arroja un error de compilación, hacer lo mismo que en el paso 2. Es decir, abrir el entorno virtual y compilar el archivo *serv_intermedio.py*

```
.venv\Scripts\activate
python serv_intermedio.py
```

Finalmente en su terminal, mostrará el siguiente mensaje:

```
[SERVER] Esperando conexión del sensor en el puerto 9000...
```
Lo cuál es completamente normal, y es correcto, porque esperando información del sensor, y es el archivo .exe que debemos ejecutar en el siguiente paso. 


### 4. Ejecutar el archivo .exe

En una ventana nueva, ingresar a la carpeta *ClienteSensor/scr*. Luego abrir una nueva terminal de VSC o el editor de texto que usted posea, y escribir el siguiente comando:

```
./sensor.exe
```
En su terminal, mostrará el siguiente mensaje. 

```
[CLIENTE] Datos envíados correctamente (26 bytes)
```

Luego en la terminal del Servidor Intermedio podrá ver el siguiente mensaje. 

```
[SERVER] Esperando conexión del sensor en el puerto 9000...
[SERVER] Conexión desde sensor: ('127.0.0.1', 55361)
[DEBUG] CRC recibido: 0x9cad4c83, CRC calculado: 0x9cad4c83
[INFO] Datos recibidos:
 Sensor ID: 13
 Timestamp: Mon Jul  7 22:43:17 2025
 Temperatura: 800.5 °C
 Humedad: 1000.0 %
 Presión: 8.0 hPa
[SUCCESS] Datos reenviados al servidor final correctamente.
```

Como verá en el mensaje, el servidor intermedio recibió correctamente los datos y los reenvia al servidor final. 

Luego para visualizarlos, debe ir a la página web que abrió en el paso 2, y recargar la página (como lo haría con cualquier página web), y podrá ver la información del sensor. 

En este caso, la información del sensor 13. 


### 5. Compilar Cliente de Consulta. 

Para compilar Cliente de Consulta, ingresar a la carpeta *ClienteConsulta*.
Luego abrir una nueva terminal de VSC o el editor de texto que usted posea, y escribir el siguiente comando:

```
python main.py
```

OBS. Si nuevamente le arroja un error de compilación, hacer lo mismo que en el paso 2. Es decir, abrir el entorno virtual y compilar el archivo *main.py*

```
.venv\Scripts\activate
python main.py
```

En caso de existir datos anomalos, en su terminal aparecerá el siguiente mensaje:

```
[ALERTA] (Variable Anómala) fuera de rango: (Valor anómalo) (Sensor X)
```

En nuestro caso se vería así

```
[ALERTA] Temperatura fuera de rango: 800.5 (Sensor 13)
```

Y en caso de no existir datos anómalos, no arrojará nada en la terminal.


