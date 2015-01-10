
// Programa : Webserver com aviso de vaga estacionamento ocupado
// Autor : sandro schmitz

#include <SPI.h>
#include <Ethernet.h>
#include <VirtualWire.h>


// A linha abaixo permite que voce defina o endereço fisico (MAC ADDRESS) 
// da placa de rede
byte mac[] = { 0x00, 0x16, 0x36, 0xE0, 0x4E, 0xDD };

// ****  CONFIGURA RECEPTOR ASK  *****
byte message[VW_MAX_MESSAGE_LEN];    // Armazena as mensagens recebidas
byte msgLength = VW_MAX_MESSAGE_LEN; // Armazena o tamanho das mensagens

// Os valores abaixo definem o endereço IP, gateway e máscara. 
// Configure de acordo com a sua rede.
IPAddress ip(192,168,0,22);         //Define o endereço IP
IPAddress gateway(192,168,0,1);    //Define o gateway
IPAddress subnet(255, 255, 255, 0); //Define a máscara de rede

// Enter the IP address of the server you're connecting to:
IPAddress server(192,168,0,55); 

// Inicializa a biblioteca da placa ethernet com as 
EthernetClient client;

int i = 0;
int contador = 0;   
char mensagem[50];   //Mensagem a ser apresentada 
char mensagem2[50];  //Mensagem a ser apresentada 


void setup() {
  Serial.begin (9600);
  vw_set_rx_pin(5); // Define o pino 5 do Arduino como entrada do pino de dados do receptor ASK
  vw_setup(500);    // Bits por segundo
  vw_rx_start();    // Inicializa o receptor
}

void loop() {
  //Inicializa a conexao ethernet e o servidor web 
  Ethernet.begin(mac, ip);

  // give the Ethernet shield a second to initialize:
  delay(1000);
  
  // ***** RECEPTOR DADOS ASK  ******
  uint8_t message[VW_MAX_MESSAGE_LEN];    
  uint8_t msgLength = VW_MAX_MESSAGE_LEN; 
  
  // Non-blocking
  if (vw_get_message(message, &msgLength)) {
    Serial.println("Recebido: ");

    for (int i = 0; i < msgLength; i++) {
      //Serial.println(i);
      Serial.write(message[i]);

      // ****  TESTES DOS BOX QUE VIERAM DO TX ASK  ****
      if (message[i] == '1'){    // define box ocupado
        delay(100);
        contador++;

        Serial.println("{\"boxId\":1,\"avaiable\":true}");
        char mensagem[] = "{\"boxId\":1,\"avaiable\":true}";
        apresentadados(mensagem);
        delay(100);
      }

      if (message[i] == '0') {    // define box livre
        delay(100);
        contador--;
        Serial.println("{\"boxId\":1,\"avaiable\":false}");
        char mensagem[] = "{\"boxId\":1,\"avaiable\":false}";
        apresentadados(mensagem);
        delay(100);
      }
    }

    Serial.println();
    Serial.print("Vagas: ");
    Serial.println(contador);
    contador = 0;
  }
}

void apresentadados(char msg[]) {
  // if there are incoming bytes available 
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  //connect to server
  Serial.println("connecting...");
  
  if (client.connect(server, 3030)) {
    // if you get a connection, report back via serial:
    Serial.println("connected");
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  delay(1000);
  
  if (client.connected()) {
    delay(1000);
    //send message to server
    client.print(msg);
  }

  //disconnect
  Serial.println("disconnecting");
  client.stop();
}
