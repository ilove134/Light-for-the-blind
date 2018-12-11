 #include <ESP8266WiFi.h>
const char* ssid     = "HUAWEI Mate 10";
const char* password = "16567000";
const char* host = "api.heclouds.com";
int DS=13;//14
int sum=0;
int SH_CP=2;//11脚，上升时将数据移位
int ST_CP=0;//12脚，上升时将数据存入寄存器
int OE=12;//13脚，高电平时禁止输出
//int MR=15;//10脚，低电平时将数据清零，通常接Vcc
int capval=1;
/*String one;
String one1;
String one2;
String two;
String two1;
String two2;
String three;
String three1;
String three2;
String four;
String four1;
String four2;*/
char left[12]="00000000000";
char right[12]="00000000000";

uint8_t j=0;
uint32_t i=0;
void setup()
{
  pinMode(DS,OUTPUT);
  pinMode(SH_CP,OUTPUT);
  pinMode(ST_CP,OUTPUT);
  pinMode(OE,OUTPUT);
  pinMode(16,INPUT);
 // pinMode(MR,OUTPUT);
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  }

void shift(int x)
{
   digitalWrite(SH_CP,LOW);
   digitalWrite(DS,x);
   digitalWrite(SH_CP,HIGH);
}

void loop()
{
String arr="";
digitalWrite(OE,HIGH);                                    
int num=0;

delay(100);

 Serial.print("connecting to ");
 Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
String url = "https://api.heclouds.com/devices/505224927/datapoints?datastream_id=x HTTP/1.1\r\nHost:api.heclouds.com\r\napi-key:1QHE4IrbSJB5=87zBC6=HlpvGHw=\r\nConnection: close\r\n\r\n";
Serial.print("Requesting URL: ");
Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url);
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 50000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){ 
    String line = client.readStringUntil('\r');
    Serial.print(line);
    int len = line.length();  
    if (len > 0)
     {
        for (i = 0; i < len; i++)
        {
            if((char)line[i]=='u'&&(char)line[i+1]=='e'&&(char)line[i+2]=='"'&&(char)line[i+3]==':'&&(char)line[i+4]=='"')//检测value单词
               {
                    for(j=0;j!=-1;j++)
                      {
                          arr=arr+line[i+5+j];
                          if(line[i+5+j+1]=='"')
                          break;
                      }
                    break; 
               }
        }
       Serial.println();
       Serial.print("arr:");
       Serial.println(arr);
       Serial.println("closing connection");
    }}

    for(int i=0;arr[i]!='\0';i++)
          num++;
    Serial.println(num);
    int k;
 //   delay(100);

    for(int p=0;p<num/13;p++)//do nun(word) times
      {
         digitalWrite(ST_CP,LOW);
         for(int i=0;i<6;i++)
             {
                left[i]=arr[i+p*13];
                if (left[i]=='0') k=0; else k=1;
                shift(k);
                Serial.print(k);
             }
          for(int i=0;i<6;i++)
             {
                right[i]=arr[i+7+p*13];
             //  if(i==1){ if(right[i]==1)right[i]=0;else if (right[i]==0)right[i]=1;}
                if (right[i]=='0') k=0; else k=1; 
                
                shift(k);  
                Serial.print(k);
             }
             
       digitalWrite(OE,LOW);
       digitalWrite(ST_CP,HIGH);
       
       Serial.println("kai");
       //Serial.println(capval);
       
       delay(550);
        digitalWrite(OE,HIGH);
   
      while(digitalRead(16)==LOW)//close
         {
            //digitalWrite(ST_CP,LOW);
            // if(digitalRead(16))   capval=capval*(-1);
            digitalWrite(OE,HIGH);
            Serial.println("guan");
            delay(100);
         }
        //digitalWrite(OE,LOW);
        digitalWrite(ST_CP,LOW);
         for(int i=0;i<6;i++)
          {
              if(left[i]=='0') left[i]='1';  else left[i]='0';
              if(right[i]=='0')  right[i]='1';  else right[i]='0';
          }
       /*   shift(0);
          shift(0);
          shift(0);
          shift(0);
          shift(0);
          shift(0);
          shift(0);
          shift(0);
          shift(0);
          shift(0);
          shift(0);
          shift(0);*/
    for(int i=0;i<6;i++)
        {if (left[i]=='0') k=0;else k=1; 
         shift(k);}
    for(int i=0;i<6;i++)
       { if (right[i]=='0') k=0; else k=1; 
        shift(k); }   
    Serial.println("kai2");
     digitalWrite(ST_CP,HIGH);
     digitalWrite(OE,LOW);
     delay(550);
     digitalWrite(OE,HIGH);
     digitalWrite(ST_CP,LOW);
    for(int i=0;i<16;i++)
   {
     shift(1);
    }
    Serial.println("kai3");

   digitalWrite(ST_CP,HIGH);  
   digitalWrite(OE,LOW);
   delay(1620);
   digitalWrite(OE,HIGH);
   digitalWrite(ST_CP,LOW);
   
  
 } 
 delay(100);
 tone(13,1000);//D10
 delay(1000);
 noTone(13);
 
 while(digitalRead(16)==LOW)//close
         {
            //digitalWrite(ST_CP,LOW);
            digitalWrite(OE,HIGH);
            Serial.println("guan");
            delay(100);
         }
         
}

