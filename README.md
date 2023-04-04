# EVCC OLED-Statusdisplay

Das EVCC OLED-Statusdisplay zeigt Informationen der PV-Anlage und des Elektrofahrzeugs über die EVCC [Rest API](https://docs.evcc.io/docs/reference/api#rest-api) an.

![20230404_124741](https://user-images.githubusercontent.com/37591931/229777549-a896dd99-5a06-485f-b2a3-caa352d57e44.jpg) ![20230404_132116](https://user-images.githubusercontent.com/37591931/229777560-df0619e1-b3bd-4ab9-a783-7c1f3cccea69.jpg)

Die Hardware besteht aus einem ESP8266 und einem günstigen 128x64 SSD1306 I²C OLED-Display.

Die Programmierung erfolgte in Arduino C++, wobei meine Programmierfähigkeiten und Codequalität auf einem Anfängerniveau sind.

Leider bin ich erst jetzt auf den [EVCC E-Ink Monitor](https://github.com/powelllens/evcc_eink_monitor) gestoßen, der eine bessere Codequalität und mehr Funktionen bietet.

Beide Projekte haben aber ihre Vor- und Nachteile.

:white_check_mark: Live Daten im Sekundentakt

:white_check_mark: Einfacher Arduino Sketch

:white_check_mark: Anpassbar für andere Displays (z.B. HD44780 LCD)

:x: Konfiguration muss im Quellcode angepasst werden

## Einrichtung
Für den Anfang setzte ich gewisse Grundkenntnisse mit der Arduino IDE und dem ESP8266 voraus.
In Zukunft werde ich die Anleitung erweitern.

I²C OLED an ESP8266 anschlißen ![https://randomnerdtutorials.com/esp8266-0-96-inch-oled-display-with-arduino-ide](https://randomnerdtutorials.com/esp8266-0-96-inch-oled-display-with-arduino-ide/)

Im Arduino Sketch müssen folgende Zeilen angepasst werden:

`const char* ssid = "SSID"; <--- W-LAN SSID`

`const char* password = "PW"; <--- W-LAN Passwort`

`const char* url = "http://192.168.24.90:7070/api/state"; //evcc IP <--- bitte änderen`

`const byte loadpointId = 0; // ab evcc 0.111.0 Nr. 1-255 <--- bitte mit Browser in API nachschauen`

`const char* ntpServer = "0.europe.pool.ntp.org"; // <--- Zeitserver z.B. FritzBox IP oder 0.europe.pool.ntp.org`

## Bekannte Probleme
1. hard coded wifi password
2. Es werden nur die ersten 5 Zeichen des Fahrzeugname angezeigt

## To-do-Liste
[ ] mehrere Ladepunkte abwechselnd anzeigen
[ ] UI-Anpassung, da bin ich offen für Vorschläge
[ ] HD44780 Version
[ ] Animationen
[ ] Formatierung in kW
[ ] evt. OLED Bildschirmschoner ![https://hackaday.com/2019/04/23/a-year-long-experiment-in-oled-burn-in/](https://hackaday.com/2019/04/23/a-year-long-experiment-in-oled-burn-in/)

## Acknowledgements

Vielen dank an das EVCC Projekt [https://docs.evcc.io/docs/Home/](https://docs.evcc.io/docs/Home/)

### Third party libraries
* [Arduino core for ESP8266 WiFi chip](https://github.com/esp8266/Arduino)
* [Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library)
* [Adafruit_SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [SSD1306 OLED Emulator](https://github.com/sam-peach/SSD1306-OLED-Emulator)

![2023-04-04 12_46_49-OLED Emulator](https://user-images.githubusercontent.com/37591931/229775498-e58c3d42-3c2f-434f-9b00-d66f9d7b7302.png)
