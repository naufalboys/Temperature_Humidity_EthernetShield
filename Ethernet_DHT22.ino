#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"

#define DHTTYPE2 DHT22   // DHT 22  (AM2302), AM2321

#define DHTPIN  2
#define DHTPIN2 3
#define DHTPIN3 4
#define Relay   5
#define VCC     A0
#define VCC_2   A1
#define VCC_3   A2
#define GND     A3

DHT dht (DHTPIN, DHTTYPE2);
DHT dht2(DHTPIN2, DHTTYPE2);
DHT dht3(DHTPIN3, DHTTYPE2);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address

IPAddress server(192, 168, 50, 4);
IPAddress ip(192, 168, 50, 6);
EthernetClient client;
const char* host      = "192.168.50.4";

//const char* server  = "ifls.lmb.its.ac.id";
//const char* host    = "ifls.lmb.its.ac.id";

void setup() {
  Serial.begin(9600);
  pinMode(VCC, OUTPUT);
  pinMode(VCC_2, OUTPUT);
  pinMode(VCC_3, OUTPUT);
  pinMode(GND, OUTPUT);
  pinMode(Relay, OUTPUT);
  digitalWrite(VCC, HIGH);
  digitalWrite(VCC_2, HIGH);
  digitalWrite(VCC_3, HIGH);
  digitalWrite(GND, LOW);
  digitalWrite(Relay, HIGH);
  
  Serial.println("Initializing Ethernet Shield");

  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);
  }
  printIPAddress();
  delay(2000);
  Serial.println("Connecting...");
  dht.begin();
  dht2.begin();
  dht3.begin();
}

void printIPAddress()
{
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }

  Serial.println();
}

void loop()
{
  if (client.connect(server, 80))
  {
    Serial.println("Network Connected");

    float hum   = dht.readHumidity();
    float temp  = dht.readTemperature();

    float hum2  = dht2.readHumidity();
    float temp2 = dht2.readTemperature();

    float hum3  = dht3.readHumidity();
    float temp3 = dht3.readTemperature();

    if (isnan(hum) || isnan(temp))
    {
      Serial.println("Sensor 1 Error");
      return;
    }

    else if (isnan(hum2) || isnan(temp2))
    {
      Serial.println("Sensor 2 Error");
      return;
    }

    else if (isnan(hum3) || isnan(temp3))
    {
      Serial.println("Sensor 3 Error");
      return;
    }

    if ((hum >=55) || (hum2 >= 55) || (hum3 >= 55))
    {
      digitalWrite(Relay, LOW);
    }
    else
    {
      digitalWrite(Relay, HIGH);
    }
    
    Serial.print("Humidity  : ");
    Serial.print(hum);
    Serial.print(" %\t");
    Serial.print("Temperature : ");
    Serial.print(temp);
    Serial.print(" *C ");
    Serial.println();
    Serial.print("Humidity 2 : ");
    Serial.print(hum2);
    Serial.print(" %\t");
    Serial.print("Temperature 2 : ");
    Serial.print(temp2);
    Serial.print(" *C ");
    Serial.println();
    Serial.print("Humidity 3 : ");
    Serial.print(hum3);
    Serial.print(" %\t");
    Serial.print("Temperature 3 : ");
    Serial.print(temp3);
    Serial.print(" *C ");
    Serial.println();
    Serial.println("Mengirim Data ~");

    String url = "/radhumidity2/input.php";
    url += "?temperature=";
    url += temp;
    url += "&humidity=";
    url += hum;
    url += "&temperature2=";
    url += temp2;
    url += "&humidity2=";
    url += hum2;
    url += "&temperature3=";
    url += temp3;
    url += "&humidity3=";
    url += hum3;

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: cl1ose\r\n\r\n");

    client.stop();
  }

  else
  {
    Serial.println("Connection failed");
    setup();
  }
    delay(5000);
}