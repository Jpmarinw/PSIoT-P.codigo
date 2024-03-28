// João Pedro Souza Marinho
// Código usando ESP32
// código pra ligar 3Leds(Sala, Quarto e Cozinha)
// Código adatado baseado no códido do professor

#include <WiFi.h>

// Definindo as portas usadas no ESP32
#define sala 15
#define quarto 2
#define cozinha 4

// Definindo a rede usada
const char* ssid     = "ctdi-aranoua";
const char* password = "aranoua@2023";

WiFiServer server(80);

// Parte adaptada a partir do código do professor
// Para receber um endereço IP automático
void setup(){

    Serial.begin(115200);
    delay(10);

    pinMode(sala, OUTPUT);
    pinMode(quarto, OUTPUT);
    pinMode(cozinha, OUTPUT);
    
    WiFi.begin(ssid, password);

    Serial.println();
    Serial.println();
    Serial.print("Waiting for WiFi... ");

    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    server.begin();
}

void loop() {

  WiFiClient client = server.available();   

  if (client) {                             
    Serial.println("New Client.");           
    String currentLine = "";                
    while (client.connected()) {           
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        if (c == '\n') {                    

          if (currentLine.length() == 0) {
        
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<html>");
            client.println("<head>");
            client.println("<title> Automação Residencial </title>");
            client.println("<meta charset=\"utf-8\">");
            client.println("<meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\">");
            client.println("</head>");
            client.println("<body>");
            client.println("<center>");
            client.println("<br>");

            client.println("<a href=\"/SALA\"\"><button>Sala</button></a>");
            client.println("<br>");
            client.println("<br>");
            client.println("<a href=\"/QUARTO\"\"><button>Quarto</button></a>");
            client.println("<br>");
            client.println("<br>");
            client.println("<a href=\"/COZINHA\"\"><button>Cozinha</button></a>");
            client.println("<br>");
            client.println("<br>");
            client.println("</center>");
            client.println("</body>");
            client.println("</html>");

            client.println();
  
            break;
          } else {   
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }


        if (currentLine.endsWith("GET /SALA")) {
          digitalWrite(sala, !digitalRead(sala));
        }
        
        if (currentLine.endsWith("GET /QUARTO")) {
          digitalWrite(quarto, !digitalRead(quarto));
        }
        
        if (currentLine.endsWith("GET /COZINHA")) {
          digitalWrite(cozinha, !digitalRead(cozinha));
        }
      }
    }
    
    client.stop();
    Serial.println("Client Disconnected.");

  }
}
