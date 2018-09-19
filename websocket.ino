/*

When the sketch is uploaded, the ESP8266 will reboot and create its own access point called AutoConnect AP. Connecting to that AP will bring up the 
list of available access points. Enter Username and PW for the access point and ESP module will save the credentials for you. 

When the module is restarted, it will connect to the AP that you chose and should be available on the network. You can get its IP from the Serial monitor 
or use mDNS library (it's uncommented by default, along with all usage of the library) to make the module discoverable.

To change colors on the module (Neopixels on Pin 2), simply go to the root URL of the web server.

*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h> 
#include <FastLED.h>
#include <ESP8266HTTPUpdateServer.h>
// #include <WiFiManager.h>
#include <Hash.h>

WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

#define NUM_LEDS 64
#define DATA_PIN D6

CRGB leds[NUM_LEDS];

int corazon[NUM_LEDS] = {
  0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 1, 0, 0, 1, 0, 0,
  0, 1, 0, 1, 1, 0, 1, 0,
  0, 1, 0, 0, 0, 0, 1, 0,
  0, 1, 0, 0, 0, 0, 1, 0,
  0, 0, 1, 0, 0, 1, 0, 0,
  0, 0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
};


uint8_t gHue = 0;
int mode = 0;
uint8_t * payloadArray;
const char* ssid = "C3P";
const char* password = "trespatios";
//const char* ssid = "UNE_6EB4";
//const char* password = "9857312100";
int contconexion = 0;

String pagina ="<html>"
"<head>"
"<script>"
"var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);"
"connection.onopen = function ()       { connection.send('Connect ' + new Date()); };"
"connection.onerror = function (error) { console.log('WebSocket Error ', error);};"
"connection.onmessage = function (e)   { console.log('Server: ', e.data);};"
"function sendRGB() {"
" var r = parseInt(document.getElementById('r').value).toString(16);"
" var g = parseInt(document.getElementById('g').value).toString(16);"
" var b = parseInt(document.getElementById('b').value).toString(16);"
" if(r.length < 2) { r = '0' + r; }"
" if(g.length < 2) { g = '0' + g; }"
" if(b.length < 2) { b = '0' + b; }"
" var rgb = '#'+r+g+b;"
" var corazon = ["
"  0, 0, 0, 0, 0, 0, 0, 0,  "
"  0, 0, 1, 0, 0, 1, 0, 0,"
"  0, 1, 0, 1, 1, 0, 1, 0,"
"  0, 1, 0, 0, 1, 0, 1, 0,"
"  0, 1, 0, 0, 1, 0, 1, 0,"
"  0, 0, 1, 0, 1, 1, 0, 0,"
"  0, 0, 0, 1, 1, 0, 0, 0,"
"  0, 0, 0, 0, 0, 0, 0, 0,"
" ];"
" var corazontexto = corazon.toString();"
" console.log('RGB: ' + rgb);"
" console.log('array puro: ' + corazon);"
" console.log('array tranform: ' + corazontexto);"
" connection.send(corazontexto);"
"}"
"</script>"
"</head>"
"<body>"
"LED Control:<br/><br/>"
"R: <input id='r' type='range' min='0' max='255' step='1' value='0' oninput='sendRGB();'/><br/>"
"G: <input id='g' type='range' min='0' max='255' step='1' value='0' oninput='sendRGB();'/><br/>"
"B: <input id='b' type='range' min='0' max='255' step='1' value='0'oninput='sendRGB();'/><br/>"
"</body>"
"</html>";
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
                mode = 0;
                // send message to client
                webSocket.sendTXT(num, "Connected");
            }
            break;
        case WStype_TEXT:
            Serial.printf("[%u] get Text: %s\n", num, payload);
            //USE.SERIAL.printf("First character of the paylod %s\n", payload[0]);
            //payloadArray = &payload[0];
            /*
            for(int i=0; i < NUM_LEDS; i++){
                //corazon[i] = payload[i];
                String wtf = (const char*) &payload;
                 if(wtf == ",") Serial.println("upcoming number");
                long swiche = strtol((const char *) &payload,NULL,16);
                Serial.print("valor convertido       ");
                Serial.println(swiche);
                 Serial.print("valor payload       ");
                Serial.println(wtf); 
              }
            */
            
           // for(int i = 0; i < NUM_LEDS; i++) {
              //(int) strtol( &payload, NULL, 16);
             // uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
             
              // leds[i] = rgb; //(long) strtol(&payload[1], NULL, 16);
              // corazon[i] = rgb; 
              // pinto un corazón
              // con un array de int donde tenga 1 le asigna a la matrix, cero asigna color negro, nada
              
              //if(corazon[i] == 1){
                //leds[i] = 0xAB01EE;
                //leds[i] = rgb;
              //} else {
              //  leds[i] = 0x000000;
              //}
              
             // Serial.println("Viene del websocket parseado: ");
             // Serial.println(rgb);
              
           // }
            
            // send message to client
            // webSocket.sendTXT(num, "message here");

            // send data to all connected clients
            // webSocket.broadcastTXT("message here");
            break;
        case WStype_BIN:
            Serial.printf("[%u] get binary lenght: %u\n", num, lenght);
            hexdump(payload, lenght);

            // send message to client
            // webSocket.sendBIN(num, payload, lenght);
            break;
    }

}

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    Serial.println();

    //WiFiManager wifiManager;
   
    //wifiManager.setAPStaticIPConfig( IPAddress(192,168,0,178), IPAddress(192,168,0,1), IPAddress(255,255,255,0));

    /* if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
    }
    */
    //delay(2000);
    //Serial.println("Connection established!");
    // Serial.println("local ip");
    //Serial.println(WiFi.localIP());

  // nos conectamos a la web
    WiFi.mode(WIFI_STA); //para que no inicie el SoftAP en el modo normal
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED and contconexion <50) { //Cuenta hasta 50 si no se puede conectar lo cancela
        ++contconexion;

    delay(500);
    Serial.print(".");
  }
  if (contconexion <50) {
      //para usar con ip fija
      IPAddress Ip(192,168,1,178); // no ilvidar configurar segun como este ip 
      IPAddress Gateway(192,168,1,1); 
      IPAddress Subnet(255,255,255,0); 
      WiFi.config(Ip, Gateway, Subnet); 
      
      Serial.println("");
      Serial.println("WiFi conectado");
      Serial.println(WiFi.localIP());
  }
  else { 
      Serial.println("");
      Serial.println("Error de conexion");
  }

  
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    
    //if(MDNS.begin("esp8266")) {
    //    Serial.println("MDNS responder started");
    //}

    // handle index
    server.on("/", []() {
        server.send(200,"text/html", pagina);
    });

    httpUpdater.setup(&server);
    server.begin();

    // Add service to MDNS
    //MDNS.addService("http", "tcp", 80);
    //MDNS.addService("ws", "tcp", 81);
}

void loop() {
    webSocket.loop();
    server.handleClient();  
    FastLED.show();
    FastLED.delay(1000/120);
}

