#include <ESP8266WiFi.h>             //Libreria para usar el modulo WiFi

const char* ssid = "AXTEL-5ED7";     // Aqui van nuestros datos de nuestra red
const char* password = "45903527";

int ledPin = D1;                     // Pin a utilizar

WiFiServer server(80);

void conectar_wifi() {               //Función para conectarnos a Wifi, sirve como base para cualquier programa que usemos con WiFi
  WiFi.begin(ssid, password);
  Serial.print("Mac Address: ");
  Serial.println(WiFi.macAddress());
  while (WiFi.status() != WL_CONNECTED)
   {   delay(500);
       Serial.print(".");
   }
  Serial.print("\nWiFi connected: "); 
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200); //Inicio el puerto serie 
 
  pinMode(ledPin, OUTPUT);    //Pines de salida 
  digitalWrite(ledPin, LOW);  //iniciamos en off

  conectar_wifi();  //Nos conectamos a WiFi
    
  server.begin();                                // Iniciamos el servidor
  Serial.print("Server started in: ");
  Serial.println(WiFi.localIP());      // Imprimimos la IP
  
}

void loop() {
  // Compruebo si hay un cliente disponible (una petición)
  WiFiClient client = server.available();
  if (!client) {
    return; // En caso de no haber un cliente, no hago nada
  }
 
  // Espero hasta que el cliente realice una petición
  Serial.println("¡Nuevo cliente!");
  while(!client.available()){
    delay(1);
  }
 
  // Leo la primera linea de la petición del cliente
  String request = client.readStringUntil('\r'); // Leo hasta retorno de carro
  Serial.println(request); //Imprimo la petición
  client.flush(); //Limpio el buffer
 
  // Interpreto lo que he recibido
 
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
 
// Pongo ledPin al valor que ha solicitado el cliente en la petición
 
  // Devuelvo la respuesta al cliente -> Todo ha ido bien, el mensaje ha sido interpretado correctamente
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  
  // A partir de aquí creo la página en raw HTML
  client.println("<!DOCTYPE HTML>"); 
  client.println("<html>");
  client.print("<body style='background-color: DEEPSKYBLUE;'></body>");
  client.println("<head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");

  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #195B6A; border: none; color: white; padding: 8px 20px;");
  client.println("text-decoration: none; font-size: 24px; margin: 2px; cursor: pointer;}</style></head>");
  
  client.print("<br><div align='center'><h2>Local Server</h2>");
  client.println("<br><br>");
  
  client.print("Estado del LED: ");
  
  if(value == HIGH) {
    client.print("Encendido");
  } else {
    client.print("Apagado");
  }
  
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button class='button'> ON </button></a>"); // Los botones con enlace
  client.println("<a href=\"/LED=OFF\"\"><button class='button button2'>OFF</button></a>");
  client.println("</div></html>");
 
  delay(1);
  Serial.println("Cliente desconectado"); // Nos desconectamos del cliente
  Serial.println("");

}
