#include <EEPROM.h>             // Nós vamos ler e escrever UIDs do PICC na EEPROM.
#include <SPI.h>                // O módulo RC522 utiliza o protocolo SPI.
#include <MFRC522.h>            // Biblioteca para Dispositivos Mifare RC522
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ESPmDNS.h>            // Função: Essa biblioteca implementa o suporte mDNS no ESP8266/ESP32. Isso permite que você atribua um nome de host ao seu dispositivo na rede local, facilitando a descoberta de dispositivos na mesma rede por meio de um nome amigável, como "esp8266.local" ou "esp32.local", em vez de depender de endereços IP.
#include <ArduinoOTA.h>
#include <WiFi.h>
#include <WiFiUdp.h>
