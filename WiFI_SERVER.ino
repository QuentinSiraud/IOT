#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

// Informations du réseau WiFi du téléphone et mot de passe du partage de connexion
#define WIFI_SSID "nicolas"
#define WIFI_PASSWORD "abcd1234"

const int LED_PIN = 17;
bool ledState = false;

WebServer server(80);

// Prototypes
void sendHtml(); 
void sendData(); 

void setup(){
  Serial.begin(921600);
  Serial.println("***** DEBUT DE SETUP *****");
  pinMode(LED_PIN, OUTPUT);

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

  // Définir l'URL pour basculer l'état de la LED
  server.on("/", sendHtml);

  server.on("/toggle", []() {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    sendHtml();
  });

  server.on("/data", sendData);

  // Démarrer le serveur
  server.begin();
  Serial.println("Serveur HTTP démarré");

}

void loop(){
  server.handleClient();
}

void sendData() {
  String jsonResponse = "{";
  jsonResponse += "\"ledState\": " + String(ledState) + ",";
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
          .btn { background-color: #5B5; border: none; color: #fff; padding: 0.5em 1em;
                 font-size: 2em; text-decoration: none; }
          .btn.OFF { background-color: #333; }
        </style>
        <script>
          // Fonction pour obtenir les données de l'ESP32 via AJAX
          function getData() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
              if (this.readyState == 4 && this.status == 200) {
                var data = JSON.parse(this.responseText);
                document.getElementById("led-status").innerHTML = data.ledState ? "Allumée" : "Éteinte";
                document.getElementById("ip-address").innerHTML = data.ipAddress;
                document.getElementById("uptime").innerHTML = data.uptime + " secondes";
              }
            };
            xhttp.open("GET", "/data", true);
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
        <a href="/toggle" class="btn LED_TEXT">LED_TEXT</a>
      </body>
    </html>
  )html";
  response.replace("LED_TEXT", ledState ? "Éteindre" : "Allumer");
  server.send(200, "text/html", response);
}
