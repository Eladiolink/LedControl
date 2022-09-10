#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h> 
#include <PubSubClient.h>

// REDE E MQTT
WiFiClient espClient;
PubSubClient client(espClient);
const char* ssid = "";
const char* password = "";
const char* mqtt_server = "";
long lastMsg = 0;
char msg[50];
int value = 0;
char * Sub = "Color";

String data = "branco";
String lastData = "";
String efeitos="";
#define LED_PIN 05
#define LED_COUNT 30
Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int brilho = 255;
uint32_t vermelho = pixels.Color(255, 0, 0);
uint32_t verde = pixels.Color(0, 255, 0);
uint32_t azul = pixels.Color(0, 0, 255);
uint32_t branco = pixels.Color(255, 255, 255);
uint32_t ciano = pixels.Color(0, 255, 255);
uint32_t amarelo = pixels.Color(255, 255, 0);
uint32_t magenta = pixels.Color(255, 0, 255);
uint32_t preto = pixels.Color(0, 0, 0);

void setup() {
   Serial.begin(9600);
  pixels.begin();
  pixels.setBrightness(brilho);
  pixels.show();
  mudarCor(data);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if(efeitos=="faded")
    faded(5);
  
  else if(efeitos=="snake")
    kamy_mary(100);
  else 
     mudarCor(data);
    
  Serial.println(data);
}

void mudarCor(String color){
  if(color == "vermelho"){
     ledsOn(vermelho);
  }
  
  else if(color == "verde"){
    ledsOn(verde);
  }
  
  else if(color == "azul"){
    ledsOn(azul);
  }
  
  else if(color == "branco"){
    ledsOn(branco);
  }
  
  else if(color == "ciano"){
    ledsOn(ciano);
  }
  
  else if(color == "amarelo"){
    ledsOn(amarelo);
  }
  
  else if(color == "magenta"){
    ledsOn(magenta);
  }

  else if(color == "preto"){
    ledsOn(preto);
  }
  else {
    setBrilho(color);
    data = lastData;
  }
  
}

void diminuirBrilho(){
    if(brilho==0)
      return;
      
    brilho-=51;
    pixels.setBrightness(brilho);
    pixels.show();
}

void aumentarBrilho(){
    if(brilho==255)
      return;
    
    brilho+=51;
    pixels.setBrightness(brilho);
    pixels.show();
}

void ledsOn(uint32_t color){
  pixels.fill(color,0,LED_COUNT);
  pixels.show();
}

void setBrilho(String brilho){
   int num= brilho.toInt();
   pixels.setBrightness(num);
   pixels.show();
}

// EFEITOS


void faded(int lay){
  for(int i=brilho;i>=0;i--){
    pixels.setBrightness(i);
    pixels.show();
    delay(lay);
  }

  for(int i=0;i<=brilho;i++){
    pixels.setBrightness(i);
    mudarCor(data);
    pixels.show();
    delay(lay);
  }
}

// Efeito de Cobra
void kamy_mary(int time){
    pixels.clear();
    pixels.show();
    for(int i=0;i<LED_COUNT-(LED_COUNT/3);i++){
      pixels.fill(cor(data),i,i+3);
      if(i>0)
        pixels.fill(cor("preto"),i-1,i+1);
      pixels.show();
      delay(time);
    }
}

uint32_t cor(String color){
      if(color == "vermelho")
        return vermelho;
        
      if(color == "verde")
        return verde;
     
     if(color == "azul")
        return azul;
        
      if(color == "preto")
        return preto;
      
      if(color == "amarelo")
        return amarelo;
     
     if(color == "magenta")
        return magenta;
     
      if(color == "ciano")
        return ciano;
      
      if(color == "branco")
        return branco;
}

//WIFI CONNECTION
void setup_wifi() {
  delay(10);                                                            
  Serial.println();                                                     
  Serial.print("Conectando com ");                                       
  Serial.println(ssid);                                                 
             
  WiFi.begin(ssid, password);                                           
 
  while (WiFi.status() != WL_CONNECTED) {                               
    delay(500);                                                         
    Serial.print(".");                                                  
  }
  randomSeed(micros());
  Serial.println();                                                     
  Serial.println("WiFi conectado");                                     
  Serial.println("Endereço de IP: ");                                   
  Serial.println(WiFi.localIP()); 
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe(Sub);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  if(messageTemp.toInt())
    lastData = data;

  if(messageTemp=="snake" || messageTemp=="faded" || messageTemp=="static"){
    efeitos = messageTemp;
    return;
  }
  
  if(messageTemp != "++" && messageTemp!="--")
    data = messageTemp;
    
  if(messageTemp=="++"){
    aumentarBrilho();
    mudarCor(data);
  }
  
  if(messageTemp == "--"){
    diminuirBrilho();
    mudarCor(data);
  }

  
}
