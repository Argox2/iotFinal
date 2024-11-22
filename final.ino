#include "WiFi.h"
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

const char* ssid = "gaelr-hotspot";
const char* password = "";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                      void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_DATA) {
    String msg = "";
    for (size_t i = 0; i < len; i++) {
      msg += (char) data[i];
    }
    Serial.printf("Mensaje recibido: %s\n", msg.c_str());
    if (msg.startsWith("MOVE_")) {
      // Reenviar el mensaje a todos los clientes conectados
      ws.textAll(msg);
    }
  }
}


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

  server.on("/control", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/control.html", "text/html");
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
  
  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);


  // Iniciar servidor. 
  server.begin();

}

void loop() {
  ws.cleanupClients();
}
