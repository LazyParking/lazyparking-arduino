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

// Inicializa a biblioteca da placa ethernet com as 
// configurações de IP fornecidas
EthernetServer server(80);

//int ledVerde = 45;    //Pino ligado ao led vermelho
//int ledVermelho = 44;   //Pino ligado ao led verde
char mensagem[20];   //Mensagem a ser apresentada 
char mensagem2[20];  //Mensagem a ser apresentada 

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
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
   
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
       char mensagem[] = "Box 503 livre!";
       char mensagem2[] =  "Distancia Verde";
       apresentadados(mensagem,mensagem2);
       delay(100);   
       //Imprime mensagem padrao, aguardando novo acionamento
       apresentadados("Aguardando...", "Aguardando...");  
       
  }
  else {
      delay(100);
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledVermelho, HIGH);
       char mensagem[] = "Box 503 Ocupado!";
       char mensagem2[] = "Distancia Vermelho  " ;
       apresentadados(mensagem,mensagem2);
       delay(100);   
       //Imprime mensagem padrao, aguardando novo acionamento
       apresentadados("Aguardando...", "Aguardando...");  
     
  }
}  
  // Rotina que recebe os valores de Mensagem e Mensagem2, 
// imprimindo o resultado no browser

void apresentadados(char msg[], char msg2[])  
{
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          // the connection will be closed after completion of 
          // the response
          client.println("Connection: close");  

          // refresh the page automatically every 5 sec
          client.println("Refresh: 0");  
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          client.println("<head><title>Lazy Parking</title></head>");
          client.println("<body>");
          client.print("Estado Box : ");
          client.print(msg);
          client.println("<br />");
          client.print("Distancia: ");
          client.print(msg2);
          client.println("<br /></body>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
 }
  


