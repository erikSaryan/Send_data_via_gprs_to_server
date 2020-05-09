#include<Wire.h>

#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26

void define_pins() {
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);
}

void print_response() {
  while(Serial2.available() != 0) {
    Serial.write(Serial2.read());
  }
}

void send_command(String command , int timeout = 10){
    timeout = timeout * 1000;
    unsigned long previous_time = millis();
    int timeout_counter = 0;
    Serial.println("Receiving: " + command + " 'ok'"); 
    Serial2.println(command);   
    while(true){
      if(Serial2.available()) {
        String str = Serial2.readStringUntil('\n');
        str = str.indexOf("OK");
        if( str == "0"){
          Serial.println(command + " 'OK' RECEIVED");
          break;
        }
      }
      if(millis() - previous_time >= timeout){
        previous_time = millis();
        ++timeout_counter;
        Serial.println("Ancav: " + String(timeout /1000) + " varkyan");
        Serial2.println(command);
        if(timeout_counter == 3){
          timeout_counter = 0;
          Serial.println("Timeout expiered");
          break;
        }
      }
    }
}
void send_tcp_command(String command , int timeout = 10){
    timeout = timeout * 1000;
    unsigned long previous_time = millis();
    int timeout_counter = 0;
    Serial.println("Receiving: " + command + " 'ok'"); 
    Serial2.println((char)26);    
    while(true){
      if(Serial2.available()) {
        String str = Serial2.readStringUntil('\n');
        str = str.indexOf("SEND OK");
        if(str == "0"){
          Serial.println(command + " 'OK' RECEIVED");
          Serial.println("Send");
          break;
        }
      }
      if(millis() - previous_time >= timeout){
        previous_time = millis();
        ++timeout_counter;
        if(timeout_counter == 3){
          timeout_counter = 0;
          Serial.println("Can't send");
          break;
        }
      }
    }
}
void send_data() {
    send_command("AT+CSQ");
    send_command("AT+CBC");
    send_command("AT+CGATT=1");
    send_command("AT+CIPSTART=TCP, 81.16.10.129, 8071");
    delay(2500);
    Serial2.println("AT+CIPSEND"); // Open Server pipe
    delay(2500);
    Serial2.println("hello esp32");
    delay(2500);
    send_tcp_command( "char 26" ); // Send Ctrl+z for send data to server
    Serial.println("_______________________"); 
}
void setup(){
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  define_pins();
  delay(5000);
  Serial.println("Start");
}
void loop(){
    send_data();
    delay(15000);
    Serial.println("Sending again");
}
