# PRACTICA3
## Codi sencer
```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>

// SSID & Password
const char* ssid = "Xiaomi_11T_Pro"; // Enter your SSID here
const char* password = "12345678"; //Enter your Password here
WebServer server(80); // Object of WebServer(HTTP port, 80 is defult)
void handle_root(void);
String getHtmlContentFromFile(const char* filename) {
  // Intenta abrir el archivo en modo lectura
  File file = SPIFFS.open(filename, "r");

  // Verifica si el archivo se abrió correctamente
  if (!file) {
    Serial.println("No se pudo abrir el archivo " + String(filename));
    return ""; // Devuelve una cadena vacía en caso de error
  }

  // Lee el contenido del archivo en una cadena
  String fileContent = file.readString();

  // Cierra el archivo
  file.close();

  // Devuelve el contenido del archivo
  return fileContent;
}

void setup() {
  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
        Serial.println("An error occurred while mounting SPIFFS");
        return;
    }

    Serial.println("SPIFFS mounted successfully");

// Abre el directorio raíz
    File root = SPIFFS.open("/");
    if (!root) {
        Serial.println("Failed to open directory");
        return;
    }

    Serial.println("Files in SPIFFS:");
    File file = root.openNextFile();
    while (file) {
        Serial.print("  ");
        Serial.println(file.name());
        file = root.openNextFile();
    }

  Serial.println("Try Connecting to ");
  Serial.println(ssid);

  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP()); //Show ESP32 IP on serial

  server.on("/", handle_root);

  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}

void loop() {
  server.handleClient();
}

// HTML & CSS contents which display on web server
String HTML = "<!DOCTYPE html>\
<html>\
<body>\
<h1>My Primera Pagina con ESP32 - Station Mode &#128522;</h1>\
<img src= 'https://images7.memedroid.com/images/UPLOADED849/5fa07a82be0e0.jpeg'>\
</body>\
</html>";

// Handle root url (/)
void handle_root() {
  String htmlContent = getHtmlContentFromFile("/web.html");
  server.send(200, "text/html", htmlContent);
  //server.send(200, "text/html", HTML);
}
```

## Explicació del codi per parts

### Incloure Llibreries Necessàries
```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>
```
Aquestes línies inclouen les llibreries necessàries per al funcionament del codi:

WiFi.h: per connectar-se a una xarxa Wi-Fi.
WebServer.h: per crear un servidor web.
Arduino.h: llibreria bàsica d'Arduino.
FS.h i SPIFFS.h: per gestionar el sistema de fitxers SPIFFS (SPI Flash File System) de l'ESP32.

### Variables Globals
```cpp
const char* ssid = "Xiaomi_11T_Pro";
const char* password = "12345678";
WebServer server(80);
```
ssid i password emmagatzemen les credencials de la xarxa Wi-Fi a la qual es connectarà l'ESP32.
server és l'objecte del servidor web que escolta a la port 80 (port HTTP per defecte).

### Funció per Llegir Contingut HTML de un Fitxer
```cpp
String getHtmlContentFromFile(const char* filename) {
  File file = SPIFFS.open(filename, "r");

  if (!file) {
    Serial.println("No se pudo abrir el archivo " + String(filename));
    return "";
  }

  String fileContent = file.readString();
  file.close();

  return fileContent;
}
```
Aquesta funció obre un fitxer especificat pel seu nom (filename) des del sistema de fitxers SPIFFS, llegeix el seu contingut i el retorna com una cadena de text. Si no es pot obrir el fitxer, imprimeix un missatge d'error i retorna una cadena buida.

### Setup
```cpp
void setup() {
  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
        Serial.println("An error occurred while mounting SPIFFS");
        return;
    }

    Serial.println("SPIFFS mounted successfully");

    File root = SPIFFS.open("/");
    if (!root) {
        Serial.println("Failed to open directory");
        return;
    }

    Serial.println("Files in SPIFFS:");
    File file = root.openNextFile();
    while (file) {
        Serial.print("  ");
        Serial.println(file.name());
        file = root.openNextFile();
    }

  Serial.println("Try Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_root);

  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}
```
1. Inicialització Serial i SPIFFS: S'inicia la comunicació serial a 115200 bps i es munta el sistema de fitxers SPIFFS. Si no es pot muntar, es mostra un missatge d'error i s'atura la configuració.
2. Llistat de Fitxers SPIFFS: S'obre el directori arrel ("/") i es llisten els fitxers presents a la consola serial.
3. Connexió Wi-Fi: Es connecta a la xarxa Wi-Fi utilitzant les credencials proporcionades. Durant la connexió, imprimeix punts a la consola serial fins que es connecta amb èxit, moment en què mostra l'adreça IP obtinguda.
4. Inicialització del Servidor Web: Configura el manejador per la ruta arrel ("/") que crida a la funció handle_root. Inicia el servidor web i imprimeix un missatge a la consola serial.

### Loop
```cpp
void loop() {
  server.handleClient();
}
```
Aquest bucle es crida contínuament per gestionar les sol·licituds del servidor web.

### Manejador de la Ruta Arrel
```cpp
void handle_root() {
  String htmlContent = getHtmlContentFromFile("/web.html");
  server.send(200, "text/html", htmlContent);
}
```
Aquesta funció s'executa quan es visita la ruta arrel del servidor ("/"). Llegeix el contingut del fitxer web.html des del sistema de fitxers SPIFFS i l'envia com a resposta HTTP amb el codi de resposta 200 (OK) i el tipus de contingut text/html.

Aquest és el funcionament del codi pas a pas. En resum, l'ESP32 crea un servidor web que serveix un fitxer HTML emmagatzemat al sistema de fitxers SPIFFS quan es visita la seva IP a través del navegador web.
