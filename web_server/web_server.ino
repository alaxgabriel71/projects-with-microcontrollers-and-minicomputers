/*
//Sensor de temperatura usando o LM35
const int LM35 = A0; // Define o pino de leitura
float temperatura; // Variável que armazenará a temperatura medida

void setup() {
  Serial.begin(9600); // inicializa a comunicação serial
}

//Função que será executada continuamente
void loop() {
  temperatura = (float(analogRead(LM35))*5/(1023))/0.01;
  Serial.print("Temperatura: ");
  Serial.println(temperatura);
  delay(2000);
}*/

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0x71, 0x81, 0x91, 0xA1, 0x51, 0x41
};

//IPAddressip(10, 6, 2, 63); //descomentar para ip estatico
// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):

EthernetServer server(80);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
  ; // wait for serial port to connect. Needed for Leonardo only
  }
  // start the Ethernet connection and the server:
  Ethernet.begin(mac); //dhcp. Se desejar estatico, passer tambem o ip Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() {
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
          client.println("Connection: close"); // the connection will be closed after completion of the response
          client.println("Refresh: 5"); // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of the analog input 0 pin
          float tempC=(float(analogRead(A0))*5/(1023))/0.01;
          client.print("Temperatura: ");
          client.print(tempC);
          client.println("<br/>");
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
