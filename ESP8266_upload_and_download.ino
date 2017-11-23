 /********************************************
 *This program demonstrates the use of ESP 8266 connected to ARDUINO board 
 *via serial link to upload and download data from Thingspeak Platform
 *Arduino communicates to ESP8266 via AT Commands.
 *For more info on ESP8266 AT commands visit: https://www.espressif.com/sites/default/files/documentation/4a-esp8266_at_instruction_set_en.pdf
 *
 *You are free to use and share this program with anyone, however the author
 *will not be held liable for any issues,problems or damage caused by your usage of this 
 *program.
 *
 *********************************************/
String apiKey="Your_Thingspeak_Write_API_Key";
String ssid="Your_SSID";
String password="Your_Wi-Fi_Password";

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200);
    Serial1.println("AT+RST\r\n");
    Serial1.println("AT+CWMODE=1\r\n");   // set esp8266 as client
    Serial1.println("AT+CWJAP=\""+ssid+"\",\""+password+"\"");  // set your Router SSID and password
    
}

void loop() {
    float val=analogRead(A0);
    float temp = (val/1024)*500;
    
   /*****Sending data to thingspeak channel*******************************************/
    /***Set TCP COnnection***/
    String cmd = "AT+CIPSTART=\"TCP\",\"";                  // TCP connection
    cmd += "184.106.153.149";                               // api.thingspeak.com
    cmd += "\",80";
    Serial1.println(cmd);
    Serial.println(cmd);
    Serial.println(Serial1.readString());

    /**prepare GET commmand **/
    String getStr = "GET /update?api_key=";   // prepare GET string
    getStr += apiKey;
    getStr +="&field1=";
    getStr += String(temp);
    getStr += "\r\n\r\n";
    
    delay(2000);
    /**send Length*/
    cmd = "AT+CIPSEND=";
    cmd += String(getStr.length());
    Serial1.println(cmd);
    Serial.println(cmd);
    Serial.println(Serial1.readString());
    String Serial1Data=Serial1.readString();
    Serial1.println(getStr);
    Serial.println(getStr);
    
    Serial1.println("AT+CIPCLOSE\r\n");
    delay(20000);


/*******Reading data from thingspeak Channel**********************************************************/
    cmd = "AT+CIPSTART=\"TCP\",\"";                  // TCP connection
    cmd += "184.106.153.149";                               // api.thingspeak.com
    cmd += "\",80";
    Serial1.println(cmd);
    Serial.println(cmd);
    Serial.println(Serial1.readString());
    getStr = "GET /channels/359719/fields/1.json?api_key=Your_Read_API_Key&result=2";   // prepare GET string, get your read api key from thingsspeak.
    getStr += "\r\n\r\n";
    
    delay(2000);
    /**send Length*/
    cmd = "AT+CIPSEND=";
    cmd += String(getStr.length());
    Serial1.println(cmd);
    Serial.println(cmd);
    Serial.println(Serial1.readString());
    Serial1Data=Serial1.readString();
    Serial1.println(getStr);
    Serial.println(getStr);
    String Receivedata =Serial1.readString();
    Serial.println(Receivedata);
    /*
     * Here we are detecting quote inside the incoming data from thingsspeak so 
     * as to get our data. You need to change this based on your incoming data.
     */
    int lastcp2 = Receivedata.lastIndexOf('"');
    int lastcp1 = Receivedata.lastIndexOf('"',lastcp2-1);

    Serial.print("The value written to Channel:");
    Serial.println(temp);
    Serial.print("The value received from channel:");
    Serial.println(Receivedata.substring(lastcp1+1,lastcp2));
      
    
    Serial1.println("AT+CIPCLOSE\r\n");
    

    delay(20000);
}
/******************End of Program******************/
