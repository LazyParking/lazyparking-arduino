// Programa : Webserver com aviso de vaga estacionamento ocupado
// Autor : sandro schmitz

#include <SPI.h>
#include <Ethernet.h>

// A linha abaixo permite que voce defina o endereço fisico (MAC ADDRESS) 
// da placa de rede
byte mac[] = { 0x00, 0x16, 0x36, 0xE0, 0x4E, 0xDD };

// Os valores abaixo definem o endereço IP, gateway e máscara. 
// Configure de acordo com a sua rede.
IPAddress ip(192,168,0,22);         //Define o endereço IP
IPAddress gateway(192,168,0,1);    //Define o gateway
IPAddress subnet(255, 255, 255, 0); //Define a máscara de rede

// Enter the IP address of the server you're connecting to:
IPAddress server(192,168,0,55); 

// Inicializa a biblioteca da placa ethernet com as 
EthernetClient client;


//int ledVerde = 45;    //Pino ligado ao led vermelho
//int ledVermelho = 44;   //Pino ligado ao led verde
char mensagem[20];   //Mensagem a ser apresentada 
char mensagem2[50];  //Mensagem a ser apresentada 

#define trigPin 12
#define echoPin 13
#define ledVerde 45
#define ledVermelho 44
 
void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledVerde, OUTPUT); //Led verde
  pinMode(ledVermelho, OUTPUT); //Led vermelho

  //Inicializa a conexao ethernet e o servidor web na porta 80
 
  Ethernet.begin(mac, ip);
 // Open serial communications and wait for port to open:
  
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 3030)) {
    Serial.println("connected");
  } 
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
   
 }
 
void loop() {
  long duration, distance; //http://arduino.cc/en/Reference/Long
  digitalWrite(trigPin, LOW);  //seta o pino 12 com um pulso baixo "LOW" ou desligado ou ainda 0
  delayMicroseconds(2); // delay de 2 microssegundos
 
  digitalWrite(trigPin, HIGH); //seta o pino 12 com pulso alto "HIGH" ou ligado ou ainda 1
  delayMicroseconds(10);  //delay de 10 microssegundos
  digitalWrite(trigPin, LOW); //seta o pino 12 com pulso baixo novamente
  duration = pulseIn(echoPin, HIGH);  //pulseIn lê o tempo entre a chamada e o pino entrar em high
  //Esse calculo é baseado em s = v . t, lembrando que o tempo vem dobrado
  //porque é o tempo de ida e volta do ultrassom
  distance = (duration/2) / 29.1;
  
     //Verifica o status do sensor Ultrasom e imprime mensagem no browser
  if (distance >= 30 || distance <= 0){    // define distancia 30cm
      delay(100);
      digitalWrite(ledVermelho, LOW);
      digitalWrite(ledVerde, HIGH);
       char mensagem[] = "Box 503 livre!\n";
       char mensagem2[] =  "Distancia Verde";
       apresentadados(mensagem,mensagem2);
       delay(100);   
              
  }
  else {
      delay(100);
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledVermelho, HIGH);
       char mensagem[] = "Box 503 Ocupado!\n";
       char mensagem2[] = "  ";
       apresentadados(mensagem,mensagem2);
       delay(100);   
       
     
  }
} 

  //  Teste Json 
  // { "id_drone": "1a2s3d", "boxes": [{ "id_box": "r4t5y6", "avaliable": 1 }] }


  // Rotina que recebe os valores de Mensagem e Mensagem2, 
  
void apresentadados(char msg[], char msg2[])  
{
  // if there are incoming bytes available 
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // as long as there are bytes in the serial queue,
  // read them and send them out the socket if it's open:
  
    if (client.connected()) {
      //Serial.print(msg);
      delay(1000);
      client.print(msg); 
      
    }
  
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    // do nothing:
    while(true);
  }
}
  

