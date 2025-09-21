#include <Wire.h>
#include <Adafruit_GFX.h>

#include <Adafruit_SSD1306.h>
//#include <Adafruit_SSD1306_EMULATOR.h> //OLED Emulator https://github.com/sam-peach/SSD1306-OLED-Emulator
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>
//#include "secrets.h" // <-- Datei mit W-LAN SSID und Passwort

// Replace with your network credentials
const char* ssid = ""; //<--- W-LAN SSID
const char* password = ""; //<--- W-LAN Passwort



// Define NTP Client to get time
const char* ntpServer = "0.europe.pool.ntp.org"; // <--- Zeitserver z.B. FritzBox IP oder 0.europe.pool.ntp.org
const long gmtOffset_sec = 0;      // Offset für Ihre Zeitzone in Sekunden
const int daylightOffset_sec = 0;  // Offset für Sommerzeit in Sekunden

unsigned long lastTime = 0;
//String BootTime = "null";

//evcc api https://docs.evcc.io/docs/reference/api#rest-api
// Replace with the URL of the API
const char* url = "http://192.168.2.2:7070/api/state"; //evcc IP <--- bitte änderen
unsigned long lastLoadpointSwitchTime = 0;
const int loadpointIds[] = {0, 1}; //alle loadpointIds mit komma getrennt eintragen z.B. {0, 1, 2}
const int numLoadpoints = sizeof(loadpointIds) / sizeof(loadpointIds[0]);
int currentLoadpointIndex = 0;
int loadpointId = loadpointIds[currentLoadpointIndex];



// OLED display parameters <-- bitte für denen OLED anpassen
  #define SCREEN_WIDTH 128     // OLED display width, in pixels
  #define SCREEN_HEIGHT 64     // OLED display height, in pixels
  #define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
  #define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  //Adafruit_SSD1306_EMULATOR display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //OLED Emulator https://github.com/sam-peach/SSD1306-OLED-Emulator

#define x 14  // Rand links

// Definiere das Icon
  #define iconWidth 10
  #define iconHeight 10
  static const unsigned char PROGMEM iconBitmapCar[] = { 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0xc9, 0x00, 0xff, 0xc0, 0xff, 0xc0, 0x9e, 0x40, 0x61, 0x80, 0x00, 0x00, 0x00, 0x00 };
  static const unsigned char PROGMEM iconBitmapSun[] = { 0x0c, 0x00, 0x40, 0x80, 0x1e, 0x00, 0x3f, 0x00, 0xbf, 0x40, 0xbf, 0x40, 0x3f, 0x00, 0x1e, 0x00, 0x40, 0x80, 0x0c, 0x00 };
  static const unsigned char PROGMEM iconBitmapHome[] = { 0x00, 0x00, 0x0c, 0x00, 0x1e, 0x00, 0x3f, 0x00, 0x7f, 0x80, 0xff, 0xc0, 0x7f, 0x80, 0x52, 0x80, 0x73, 0x80, 0x73, 0x80 };
  static const unsigned char PROGMEM iconBitmapBatt[] = { 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x80, 0x80, 0xaa, 0xc0, 0xaa, 0xc0, 0x80, 0x80, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00 };
  static const unsigned char PROGMEM iconBitmapGrid[] = { 0x1e, 0x00, 0x2d, 0x00, 0x0c, 0x00, 0x3f, 0x00, 0x4c, 0x80, 0x0c, 0x00, 0x12, 0x00, 0x1e, 0x00, 0x33, 0x00, 0x21, 0x00 };
  static const unsigned char PROGMEM iconBitmapEuro[] = { 0b00111100, 0b01000010, 0b10000000, 0b11111110, 0b10000000, 0b01000010, 0b00111100, 0b00000000};
  
//ADC_MODE(ADC_VCC); //https://arduino-esp8266.readthedocs.io/en/latest/libraries.html#wifi-esp8266wifi-library
//Serial.println(ESP.getVcc());

//Setup Http Client
  WiFiClient wifiClient;
  HTTPClient http;

void setup() {

// Convert compile date and time to string
  String compileDate = __DATE__;
  String compileTime = __TIME__;

// Enable Serial
  Serial.begin(9600);
  delay(1000);

// Enable OLED display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  //display.setRotation(0);
  display.display();
  delay(2000);  // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);       // Set text size. Increase number to increase size. //6px x 8px
  display.setTextColor(WHITE);  // Set text color.

// Connect to Wi-Fi network
  int i;
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("SSID: " + String(ssid));

    //display.setCursor(0, 20);

    switch (i % 4) {
      case 0:
        display.println(F("Connecting to WiFi"));
        Serial.println(F("Connecting to WiFi"));
        break;
      case 1:
        display.println(F("Connecting to WiFi."));
        Serial.println(F("Connecting to WiFi."));
        break;
      case 2:
        display.println(F("Connecting to WiFi.."));
        Serial.println(F("Connecting to WiFi.."));
        break;
      case 3:
        display.println(F("Connecting to WiFi..."));
        Serial.println(F("Connecting to WiFi..."));
        break;
    }
    i++;
    if (i > 20) {
      i = 0;
    }
    //display.setCursor(0, 40);
    display.println();
    display.println("EVCC URL:");
    display.println(String(url));
    //display.println("loadpointId: " + String(loadpointId));
    //display.setCursor(0, 50);
    display.println();
    display.println("compileDate: " + compileDate + " " + compileTime);
    
    display.display();
  }
  Serial.println("Connected to WiFi");



// Initialize NTP client
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  // set timezone to Berlin
  //setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1); // Setze die Zeitzone auf Mitteleuropäische Zeit (CET) und Sommerzeit (CEST)
  setenv("TZ", "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", 3);
  tzset();  // Wende die neue Zeitzone an

  while (!time(nullptr)) {
    delay(1000);
    Serial.println("Waiting for time synchronization...");
  }

  //time_t now = time(nullptr);  // Aktuelle Zeit abrufen
  //char timeStr[20];
  //strftime(timeStr, sizeof(timeStr), "%d.%m.%Y %H:%M:%S", localtime(&now));  // Formatieren des Zeit-Strings
  //BootTime = timeStr;                                                        // Serial.println(timeStr);

// Initialize http client
  http.begin(wifiClient, url);  // begin mit WiFiClient und URL

}

void loop() {
 
  if (millis() - lastTime >= 1000) {
    lastTime = millis();  // Speichern Sie die aktuelle Zeit
    //Serial.println(F("Runtime in s: ") + String(lastTime / 1000 / 60));
    // Nächste Aktion hier

    if (WiFi.status() == WL_CONNECTED) {
      
      //Output runtime
        //Serial.printf("Runtime: %lu min\n", lastTime / 1000 / 60);
        unsigned long runtime = millis() / 1000;
        unsigned int seconds = runtime % 60;
        unsigned int minutes = (runtime / 60) % 60;
        unsigned int hours = (runtime / 3600) % 24;
        unsigned int days = (runtime / 86400);

        Serial.printf("Laufzeit: %d Tage %02d:%02d:%02d\n", days, hours, minutes, seconds);

  // Alle 10 Sekunden Loadpoint wechseln
    if (millis() - lastLoadpointSwitchTime >= 10000) {
      lastLoadpointSwitchTime = millis();
      currentLoadpointIndex = (currentLoadpointIndex + 1) % numLoadpoints;
      loadpointId = loadpointIds[currentLoadpointIndex];
      Serial.printf("Wechsle zu Loadpoint ID: %d\n", loadpointId);
    }


      // Output WiFI RSSI
        // Lese die WiFi-Stärke aus und skaliere sie auf den Bereich 0-100%
        int rssi = WiFi.RSSI();
        int signal_strength = map(rssi, -100, -50, 0, 100);
        signal_strength = constrain(signal_strength, 0, 100);
        //Serial.printf("RSSI: %ddbm %d%%\n", rssi, signal_strength);

      // Output current time to serial monitor / OLED
      //Serial.println("Zeit Abfragen");  //Debug
      time_t now = time(nullptr);       // Aktuelle Zeit abrufen
      char timeStr[20];
      //strftime(timeStr, sizeof(timeStr), "%d.%m.%Y %H:%M:%S", localtime(&now));  // Formatieren des Zeit-Strings
      strftime(timeStr, sizeof(timeStr), "%H:%M:%S", localtime(&now));  // Formatieren des Zeit-Strings
      //Serial.println(timeStr);

      // Make a HTTP request
      //Serial.println("HTTP Client");  //Debug

      // Führe einen HTTP-GET-Aufruf aus

      int httpCode = http.GET();
      //Serial.println(F("HTTP Status Code :") + String(httpCode));  //Debug
      Serial.printf("HTTP Status Code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK) {
        //Serial.println("payload = http.getString");  //Debug
        //Serial.println(ESP.getFreeHeap());
        String payload = http.getString();
        //Serial.println(ESP.getFreeHeap());
        //Serial.println(payload);

        // Konvertiere die empfangenen Daten in ein JSON-Dokument
        //Serial.println("deserializeJson(doc, payload)");  //Debug
        int doclen = payload.length(); //Lange der JSON Datei emittteln
        //Serial.println(doclen);
        //Serial.println(ESP.getFreeHeap());
        DynamicJsonDocument doc(doclen + 1000); //Größe der JSON Datei je nach größe der API z.B.: 2048 oder 4096 Byte
        deserializeJson(doc, payload);
        //Serial.println(ESP.getFreeHeap());
        //Serial.println("Extract the required data");  //Debug
        //Serial.println(doc.memoryUsage());
        // Extract the required data
          int batteryPower = doc["batteryPower"];
          //int invertedBatteryPower = -1 * batteryPower;
          int batterySoc = doc["batterySoc"];
          int gridPower = doc["grid"]["power"];
          int homePower = doc["homePower"];
          int pvPower = doc["pvPower"];
          float tariffGrid = doc["tariffGrid"].as<float>();
          
          int chargePower = doc["loadpoints"][loadpointId]["chargePower"];
          bool connected = doc["loadpoints"][loadpointId]["connected"];
          //bool connected = true;
          bool charging = doc["loadpoints"][loadpointId]["charging"];
          //charging = true;
          int vehicleSoc = doc["loadpoints"][loadpointId]["vehicleSoc"];
          int64_t chargeRemainingDurationSec = doc["loadpoints"][loadpointId]["chargeRemainingDuration"];       
          String mode = doc["loadpoints"][loadpointId]["mode"];
          //int vehicleSoc = 80;
          //String vehicleName = "Gastfahrzeug";
          String vehicleName = doc["loadpoints"][loadpointId]["vehicleName"];
        //auf rechtbündig und Links Formatieren
          //Serial.println("auf rechtbündig Formatieren"); //Debug
          char homePowerStr[7];                     //7 = Anzahl der Stellen
          sprintf(homePowerStr, "%6d", homePower);  //homePowerStr = Augabe, %6d = Anzahl freier Stellen, homePower = Datenquelle

          char BatteryPowerStr[10];                       //7 = Anzahl der Stellen
          sprintf(BatteryPowerStr, "%6d", batteryPower);  //homePowerStr = Augabe, %6d = Anzahl freier Stellen, homePower = Datenquelle

          char pvPowerStr[7];                    //7 = Anzahl der Stellen
          sprintf(pvPowerStr, "%6ld", pvPower);  //homePowerStr = Augabe, %6d = Anzahl freier Stellen, homePower = Datenquelle

          char gridPowerStr[7];                      //7 = Anzahl der Stellen
          sprintf(gridPowerStr, "%6ld", gridPower);  //homePowerStr = Augabe, %6d = Anzahl freier Stellen, homePower = Datenquelle

          char chargePowerStr[8];                        //7 = Anzahl der Stellen
          sprintf(chargePowerStr, "%6ld W", chargePower);  //homePowerStr = Augabe, %6d = Anzahl freier Stellen, homePower = Datenquelle

          char batterySocStr[4];                      //3 = Anzahl der Stellen
          sprintf(batterySocStr, "%3d", batterySoc);  //homePowerStr = Augabe, %2d = Anzahl freier Stellen, homePower = Datenquelle

          char tariffGridStr[3];                      //3 = Anzahl der Stellen
          //sprintf(tariffGridStr, "%.2f", tariffGrid);  //tariffGridStr = Augabe, %.2f = Nachkommastellen, tariffGrid = Datenquelle
          sprintf(tariffGridStr, "%3d", (int)(tariffGrid * 100));
          //Serial.println(tariffGridStr);
          char vehicleSocStr[4];                      //3 = Anzahl der Stellen
          sprintf(vehicleSocStr, "%3d", vehicleSoc);  //homePowerStr = Augabe, %2d = Anzahl freier Stellen, homePower = Datenquelle
          
          char vehicleNameStr[7];        //6 = Anzahl der Stellen + 1 (Fahrzeug Name wird nch 4 Zeichen abgeschitten)
          sprintf(vehicleNameStr, "%-6.6s", vehicleName.substring(0, 5) + ":");  //vehicleNameStr = Augabe, %-6.6s = Anzahl freier Stellen, vehicleName = Datenquelle

          //Serial.println(mode);
          char modeStr[8];
          if (mode == "off") {
              strcpy(modeStr, "Aus    ");
            } else if (mode == "pv") {
              strcpy(modeStr, "PV     ");
            } else if (mode == "minpv") {
              strcpy(modeStr, "Min+PV ");
            } else if (mode == "now") {
              strcpy(modeStr, "Schnell");
            } else {
            };
          //Serial.println(modeStr);
          
          //Output chargeRemainingDuration

          char chargeRemainingDurationStr[20];
          
          if (chargeRemainingDurationSec >= 0 && charging) {
            int seconds = chargeRemainingDurationSec % 60;
            int minutes = (chargeRemainingDurationSec / 60) % 60;
            int hours = chargeRemainingDurationSec / 3600;
          
            sprintf(chargeRemainingDurationStr, "%02d:%02d:%02d h", hours, minutes, seconds);
            //Serial.printf("Restzeit: %s\n", chargeRemainingDurationStr);
            //Serial.print("Sekunden: ");
            //Serial.println(chargeRemainingDurationSec);
          }



        // Display the extracted data on OLED display
            //Serial.println("Display the extracted data on OLED display");  //Debug
          
          String texts[] = {
            "Haus:      " + String(homePowerStr) + " W",
            "Solar:     " + String(pvPowerStr) + " W",
            "Akku: " + String(batterySocStr) + "% " + String(BatteryPowerStr) + " W",
            "Netz: " + String(tariffGridStr) + "c " + String(gridPowerStr) + " W",
            String(connected ? String(vehicleNameStr) + String(vehicleSocStr) + "% " + String(chargePowerStr) : "kein Auto"), //+ ":"
            String(charging ?  String(modeStr) + "  " + String(chargeRemainingDurationStr) : String(modeStr) + "    " + timeStr)
            //timeStr
            //BootTime,
            //"Laden: " + String(charging ? "Ja" : "Nein"),
            //"Auto: " + String(vehicleSoc) + "%  " + String(chargePower) + " W",
            //"Verbunden: " + String(connected ? "Ja " : "Nein") + String(charging ? "laden" : "")
          };
          display.clearDisplay();

          for (int i = 0; i < 6; i++) {
            switch (i) {
              //case 0: display.setCursor(x, 0); break;
              //case 1: display.setCursor(x, (i * 10)+2); break;
              case 4: display.setCursor(x, (i * 10) + 5); break;
              case 5: display.setCursor(x, (i * 10) + 7); break;
              default: display.setCursor(x, (i * 10));  //10px pro Zeile
            }
            display.println(texts[i]);
            //Serial.println(texts[i]);
          }
        
          //Serial.println("ICONS"); //Debug
            display.drawFastHLine(0, 54, 128, WHITE);  // draw a 1px high and 128px wide line
            display.drawBitmap(0, -1, iconBitmapHome, iconWidth, iconHeight, WHITE);
            display.drawBitmap(0, 10, iconBitmapSun, iconWidth, iconHeight, WHITE);
            display.drawBitmap(0, 20, iconBitmapBatt, iconWidth, iconHeight, WHITE);
            display.drawBitmap(0, 30, iconBitmapGrid, iconWidth, iconHeight, WHITE);
            display.drawBitmap(0, 44, iconBitmapCar, iconWidth, iconHeight, WHITE);
            //display.drawBitmap(70, 30, iconBitmapEuro, 8, 8, WHITE);

          //WiFi Stärke
            // Zeichne die Animation auf dem Display
            //Serial.println(signal_strength);
            // Zeichne diagonale Balken
            for (int i = 0; i < signal_strength / 10; i=i+2) {
              //display.fillRect(i * 2, 54, 1, 64 - i, WHITE);
              display.drawLine(i, 64, i, 64-i, WHITE);
            }

      } else {
        Serial.printf("HTTP request failed with error %d\n", httpCode);
      }
      //http.end();

    } else {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("kein W-LAN :(");
    }
    //Serial.println("display.display()"); //Debug
    display.display();
    //delay(1000);
    // Ersetzen Sie delay(1000) durch:
  }
}
