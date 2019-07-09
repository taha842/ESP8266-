// Engr.Muhammad Taha
// 0330-8530186
// REX Electric Service
#include<SoftwareSerial.h>
SoftwareSerial mySerial(2,3); //setting Tx and Rx pins


String server=""; //variable for sending data to webpage
boolean No_IP=false; //variable to check for ip Address
String IP=""; //variable to store ip Address
char temp1='0';

int a=0;

String data;
   
void setup() 
{
  Serial.begin(115200);
  
  while(a==0){
    mySerial.begin(115200);
    Serial.println("Changing Baud to 9600...");
    establishConnection("AT+UART_DEF=9600,8,1,0,0",1500);
    a++;
  }
    
   
  // mySerial.begin(115200);
  // establishConnection("AT+CIOBAUD=9600",1000);
  mySerial.end();
  mySerial.begin(9600);
   
   wifi_init();
   //sendToServer();
   Serial.println("System Ready..");
   showIP();
}

void loop() 
{
  
  
   while(mySerial.available())
   {
    
      data=mySerial.readString();
      Serial.println(data); 
   }   
     
    /*
    int len=data.length();
    
    if (len>=20) {
       

      
      server = "<h1>(6) SES SHOW3 : ";
      server+="03332";
      server+="</h1>";
      sendData(server);
      len=0;
    }*/
}


void findIp(int time1) //check for the availability of IP Address
{
  int time2=millis();
  while(time2+time1>millis())
  {
    while(mySerial.available()>0)
    {
      if(mySerial.find("IP has been read"))
      {
        No_IP=true;
      }
    }
  }
}

void showIP()//Display the IP Address 
{
  IP="";
  char ch=0;
  while(1)
  {
    mySerial.println("AT+CIFSR");
    while(mySerial.available()>0)
    {
      if(mySerial.find("STAIP,"))
      {
        delay(1000);
        Serial.print("IP Address:");
        while(mySerial.available()>0)
        {
          ch=mySerial.read();
          if(ch=='+')
          break;
          IP+=ch;
        }
      }
      if(ch=='+')
      break;
    }
    if(ch=='+')
    break;
    delay(1000);
  }
  Serial.print(IP);
  Serial.print("Port:");
  Serial.println(80);
}

void establishConnection(String mySerialand, int timeOut) //Define the process for sending AT mySerialands to module
{
  int q=0;
  while(1)
  {
    Serial.println(mySerialand);
    mySerial.println(mySerialand); 
    while(mySerial.available())
    {
      if(mySerial.find("OK"))
      q=8;
    }
    delay(timeOut);
    if(q>5)
    break;
    q++;
  }
  if(q==8)
  Serial.println("OK");
  else
  Serial.println("Error");
}

void wifi_init() //send AT mySerialands to module
{
 
      establishConnection("AT",100);
      
      establishConnection("AT+CWMODE=3",100);
      establishConnection("AT+CWQAP",100);  
     // establishConnection("AT+CIOBAUD=9600", 2000);
      //establishConnection("AT+RST",5000);
      findIp(5000);
      if(!No_IP)
      {
        Serial.println("Connecting Wifi....");
        establishConnection("AT+CWJAP=\"SES_KHI\",\"12345678\"",7000);         //provide your WiFi username and password here
      }
      else
        {
        }
      Serial.println("Wifi Connected"); 
     // showIP();
      establishConnection("AT+CIPMUX=1",5000);
      establishConnection("AT+CIPSERVER=1,80",7000);
}

void sendData(String server1)//send data to module
{
     int p=0;
     while(1)
     {
      unsigned int l=server1.length();
      Serial.print("AT+CIPSEND=0,");
      mySerial.print("AT+CIPSEND=0,");
      Serial.println(l+2);
      mySerial.println(l+2);
      delay(100);
      Serial.println(server1);
      mySerial.println(server1);
      while(mySerial.available())
      {
        //Serial.print(Serial.read());
        if(mySerial.find("OK"))
        {
          p=11;
          break;
        }
      }
      if(p==11)
      break;
      delay(100);
     }
}

