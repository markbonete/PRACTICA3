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