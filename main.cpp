#include <Arduino.h>
#include <WiFi.h>

const char * ssid = "OcLopez";          // Nombre red wifi: WEP WPA
const char * password = "1020304050";   // Contraseña red wifi

WiFiServer server(80);


String getTipoCifrado(wifi_auth_mode_t tipoCifrado){
  switch(tipoCifrado){
    case (WIFI_AUTH_OPEN): return "Abierta";
    case (WIFI_AUTH_WEP): return "WEP";
    case (WIFI_AUTH_WPA_PSK): return "WPA_PSK";
    case (WIFI_AUTH_WPA2_PSK): return "WPA2_PSK";
    case (WIFI_AUTH_WPA_WPA2_PSK): return "WPA_WPA2_PSK";
    case (WIFI_AUTH_WPA2_ENTERPRISE): return "WPA2_ENTERPRISE";
  }
}

void escanearRedes(){
  int numeroredes = WiFi.scanNetworks();
  
  Serial.print("Numero de redes WiFi encontradas: ");
  Serial.println(numeroredes);

  for(int i = 0; i < numeroredes; i++){
    Serial.print("Nombre de la red: ");
    Serial.println(WiFi.SSID(i));

    Serial.print("Fuerza de la señal: ");
    Serial.println(WiFi.RSSI(i));

    Serial.print("Codigo mac: ");
    Serial.println(WiFi.BSSIDstr(i));

    Serial.print("Tipo de cifrado: ");

    String tipoc = getTipoCifrado(WiFi.encryptionType(i));
    Serial.println(tipoc);
    Serial.println("_____________________________________________________");
  }
}

void conectarRedWifi(){
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Estableciendo Conexion WiFi...");

  while(WiFi.status() != WL_CONNECTED){ // Mientras no este conectado al wifi haga...
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado a la red WiFi ");
  Serial.println(ssid);
}

void setup() {
  Serial.begin(115200);

  pinMode(2,OUTPUT);

  escanearRedes();

  conectarRedWifi();
  Serial.print("Ip Local: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient cliente = server.available();

  if(cliente){
    String mensaje="";
    String mensaje1="";
    Serial.println("LLego un nuevo cliente");
    while (cliente.connected()){
      if(cliente.available()){
        char letra = cliente.read();
        Serial.write(letra);
        if(letra=='\n'){              // Es un caracter enter
          if (mensaje.length()==0){
            cliente.println("HTTP/1.1 200 OK");
            cliente.println("Content-type:text/html");
            cliente.println();          // Indicamos que terminamos de enviar la cabecera HTTP

            cliente.println("<!DOCTYPE html>");
            cliente.println("<html lang=\"es\">");
            cliente.println("<head>");
            cliente.println("<meta charset=\"UTF-8\">");
            cliente.println("<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">");
            cliente.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
            cliente.println("<style type=\"text/css\">");

            cliente.println(".botonprendido{");
            cliente.println("border: 0;");
            cliente.println("background: none;");
            cliente.println("display: block;");
            cliente.println("margin: 20px auto;");
            cliente.println("text-align: center;");
            cliente.println("border: 2px solid rgb(42, 181, 7);");
            cliente.println("padding: 14px 10px;");
            cliente.println("width: 200px;");
            cliente.println("outline: none;");
            cliente.println("color: white;");
            cliente.println("border-radius: 24px;");
            cliente.println("transition: 0.25s;");
	          cliente.println("font-family: Verdana;");
            cliente.println("text-decoration: none;}");

            cliente.println(".botonprendido:hover {");
	          cliente.println("background: #0bba0b;");
	          cliente.println("cursor: pointer;}");

            cliente.println(".botonapagado{");
            cliente.println("border: 0;");
            cliente.println("background: none;");
            cliente.println("display: block;");
            cliente.println("	margin: 20px auto;");
            cliente.println("text-align: center;");
            cliente.println("border: 2px solid rgb(150, 1, 1);");
            cliente.println("padding: 14px 10px;");
            cliente.println("width: 200px;");
            cliente.println("outline: none;");
            cliente.println("color: white;");
            cliente.println("border-radius: 24px;");
            cliente.println("transition: 0.25s;");
	          cliente.println("font-family: Verdana;");
            cliente.println("text-decoration: none;}");

            cliente.println(".botonapagado:hover {");
	          cliente.println("background: #ba250b;");
	          cliente.println("cursor: pointer;}");

            cliente.println(".box{");
	          cliente.println("width: 230px;");
	          cliente.println("padding: 30px;");
            cliente.println("position: absolute;");
            cliente.println("top: 50%;");
	          cliente.println("left: 50%;");
	          cliente.println("transform: translate(-50% , -50%);");
            cliente.println("background: rgb(73, 99, 214);");
            cliente.println("text-align: center;");
            cliente.println("border-radius: 24px;}");

            cliente.println("</style>");
            
            cliente.println("<title>Document</title>");
            cliente.println("</head>");
            cliente.println("<body>");
            cliente.println("<div class=\"divi\">");
            cliente.println("<form class=\"box\">");
            cliente.println("<a href=\"H\" class=\"botonprendido\">Encender</a>");
            cliente.println("<a href=\"L\" class=\"botonapagado\">Apagar</a>");
            cliente.println("</form>");
            cliente.println("</div>");
            cliente.println("</body>");
            cliente.println("</html>");
            
            cliente.println(); 
            break;
          }else{
            
            mensaje="";
          }
          
        }else if(letra != '\r'){
          mensaje = mensaje+letra; 
        }
        if(mensaje.endsWith("GET /H")){
          digitalWrite(2, HIGH);
        }
        if(mensaje.endsWith("GET /L")){
          digitalWrite(2, LOW);
        }
      }
    }
    Serial.println("Fin de conexion");
    cliente.stop();
  } 
}