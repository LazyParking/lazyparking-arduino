// Programa : Webserver com aviso de acionamento de botoes
// Autor : sandro schmitz
// Baseado no programa exemplo Webserver, de David Mellis e Tom Igoe

#include <SPI.h>
#include <Ethernet.h>

// A linha abaixo permite que voce defina o endereço fisico (MAC ADDRESS) 
// da placa de rede
byte mac[] = { 0x00, 0x16, 0x36, 0xE0, 0x4E, 0xDD };

// Os valores abaixo definem o endereço IP, gateway e máscara. 
// Configure de acordo com a sua rede.
IPAddress ip(192,168,0,22);         //Define o endereço IP
IPAddress gateway(192,168,1,1);    //Define o gateway
IPAddress subnet(255, 255, 255, 0); //Define a máscara de rede

// Inicializa a biblioteca da placa ethernet com as 
// configurações de IP fornecidas
EthernetServer server(80);

int botao1 = 6;   //Botao que aciona o led vermelho
int botao2 = 7;   //Botao que aciona o led verde
int pinoled=8;    //Pino ligado ao led vermelho
int pinoled2=9;   //Pino ligado ao led verde
int leitura = 0;  //Armazena o valor de leitura do botao1
int leitura2 = 0; //Armazena o valor de leitura do botao2
char mensagem[20];   //Mensagem a ser apresentada para o botao1
char mensagem2[20];  //Mensagem a ser apresentada para o botao2

void setup()
{
  pinMode(pinoled, OUTPUT); //Led
  pinMode(pinoled2, OUTPUT); //Led
  pinMode(botao1, INPUT);
  digitalWrite(botao1, HIGH);
  pinMode(botao2, INPUT);
  digitalWrite(botao2, HIGH);
  //Inicializa a conexao ethernet e o servidor web na porta 80
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void loop()
{
   //Verifica o status do Botao1 e imprime mensagem no browser
   leitura=digitalRead(botao1);   
   if (leitura == 0)
     { 
       digitalWrite(pinoled,1);
       char mensagem[] = "Box 503 ocupado!";
       char mensagem2[] = "Aguardando...";
       apresentadados(mensagem,mensagem2);
       delay(1000);   //Mantem o led aceso por 5 segundos
       //Imprime mensagem padrao, aguardando novo acionamento
       apresentadados("Aguardando...", "Aguardando...");  
       digitalWrite(pinoled,0);
     }

   //Verifica o status do Botao2 e imprime mensagem no browser
   leitura2=digitalRead(botao2); 
   if (leitura2 == 0)
     { 
       digitalWrite(pinoled2,1);
       char mensagem[] = "Aguardando...";
       char mensagem2[] = "Box 809 ocupado!";
       apresentadados(mensagem,mensagem2);   
       delay(1000);  //Mantem o led aceso por 5 segundos
       //Imprime mensagem padrao, aguardando novo acionamento
       apresentadados("Aguardando...", "Aguardando...");  
       digitalWrite(pinoled2,0);   
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
          client.print("Estado Box : ");
          client.print(msg);
          client.println("<br />");
          client.print("Estado Box : ");
          client.print(msg2);
          client.println("<br />");
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
