#include "WiFi.h"
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

const char* ssid = "DIRECT-NS-Hotspot";
const char* password = "password";

AsyncWebServer server(80);


void setup() {
  
  Serial.begin(115200);
  
  // Inicializar SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("Error al montar SPIFFS");
    return;
  }

  // Conexión Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Conectando a Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado a la red Wi-Fi");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Rutas del servidor. 

  // Ruta para la pagina principal
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  // Ruta para CSS.
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Rutas para el juego snake.
  server.on("/snake", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/games/snake/snake.html", "text/html");
  });
  server.on("/snake.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/games/snake/snake.js", "application/javascript");
  });


  // Iniciar servidor. 
  server.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
}
