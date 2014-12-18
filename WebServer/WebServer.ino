#include <SPI.h>
#include <Ethernet.h>

//pins used are 10 11 12 13 4 -->don't use these for other purposes
//ss for wiznet w5100 --> 10
//ss for sd card --> 4



// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,3, 5);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);
String POStreq;

#define OUT1 2
#define OUT2 3
#define OUT3 4
const int pingPin = A0;
void setup() {
  pinMode(OUT1,OUTPUT);  
  pinMode(OUT2,OUTPUT);
  pinMode(OUT3,OUTPUT);
  
    pinMode(pingPin,OUTPUT);
  
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}



long find_distance(){
  


  long duration;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  
   pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
    // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return 10*duration / 29 / 2;
  
  
  
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
        POStreq +=c;

 
        
        //Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          
          


                  
          ProcessCheckbox(client);          
          POStreq=""; 
          
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
    Serial.println("client disonnected");
  }
}



void ProcessCheckbox(EthernetClient client)
{

    if (POStreq.indexOf("GET / HTTP/1.1") > -1) {  
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json");
          client.println("Connection: close");
          client.println();
            


 
            client.print(find_distance());
            client.print(",");  
            client.print(analogRead(2));
       

          
          client.print(",");
          //Serial.print(digitalRead(OUT1));
          //Serial.print(digitalRead(OUT2));
          //Serial.print(digitalRead(OUT3));
          client.print(digitalRead(OUT1) + digitalRead(OUT2)*2 + digitalRead(OUT3)*4);      
         
          
        
    }
  
  
  else{
  
  if (POStreq.indexOf("GET /?out1=1 HTTP/1.1") > -1) {  
  
      digitalWrite(OUT1,HIGH);
          Serial.println("out 1 on!!!");
        
    }
    
  else if (POStreq.indexOf("GET /?out1=0 HTTP/1.1") > -1) {  
      
      digitalWrite(OUT1,LOW);
      Serial.println("out 1 off!!!");
        
    }
    
    
     else if (POStreq.indexOf("GET /?out2=1 HTTP/1.1") > -1) {  
    
      digitalWrite(OUT2,HIGH);
        Serial.println("out 2 on!!!");
        
    }
    
 else if (POStreq.indexOf("GET /?out2=0 HTTP/1.1") > -1) {  
    
      digitalWrite(OUT2,LOW);
        Serial.println("out 2 off!!!");
        
    }
    
        
   else   if (POStreq.indexOf("GET /?out3=1 HTTP/1.1") > -1) {  
      
      digitalWrite(OUT3,HIGH);
      Serial.println("out 3 on!!!");
        
    }
    
  else if (POStreq.indexOf("GET /?out3=0 HTTP/1.1") > -1) {
      
      digitalWrite(OUT3,LOW);
      Serial.println("out 3 off!!!");
        
    }
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json");
          client.println("Connection: close");
          client.println();
    
    client.print(digitalRead(OUT1) + digitalRead(OUT2)*2 + digitalRead(OUT3)*4);      
    
  }
    
   
    
    
}



