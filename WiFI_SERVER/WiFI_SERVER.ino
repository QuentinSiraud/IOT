// 📚 Librairies
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

// 🧱 Macros 
#define WIFI_SSID "nicolas" // Nom du réseau WiFi
#define WIFI_PASSWORD "abcd1234" // Mot de passe du partage de connexion
#define API_ENDPOINT "/api/data"

// 📌 Pins
const int WINDOW_SERVOR_PIN = 2; // Anciennement 5
const int DOOR_SERVOR_PIN = 4; // 🚪 Anciennement 13
const int LED_PIN = 12; // 💡
const int MOTION_SENSOR_PIN = 14; // 🏃
const int LEFT_BTN_PIN = 16; // 👈
const int HUMIDITY_SENSOR_PIN = 17; // 💧
const int GAS_SENSOR_PIN = 23; // 💨
const int BUZZER_PIN = 25; // 🔊
const int RGB_LED_PIN = 26; // 🚦
const int RIGHT_BTN_PIN = 27; // 👉
const int SMOKE_SENSOR_PIN = 34; // 🔥

// 🌟 Variables
bool ledState = false;

// 💻 Prototypes
/* WEB SERVER */
void setupWiFi();
void setupWebServer();
void setupPins();
//void updateLCDMessage(String message);
void sendHtml(); 
void sendApiData();

WebServer server(80);

void setup(){
  Serial.begin(921600);
  setupPins();          // Initialisation des broches
  setupWiFi();          // Connexion au WiFi
  setupWebServer();     // Initialisation du serveur web
}

void loop(){
  server.handleClient();
}

// Fonction pour initialiser les broches
void setupPins() {
  pinMode(LED_PIN, OUTPUT);
  Serial.println("Broches configurées");
}

// Fonction pour initialiser le WiFi
void setupWiFi() {
  // Connexion au WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connexion au WiFi : ");
  Serial.print(WIFI_SSID);

  // Attendre la connexion...
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Connecté !");

  // Afficher l'adresse IP
  Serial.print("Adresse IP de l'ESP32 : ");
  Serial.println(WiFi.localIP());
}

// Fonction pour initialiser le portail web
void setupWebServer() {
  // Route de la racine du portail web
  server.on("/", sendHtml);

  // Route pour allumer/éteindre la LED
  server.on("/toggle", []() {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    sendHtml();
  });

  // Route pour recevoir les données JSON
  server.on(API_ENDPOINT, sendApiData);

  // Démarrer le serveur
  server.begin();
  Serial.println("Serveur HTTP démarré");
}

// Fonction pour afficher un message sur l'écran LCD
/*
void updateLCDMessage(String message) {
  mylcd.clear();
  mylcd.setCursor(0, 0);
  mylcd.print(message);
}
*/

void sendApiData() {
  String jsonResponse = "{";
  jsonResponse += "\"ledState\": \"" + String(ledState ? "on" : "off") + "\",";
  jsonResponse += "\"ipAddress\": \"" + WiFi.localIP().toString() + "\",";
  jsonResponse += "\"uptime\": " + String(millis() / 1000);
  jsonResponse += "}";
  server.send(200, "application/json", jsonResponse);
}

void sendHtml() {
  String response = R"html(
    <!DOCTYPE html>
    <html>
      <head>
        <title>Contrôle LED via ESP32</title>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <style>
          html { font-family: sans-serif; text-align: center; }
          body { display: flex; flex-direction: column; align-items: center; }
          h1 { margin-bottom: 1.2em; }
          .btn { background-color: #ffa921; border: none; color: #fff; padding: 0.5em 1em;
                 font-size: 2em; text-decoration: none; }
          .btn.off { background-color: #826537; }
        </style>
        <script>
          // Fonction pour obtenir les données de l'ESP32 via AJAX
          function getData() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
              if (this.readyState == 4 && this.status == 200) {
                var data = JSON.parse(this.responseText);
                document.getElementById("led-status").innerHTML = data.ledState == "on" ? "Allumée" : "Éteinte";
                document.getElementById("ip-address").innerHTML = data.ipAddress;
                document.getElementById("uptime").innerHTML = data.uptime + " secondes";
              }
            };
            xhttp.open("GET", "API_ENDPOINT", true);
            xhttp.send();
          }

          // Appelle la fonction getData toutes les 2 secondes
          setInterval(getData, 2000);
        </script>
      </head>
      <body onload="getData()">
        <h1>Contrôle LED</h1>
        <p><strong>État de la LED :</strong> <span id="led-status"></span></p>
        <p><strong>Adresse IP :</strong> <span id="ip-address"></span></p>
        <p><strong>Uptime :</strong> <span id="uptime"></span></p>
        <a href="/toggle" class="btn LED_CLASS">LED_TEXT</a>
      </body>
    </html>
  )html";

  response.replace("API_ENDPOINT", String(API_ENDPOINT));  // Remplace la macro par la valeur réelle
  response.replace("LED_TEXT", ledState ? "Éteindre LED" : "Allumer LED");
  response.replace("LED_CLASS", ledState ? "" : "off");
  server.send(200, "text/html", response);
}
