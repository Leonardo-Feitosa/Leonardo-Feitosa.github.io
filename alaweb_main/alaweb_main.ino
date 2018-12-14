// Bibliotecas
#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo1

// Parâmetros da rede
// Informacoes de endereço IP, gateway, máscara de rede
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x78, 0xEC }; //physical mac address (MAC)
byte ip[] = { 10, 80, 14, 150 }; // IP na LAN
byte gateway[] = { 10, 80, 1, 1 }; // internet access via router
byte subnet[] = { 255, 255, 240, 0 }; //subnet mask
EthernetServer server(80); //server port 80

String readString; // stores the HTTP request

void setup()
{
   Ethernet.begin(mac, ip, gateway, subnet); // Inicializa os parâmetros da rede
   
   //Verificar se há hardware Ethernet presente
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("O Shield Ethernet não foi encontrado. Desculpe, não é possível executar sem hardware. :(");
    while (true) {
      delay(1); // não faz nada, não adianta correr sem hardware Ethernet
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("O cabo Ethernet não está conectado.");
  }
     
   server.begin(); // diz ao servidor que comece a escutar a entrada de conexões
   myservo.attach(9);  //the pin for the servo1 control
  
   Serial.begin(9600); // for diagnostics
   Serial.println("Server servo/pin 9 - SE ALAWEB 1.0"); // so I can keep track of what is loaded
}

void loop()
{
    // Create a client connection
    // Verifca se há dados disponíveis para leitura a partir do servidor
    EthernetClient client = server.available();  // try to get client

    if (client) { // tem cliente?
        while (client.connected()) {
            if (client.available()) {   // dados do cliente disponíveis para leitura
                char c = client.read(); // leia 1 byte (caractere) do cliente
                readString += c;  // salve o pedido de HTTP 1 char de cada vez
                
                // a última linha da solicitação do cliente está em branco e termina com \n
                // responder ao cliente somente após a última linha recebida
                if (c == '\n') {

                  Serial.println(readString); // Imprimir requisição no monitor serial para depuração
                  
                  //enviar um cabeçalho de resposta http padrão
                  client.println("HTTP/1.1 200 OK");
                  client.println("Content-Type: text/html");
                  client.println("Connection: keep-alive");
                  client.println();
                  // Página HTML minificada
                  client.println("<!DOCTYPE html><html lang=\"pt-br\"><head><meta charset=\"UTF-8\"/><title>Sistema ALAWEB 1.0 - PSE</title><script type=\"text/javascript\" src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script><link rel=\"stylesheet\" type=\"text/css\" href=\"https://leonardo-feitosa.github.io/style.min.css\"/><link rel=\"shortcut icon\" type=\"image/x-icon\" href=\"https://raw.githubusercontent.com/Leonardo-Feitosa/Leonardo-Feitosa.github.io/master/ifcelogoico.ico\"></head><body><section><article><h2 class=\"titulo\">SISTEMA ALAWEB 1.0</h2><fieldset class=\"Classexibe\" id=\"exibe\"><legend class=\"exi\">Exibição</legend><p><input class=\"mostrarValor\" type=\"text\" id=\"mostrarValor\" size=\"2\" readonly=\"readonly\" value=\"00:00\"/></p></fieldset><fieldset><legend>Configuração</legend><p><label for=\"min\">Minutos: &nbsp;&nbsp;</label><input type=\"text\" id=\"min\" size=\"2\" maxlength=\"3\"/></p><p><label for=\"seg\">Segundos: </label><input type=\"text\" id=\"seg\" size=\"2\" maxlength=\"2\"/></p></fieldset><fieldset><legend>Comandos</legend><p class=\"allBtns\"><input class=\"btn\" type=\"button\" value=\"INICIAR\" id=\"btnIniciar\" onclick=\"IniciarCronometro('start')\" /> <input class=\"btn\" type=\"button\" value=\"PAUSAR\" id=\"btnPausar\" onclick=\"PausarCronometro()\" disabled=\"disabled\"/> <input class=\"btn\" type=\"button\" value=\"RESETAR\" id=\"btnResetar\" onclick=\"ResetarCronometro()\" disabled=\"disabled\"/></p></fieldset><fieldset><legend>Ativar Sistema</legend><p class=\"allBtns\"><input class=\"btn\" id=\"btnAtivar\" type=\"button\" value=\"ATIVAR\" onclick=\"getActionServo();\"/> </p></fieldset></article></section><script></script><script type=\"text/javascript\" src=\"https://leonardo-feitosa.github.io/script.min.js\"></script></body></html>");
                 
                  delay(10); // dê tempo ao navegador da web para receber os dados
                  client.stop(); // feche a conexão
                                  
                    // controle do pino arduino
                    if(readString.indexOf("ajax_alaweb") > 0)
                    {
                      myservo.write(100);          
                      delay (500);
                      myservo.write(0);
                      delay (30);         
                     } 
                   readString = ""; // finished with request, empty string  // Clearing string for next read
                } // end if (c == '\n')
            } // end if (client.available())
        } // end while (client.connected())
    } // end if (client)
} //end loop()


