//João Pedro Souza Marinho
//Código para ligar TV
//Código adaptado apartir do código do professor

#include <WiFi.h>
#include <WiFiManager.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = 4;  

IRsend irsend(kIrLed);  .

#define sala 15
#define quarto 2
#define cozinha 4

// const char* ssid     = "ctdi-aranoua";
// const char* password = "aranoua@2023";

WiFiServer server(80);

void setup()
{
  Serial.begin(115200);

  pinMode(sala, OUTPUT);
  pinMode(quarto, OUTPUT);
  pinMode(cozinha, OUTPUT);

  irsend.begin();
 
  delay(10);

    WiFiManager wm;
    bool res;
  
    res = wm.autoConnect("IFAMESP32","ha150894"); 
    if(!res) {
        Serial.println("Failed to connect");
        
    } 
    else {   
        Serial.println("connected...yeey :)");
    }

  server.begin();

}

int value = 0;

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
            client.println("<a href=\"/TV\"\"><button>TV</button></a>");
            client.println("<br>");
            client.println("<br>");
            client.println("</center>");
            client.println("</body>");
            client.println("</html>");

            client.println();
          
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

        if (currentLine.endsWith("GET /TV")) {
            irsend.sendNEC(0x2FD48B7);
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
