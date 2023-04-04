# EVCC OLED-Statusdisplay

Ich möchte euch mein EVCC OLED-Statusdisplay vorstellen, das die aktuellen Informationen der PV-Anlage und des Elektrofahrzeugs über die [Rest API](https://docs.evcc.io/docs/reference/api#rest-api) abruft und im Sekundentakt aktualisiert.

![20230404_124741](https://user-images.githubusercontent.com/37591931/229777549-a896dd99-5a06-485f-b2a3-caa352d57e44.jpg) ![20230404_132116](https://user-images.githubusercontent.com/37591931/229777560-df0619e1-b3bd-4ab9-a783-7c1f3cccea69.jpg)

Die Hardware besteht aus einem ESP8266 und einem günstigen 128x64 SSD1306 I²C OLED-Display.

Die Programmierung erfolgte in Arduino C++, wobei meine Programmierfähigkeiten und Codequalität auf einem Anfängerniveau sind.

Leider bin ich erst jetzt auf den [EVCC E-Ink Monitor](https://github.com/powelllens/evcc_eink_monitor) #3814 gestoßen, der eine bessere Codequalität und mehr Funktionen bietet.

Beide Projekte haben aber ihre Vor- und Nachteile.

:white_check_mark: Live Daten im Sekundentakt
:white_check_mark: Einfacher Arduino Sketch
:white_check_mark: Anpassbar für andere Displays (z.B. HD44780 LCD)
:x: Konfiguration muss im Quellcode angepasst werden

![2023-04-04 12_46_49-OLED Emulator](https://user-images.githubusercontent.com/37591931/229775498-e58c3d42-3c2f-434f-9b00-d66f9d7b7302.png)

# Einrichtung
Für den Anfang setzte ich gewisse Grundkenntnisse mit der Arduino IDE und dem ESP8266 voraus.
In Zukunft werde ich die Anleitung erweitern.

I²C OLED an ESP8266 anschlißen ![(https://randomnerdtutorials.com/esp8266-0-96-inch-oled-display-with-arduino-ide](https://randomnerdtutorials.com/esp8266-0-96-inch-oled-display-with-arduino-ide/)

Im Arduino Sktech müssen follgende Zeilen angepasst werden:


