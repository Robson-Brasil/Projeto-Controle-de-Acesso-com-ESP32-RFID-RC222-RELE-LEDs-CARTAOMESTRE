#include <EEPROM.h>             // Nós vamos ler e escrever UIDs  do PICC na EEPROM.
#include <SPI.h>                // O módulo RC522 utiliza o protocolo SPI.
#include <MFRC522.h>            // Biblioteca para Dispositivos Mifare RC522
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ESPmDNS.h>
#include <NetworkUdp.h>
#include <ArduinoOTA.h>
#include <WiFi.h>