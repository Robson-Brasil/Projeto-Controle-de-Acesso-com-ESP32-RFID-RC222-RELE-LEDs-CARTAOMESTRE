//Bibliotecas

#include <EEPROM.h>             // Biblioteca para ler e escrever dados na memória EEPROM, usada para armazenar os UIDs dos cartões/tag RFID.
#include <SPI.h>                // Biblioteca para comunicação via protocolo SPI, necessária para o funcionamento do módulo RFID RC522.
#include <MFRC522.h>            // Biblioteca para dispositivos RFID da família Mifare, como o RC522, utilizada para ler e manipular tags RFID.
#include <LiquidCrystal_I2C.h>   // Biblioteca para controlar displays LCD conectados via I2C, facilitando a exibição de informações no display.
#include <Wire.h>               // Biblioteca para comunicação I2C (dois fios), utilizada por dispositivos como o display LCD e outros periféricos I2C.
#include <ESPmDNS.h>            // Biblioteca para mDNS, permitindo acessar o ESP32 na rede local através de um nome de domínio (ex: "meu-dispositivo.local").
#include <NetworkUdp.h>         // Biblioteca para comunicação via protocolo UDP, usada para transmissões rápidas e sem confirmação de pacotes na rede.
#include <ArduinoOTA.h>         // Biblioteca para atualização de firmware via OTA (Over-the-Air), permitindo enviar novos códigos para o ESP32 através da rede Wi-Fi.
#include <WiFi.h>               // Biblioteca para gerenciar a conexão Wi-Fi no ESP32, permitindo conectar o dispositivo à rede e realizar transmissões de dados.
