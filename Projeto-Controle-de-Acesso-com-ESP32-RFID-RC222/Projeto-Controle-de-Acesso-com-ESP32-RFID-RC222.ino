/*
  IoT - Automação Residencial
  Controle de Acesso por RFID
  Cadastramento por Cartão Mestre
  Autor : Robson Brasil
  Dispositivo : ESP32 WROOM32 38 Pinos
  Módulo RFID RC-522
  LCD I2C 20x4
  Atualização via OTA
  Versão : 1.0 - Realise Candidate
  Última Modificação : 14/10/2024
  Preferences--> Aditional boards Manager URLs:
                           http://arduino.esp8266.com/stable/package_esp8266com_index.json,
                           https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
  Download Board ESP32 v.(x.x.x): (Obs. na versão 2.0.8 a biblioteca <MFRC522.h> está com problemas no ESP32)
  -------------------------------------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------------------------------------
  Se apresentar esse erro "  c:\Users\TEU USÁRIO\Documents\Arduino\libraries\MFRC522\src\MFRC522Extended.cpp:824:44: error: ordered comparison of pointer with integer zero ('byte*' {aka 'unsigned char*'} and 'int')
  824 |         if (backData != nullptr && backLen > 0) {
      |                                    ~~~~~~~~^~~
  c:\Users\TEU USÁRIO\Documents\Arduino\libraries\MFRC522\src\MFRC522Extended.cpp:847:84: error: ordered comparison of pointer with integer zero ('byte*' {aka 'unsigned char*'} and 'int')
  847 |                 if (backData && backData != reinterpret_cast<byte*>(-1) && backLen > 0) {
      |  "
  O caminho do arquivo a ser corrigido é: "C:\Users\TEU USÁRIO\Documents\Arduino\libraries\MFRC522\src\MFRC522Extended.cpp"
  
  Basta Fazer como está aqui 
  "Correção da Linha 824: 
  if (backData != nullptr && *backLen > 0) {
		if (*backLen < in.inf.size)
			return STATUS_NO_ROOM;

		*backLen = in.inf.size;
		memcpy(backData, in.inf.data, in.inf.size);
	}"

  "Correção ad Linha 847 : 		
  if (backData && backData != reinterpret_cast<byte*>(-1) && backLen && *backLen > 0) {
			if ((*backLen + ackDataSize) > totalBackLen)
				return STATUS_NO_ROOM;

			memcpy(&(backData[*backLen]), ackData, ackDataSize);
			*backLen += ackDataSize;
		}"
  
  Este é um projeto da biblioteca MFRC522; com implementação do Visor de Cristal Liquído e acrescentado WebServer OTA, 
  para mais detalhes e outros exemplos, veja: https://github.com/miguelbalboa/rfid.

  Este projeto mostra um Sistema Completo de Controle de Acesso para Portas.
  Fluxo de Trabalho Simples (não limitado a):
                      //                  +------------+
  +-----------------------------------------LER AS TAGS-------------------------------------+
  |                        +-----------------------------------------+                      |
  |                        |                                         |                      |
  |                        |                                         |                      |
  |                   +----v-----+                             +-----v-----+                |
  |                   |TAG MESTRE|                             |OUTRAS TAGS|                |
  |                   +----+-----+                           +---------------+              |
  |                        |                                 |               |              |
  |                        |                                 |               |              |
  |                   +----v----+                    +-------v-------+ +-----v-----+        |
  |        +----------+LER  TAGS+------------+       |TAG CADASTRADAS| |TAG    DESC|        |
  |        |          +----+----+            |       +-------+-------+ +-----+-----+        |
  |        |               |                 |               |               |              |
  |  +-----v----+   +------v-------+   +-----v-----+   +-----v------+  +-----v-----+        |
  |  |TAG MESTRE|   |TAG CADASTRADA|   |TAG    DESC|  |ACESSO ACEITO|  |ACESSO NEGA|        |
  |  +----------+   +------+-------+   +-----+-----+  +------+------+  +-----+-----+        |
  |                        |                 |               |               |              |
  |      +----+      +-----v-----+       +---v---+           |               +-------------->
  +------>SAIR|      |DELETAR  DA|       |ADD  NA|           |                              |
  |      +----+      |  EEPROM   |       | EEPROM|           |                              |
  -------------------+-----------+-------+-------+           +------------------------------>
  Use um cartão mestre que funcionará como programador, então você poderá escolher quem terá acesso ou não.

  Interface de usuário fácil
  Apenas um tag RFID é necessário para adicionar ou remover tags. Você pode escolher usar LEDs para saída ou
  um módulo LCD serial para informar os usuários.
  Armazena informações na EEPROM
  As informações são armazenadas na memória EEPROM não volátil do Arduino para preservar os tags do usuário e
  o cartão mestre. Nenhuma informação é perdida se a energia for desligada. A EEPROM tem um ciclo de leitura
  ilimitado, mas um ciclo de gravação limitado de cerca de 100.000.
  Segurança
  Para simplificar, usaremos os IDs exclusivos dos tags. É simples, mas não é à prova de hackers.
  @license Liberado para o domínio público.
  Layout típico de pinos usados:
  -------------------------------------------------------------------------------------------------------------------
    MFRC522                 Arduino      Arduino   Arduino       Arduino       Arduino      ESP32
              Reader/PCD    Uno/101       Mega     Nano v3    Leonardo/Micro  Pro Micro  ESP32-WROOM
    Signal       Pino       GPIO          GPIO      GPIO          GPIO          GPIO        GPIO
  -------------------------------------------------------------------------------------------------------------------
  //RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST         2
  //SPI SS      SDA(SS)      10            53        D10        10               10          5
  //SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16          23
  //SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14          19
  //SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15          18
*/

#include "Bibliotecas.h"
#include "GPIOs.h"
#include "LoginsSenhas.h"

/*
  Em vez de um relé, você pode querer usar um servo. Os servos também podem bloquear e desbloquear fechaduras
  de portas.
  O relé será usado por padrão
*/

// #include <Servo.h>

/*
  Para visualizar o que está acontecendo no hardware, precisamos de alguns leds e para controlar a trava da porta,
  um relé e um botão de limpeza (ou algum outro hardware) Led de ânodo comum usado, digitalWriting HIGH desliga o
  led Lembre-se de que, se você estiver indo para usar led de cátodo comum ou apenas leds separados, simplesmente
  comente #define COMMON_ANODE.
*/

#define COMMON_ANODE

#ifdef COMMON_ANODE
#define LED_ON LOW
#define LED_OFF HIGH
#else
#define LED_ON HIGH
#define LED_OFF LOW
#endif

int estadoBotao = 0;  // Variável para ler o estado do botao

boolean match = false;        // Inicializar a correspondência do cartão como falso
boolean programMode = false;  // Inicializar modo de programação como falso
boolean replaceMaster = false;

uint8_t successRead;  // Variável inteira para manter se tivemos uma leitura bem-sucedida do leitor.

byte storedCard[4];  // Armazena um ID lido da EEPROM.
byte readCard[4];    // Armazena o ID escaneado lido do módulo RFID.
byte masterCard[4];  // Armazena o ID do cartão master lido da EEPROM.

// Cria uma instância da classe MFRC522.
constexpr uint8_t RST_PIN = 2;  // Configurável, veja o layout típico de pinos acima.
constexpr uint8_t SS_PIN = 5;   // Configurável, veja o layout típico de pinos acima.

MFRC522 mfrc522(SS_PIN, RST_PIN);

// --- Variáveis Globais ---
char st[20];

// Definir o número de colunas e linhas do display LCD
int ColunasLCD = 20;
int LinhasLCD = 4;

// Definir o endereço do display LCD, número de colunas e linhas
// Se você não souber o endereço do seu display, execute um sketch de scanner I2C.
LiquidCrystal_I2C lcd(0x27, ColunasLCD, LinhasLCD);

String messageStatic0 = "POR FAVOR";
String messageStatic1 = "APROXIME SEU CARTAO!";

/* Função para rolar o texto
    A função aceita os seguintes argumentos:
    row: Número da linha onde o texto será exibido
    message: Mensagem para rolar
    delayTime: Atraso entre cada deslocamento de caractere
    ColunasLCD: Número de colunas do seu LCD*/

void scrollText(int row, String message, int delayTime, int ColunasLCD) {
  for (int i = 0; i < ColunasLCD; i++) {
    message = " " + message;
  }
  message = message + " ";
  for (int pos = 0; pos < message.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + ColunasLCD));
    delay(delayTime);
  }
}

//Protótipos
void initOutputInput();
void initEEPROM();
void initSerialBegin();
void VerificarBotao();
void ProcessaLeituraCartao();
void lcdSetCursor();
void initWiFiConectado();
void initOTA();

void initOTA(){
  // Port defaults to 3232
  ArduinoOTA.setPort(8282); //Escolhe a porta que você preferir

  // Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setHostname("ESP32-RFID");

  // No authentication by default
  ArduinoOTA.setPassword("Coloca Aqui uma senha da tua escolha");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else {  // U_SPIFFS
        type = "filesystem";
      }

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      }
    });

  ArduinoOTA.begin();

  Serial.println("Conectado");
  Serial.print("Endereço de IP: ");
  Serial.println(WiFi.localIP());
}

void initWiFiConectado(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  unsigned long startAttemptTime = millis();
  unsigned long timeout = 3000; // Timeout inicial de 5 segundos

  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - startAttemptTime > timeout) {
      Serial.println("Tentando se conectar novamente ao WiFi...");
      WiFi.disconnect();
      WiFi.begin(ssid, password);
      startAttemptTime = millis(); // Reiniciar o timer
      timeout += 3000; // Aumentar o timeout a cada tentativa (opcional)
    }
    delay(200); // Pequeno atraso para evitar sobrecarga na CPU
  }

  Serial.println("Conectado ao WiFi!");
}

void initEEPROM() {
    /* Wipe Code - Se o botão (BotaoWipe) for pressionado durante a execução da configuração (ligada), a EEPROM
     será apagada.*/

  if (digitalRead(BotaoWipe) == LOW) {  // Quando o botão for pressionado, o pino deve ficar baixo, o botão está conectado ao GND
    digitalWrite(LedVermelho, LED_ON);  // O LED vermelho fica aceso para informar o usuário que vamos apagar
    Serial.println("Botao de formatacao apertado");
    Serial.println("Voce tem 10 segundos para cancelar");
    Serial.println("Isso vai apagar todos os seus registros e nao tem como desfazer");
    bool buttonState = monitorBotaoWipebutton(10000);            // Dê ao usuário tempo suficiente para cancelar a operação.
    if (buttonState == true && digitalRead(BotaoWipe) == LOW) {  // Se o botão ainda estiver pressionado, apaga a EEPROM.
      Serial.println("Inicio da formatacao da EEPROM");
      for (uint16_t x = 0; x < EEPROM.length(); x = x + 1) {  // Fim do loop do endereço da EEPROM
        if (EEPROM.read(x) == 0) {                            // Se endereço da EEPROM for 0
          // Não faz nada, pois já está limpo. Vá para o próximo endereço para economizar tempo e reduzir gravações na EEPROM
        } else {
          EEPROM.write(x, 0);  // Se não, escreva 0 para limpar, isso leva 3,3 milissegundos
        }
      }
      Serial.println("EEPROM formatada com sucesso");
      digitalWrite(LedVermelho, LED_OFF);  // Visualize uma formatação bem-sucedida
      delay(200);
      digitalWrite(LedVermelho, LED_ON);
      delay(200);
      digitalWrite(LedVermelho, LED_OFF);
      delay(200);
      digitalWrite(LedVermelho, LED_ON);
      delay(200);
      digitalWrite(LedVermelho, LED_OFF);
    } else {
      Serial.println("Formatacao cancelada");  // Mostra algum feedback de que o botão de formatação não foi pressionado por 15 segundos.
      digitalWrite(LedVermelho, LED_OFF);
    }
  }

    /* Verificar se o cartão mestre foi definido, caso contrário permitir que o usuário escolha um cartão mestre
     Isso também é útil para apenas redefinir o cartão mestre
     Você pode manter outros registros EEPROM, basta escrever outro número diferente de 143 no endereço EEPROM 1
     O endereço EEPROM 1 deve conter o número mágico '143'*/

  if (EEPROM.read(1) != 143) {
    Serial.println("Cartao Mestre nao definido");
    Serial.println("Leia um chip para definir cartao Mestre");
    do {
      successRead = getID();          // Define a variável successRead como 1 quando conseguimos fazer a leitura do leitor, caso contrário, 0.
      digitalWrite(LedAzul, LED_ON);  // Visualizar a necessidade de definir o Cartão Mestre.
      delay(200);
      digitalWrite(LedAzul, LED_OFF);
      delay(200);
    } while (!successRead);              // O programa não irá avançar até que você obtenha uma leitura bem sucedida.
    for (uint8_t j = 0; j < 4; j++) {    // Laço de repetição 4 vezes
      EEPROM.write(2 + j, readCard[j]);  // Escreva o UID do PICC escaneado na EEPROM, começando do endereço 3
    }
    EEPROM.write(1, 143);  // Escreva no EEPROM o cartão mestre que definimos.
    Serial.println("Cartao Mestre definido");
  }
  Serial.println("-------------------");
  Serial.println("UID do cartao Mestre");
  for (uint8_t i = 0; i < 4; i++) {      // Ler o UID do Cartão Mestre na EEPROM.
    masterCard[i] = EEPROM.read(2 + i);  // Escreva-o em masterCard.
    Serial.print(masterCard[i], HEX);
  }
  Serial.println("");
  Serial.println("-------------------");
  Serial.println("Tudo esta pronto");
  Serial.println("Aguardando pelos chips para serem lidos");
  cycleLeds();  // "Está tudo pronto, vamos dar ao usuário algum feedback por meio do ciclo de LEDs.

  EEPROM.commit();
}

void initSerialBegin(){
  // Protocolo de configuração
  Serial.begin(115200);  // Inicializar comunicações serial com o PC.
  while (!Serial);
}

void ProcessaLeituraCartao() {
  bool successRead = false;

  do {
    successRead = getID();  // Define a variável successRead como 1 quando conseguimos ler do leitor e como 0 caso contrário.

    if (digitalRead(BotaoWipe) == LOW) {  // Verifique se o botão está pressionado
      // Indicar que a operação normal é interrompida ao pressionar o botão de limpeza.
      digitalWrite(LedVermelho, LED_ON);  // Certifique-se de que o LED vermelho está aceso
      digitalWrite(LedVerde, LED_OFF);    // Certifique-se de que o LED verde está desligado
      digitalWrite(LedAzul, LED_OFF);     // Certifique-se de que o LED azul está desligado
      
      Serial.println("Botao de formatacao apertado");
      Serial.println("O cartao Mestre sera apagado em 10 segundos");

      bool buttonState = monitorBotaoWipebutton(10000);  // Dar ao usuário tempo suficiente para cancelar a operação

      if (buttonState == true && digitalRead(BotaoWipe) == LOW) {  // Se o botão ainda estiver pressionado, limpe a EEPROM.
        EEPROM.write(1, 0);  // Resetar o número mágico.
        EEPROM.commit();
        Serial.println("Cartao Mestre desvinculado do dispositivo");
        Serial.println("Aperte o reset da placa para reprogramar o cartao Mestre");
        while (1);  // Travar o código aqui até que o reset seja pressionado
      }

      Serial.println("Desvinculo do cartao Mestre cancelado");
    }

    if (programMode) {
      cycleLeds();  // Modo de Programação exibe uma sequência de cores
    } else {
      normalModeOn();  // Modo normal
    }

  } while (!successRead);  // O loop continua até obter uma leitura bem-sucedida.

  if (programMode) {
    if (isMaster(readCard)) {  // Verifique se o cartão mestre foi escaneado novamente para sair do modo de programa.
      Serial.println("Leitura do cartao Mestre");
      Serial.println("Saindo do modo de programacao");
      Serial.println("-----------------------------");
      programMode = false;
      return;
    } else {
      if (findID(readCard)) {  // Se o cartão escaneado for conhecido, exclua-o.
        Serial.println("Conheco este chip, removendo...");
        deleteID(readCard);
        Serial.println("-----------------------------");
        Serial.println("Leia um chip para adicionar ou remover da EEPROM");
      } else {  // Se o cartão escaneado não for conhecido, adicione-o.
        Serial.println("Nao conheco este chip, incluindo...");
        writeID(readCard);
        Serial.println("-----------------------------");
        Serial.println("Leia um chip para adicionar ou remover da EEPROM");
      }
    }
  } else {
    if (isMaster(readCard)) {  // Se o ID do cartão escaneado corresponder ao ID do Cartão Mestre - entrar no modo de programa.
      programMode = true;
      Serial.println("Ola Mestre - Modo de programacao iniciado");
      uint8_t count = EEPROM.read(0);  // Ler o primeiro byte da EEPROM
      Serial.print("Existem ");        // Exibir número de IDs na EEPROM
      Serial.print(count);
      Serial.print(" registro(s) na EEPROM");
      Serial.println("");
      Serial.println("Leia um chip para adicionar ou remover da EEPROM");
      Serial.println("Leia o cartao Mestre novamente para sair do modo de programacao");
      Serial.println("-----------------------------");
    } else {
      if (findID(readCard)) {  // Se não, verifique se o cartão está na EEPROM
        lcd.setCursor(2, 3);
        lcd.print("Voce pode passar");
        Serial.println("Bem-vindo, voce pode passar");
        granted(300);  // Abrir a fechadura da porta por 300 ms
      } else {  // Se não, mostrar que o ID não é válido
        Serial.println("Voce nao pode passar");
        lcd.setCursor(0, 3);
        lcd.print("Voce nao pode passar");
        delay(3000);
        lcd.clear();  // Limpa a tela
        denied();
      }
    }
  }  // Fim do else de if (isMaster(readCard))
}

void lcdSetCursor(){

  lcd.setCursor(5, 0);
  // Imprimir mensagem estática
  lcd.print(messageStatic0);
  // Defina o cursor na segunda coluna, primeira linha
  lcd.setCursor(0, 1);
  // Imprimir mensagem estática
  lcd.print(messageStatic1);
}

///////////////////////////////////////////////////// Setup ////////////////////////////////////////////////////////

void setup() {

  EEPROM.begin(1024);
  initOutputInput();
  initEEPROM();
  initSerialBegin();
  initWiFiConectado();
  initOTA();
 
  SPI.begin();  // O Módulo MFRC522 usa o protocolo SPI

  mfrc522.PCD_Init();  // Inicializa o Módulo MFRC522

  // Se você definir o Ganho da Antena como Max, ele aumentará a distância de leitura
  mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max); //<--Não consegui fazer funcionar na potência máxima

  Serial.println("Controle de Acesso V.1.0 - Realise Candidate");  // Para fins de depuração
  ShowReaderDetails();                               // Mostrar detalhes do leitor de cartão PCD - MFRC522.

  // Iniciar o LCD
  lcd.begin();
  // Ligar retroiluminação do LCD
  lcd.backlight();
}

////////////////////////////////////////////////////// Main Loop ////////////////////////////////////////////////////

void loop() {

  ProcessaLeituraCartao();
  lcdSetCursor();
  ArduinoOTA.handle();

}    // Fim do void loop

void initOutputInput() {
  // Arduino Pin Configuration
  pinMode(PortaAberta, INPUT_PULLUP);  //Sensor de fim de curso, o RFID só lerá outro cartão, quando a porta for fechada
  pinMode(LedVermelho, OUTPUT);
  pinMode(LedVerde, OUTPUT);
  pinMode(LedAzul, OUTPUT);
  pinMode(BotaoWipe, INPUT_PULLUP);  // Habilitar o resistor pull-up do pino.
  pinMode(Rele, OUTPUT);
  pinMode(BotaoAbrirPorta, INPUT_PULLUP);
  pinMode(Buzzer, OUTPUT);  //Definindo o pino buzzer como de saída.

  // Tenha cuidado com o comportamento do circuito do relé durante a reinicialização ou desligamento do seu Arduino.
  digitalWrite(Rele, HIGH);            // Certifique-se de que a porta esteja trancada
  digitalWrite(LedVermelho, LED_OFF);  // Certifique-se de que o LED esteja desligado
  digitalWrite(LedVerde, LED_OFF);     // Certifique-se de que o LED esteja desligado
  digitalWrite(LedAzul, LED_OFF);      // Certifique-se de que o LED esteja desligado
}

//////////////////////////////////////////////// Acesso Permitido //////////////////////////////////////////////////

void granted(uint16_t setDelay) {
  digitalWrite(LedAzul, LED_OFF);      // Desliga o LED azul
  digitalWrite(LedVermelho, LED_OFF);  // Desliga o LED vermelho
  digitalWrite(LedVerde, LED_ON);      // Liga o LED verde
  digitalWrite(Rele, LOW);             // Destrava a porta
  delay(setDelay);                     // Mantém a porta destravada pelo tempo especificado
  digitalWrite(Rele, HIGH);            // Trava a porta novamente

  // Mantém o acesso liberado até que o sensor da porta seja acionado
  while (digitalRead(PortaAberta)) {
    digitalWrite(Rele, LOW);
    digitalWrite(Rele, HIGH);
  }

  // Aguarda até que a porta seja fechada antes de continuar
  while (!digitalRead(PortaAberta)) {
    digitalWrite(LedVerde, LOW);
    digitalWrite(LedVerde, HIGH);
  }

  lcd.clear();  // Limpa a tela do LCD
}

///////////////////////////////////////////////// Accesso Negado ///////////////////////////////////////////////////

void denied() {
  digitalWrite(LedVerde, LED_OFF);    // Certifique-se de que o LED verde está desligado
  digitalWrite(LedAzul, LED_OFF);     // Certifique-se de que o LED azul está desligado
  digitalWrite(LedVermelho, LED_ON);  // Certifique-se de que o LED vermelho está ligado
  digitalWrite(Rele, HIGH);           // Tranca a porta novamente

  digitalWrite(Buzzer, HIGH);
  delay(250);
  digitalWrite(Buzzer, LOW);
  delay(250);
  digitalWrite(Buzzer, HIGH);
  delay(250);
  digitalWrite(Buzzer, LOW);
  delay(250);
  digitalWrite(Buzzer, HIGH);
  delay(250);
  digitalWrite(Buzzer, LOW);
  delay(250);
  digitalWrite(Buzzer, HIGH);
  delay(250);
  digitalWrite(Buzzer, LOW);
  delay(250);
  digitalWrite(Buzzer, HIGH);
  delay(250);
  digitalWrite(Buzzer, LOW);
  delay(250);
  digitalWrite(Buzzer, HIGH);
  delay(250);
  digitalWrite(Buzzer, LOW);
}

////////////////////////////// Obter UID do PICC (Proximity Integrated Circuit Card) ////////////////////////////////

uint8_t getID() {
  // Getting ready for Reading PICCs
  if (!mfrc522.PICC_IsNewCardPresent()) {  // Se um novo PICC for colocado no leitor RFID, continue.
    return 0;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {  // Uma vez que um PICC é colocado, obtenha o número de série e continue.
    return 0;
  }

  /* Existem PICCs Mifare que têm UID de 4 bytes ou 7 bytes. Cuidado ao usar PICCs de 7 bytes.
     Acredito que devemos assumir que todo PICC tem UID de 4 bytes
     Até que suportemos PICCs de 7 bytes.*/

  Serial.println("UID do chip lido:");
  for (uint8_t i = 0; i < 4; i++) {  //
    readCard[i] = mfrc522.uid.uidByte[i];
    Serial.print(readCard[i], HEX);
    lcd.setCursor(0, 2);
    lcd.print("USUARIO  AUTORIZADO?");
  }

  Serial.println("");
  mfrc522.PICC_HaltA();  // Parar a leitura
  return 1;
}

void ShowReaderDetails() {
  // Obter a versão do software MFRC522
  byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  Serial.print("Versao do software MFRC522: 0x");
  Serial.print(v, HEX);
  if (v == 0x91)
    Serial.print(" = v1.0");
  else if (v == 0x92)
    Serial.print(" = v2.0");
  else
    Serial.print(" (desconhecido),provavelmente um clone chines?");
  Serial.println("");
  // Se for retornado 0x00 ou 0xFF, a comunicação provavelmente falhou.
  if ((v == 0x00) || (v == 0xFF)) {
    Serial.println("ALERTA: Falha na comunicacao, o modulo MFRC522 esta conectado corretamente?");
    Serial.println("SISTEMA ABORTADO: Verifique as conexoes.");
    // Visualize que o sistema está interrompido
    digitalWrite(LedVerde, LED_OFF);    // Certifique-se de que o LED verde está desligado.
    digitalWrite(LedAzul, LED_OFF);     // Certifique-se de que o LED azul está desligado.
    digitalWrite(LedVermelho, LED_ON);  // Ligar o LED vermelho
    while (true);  // Não prossiga.
  }
}

////////////////////////////////////////////// LEDs (Modo de Programa) //////////////////////////////////////////////

void cycleLeds() {
  digitalWrite(LedVermelho, LED_OFF);  // Certifique-se de que o LED vermelho está desligado.
  digitalWrite(LedVerde, LED_ON);      // Certifique-se de que o LED verde está ligado.
  digitalWrite(LedAzul, LED_OFF);      // Certifique-se de que o LED azul está desligado.
  delay(200);
  digitalWrite(LedVermelho, LED_OFF);  // Certifique-se de que o LED vermelho está desligado.
  digitalWrite(LedVerde, LED_OFF);     // Certifique-se de que o LED verde está desligado.
  digitalWrite(LedAzul, LED_ON);       // Certifique-se de que o LED azul está ligado.
  delay(200);
  digitalWrite(LedVermelho, LED_ON);  // Certifique-se de que o LED vermelho está ligado.
  digitalWrite(LedVerde, LED_OFF);    // Certifique-se de que o LED verde está desligado.
  digitalWrite(LedAzul, LED_OFF);     // Certifique-se de que o LED azul está desligado.
  delay(200);
}

////////////////////////////////////////////// LEDs (Modo Normal) ///////////////////////////////////////////////////

// Variáveis globais para debounce e controle do tempo do relé
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;     // Tempo de debounce (50 ms)
bool lastButtonState = HIGH;          // Estado anterior do botão
bool buttonPressed = false;           // Indica se o botão foi pressionado
unsigned long releDestravarTime = 0;  // Tempo para destravar o relé
unsigned long releDelay = 1500;       // Tempo que o relé ficará destravado (1.5s)
bool releAtivo = false;               // Estado do relé (porta destravada)

void normalModeOn() {
  digitalWrite(LedAzul, LED_ON);       // LED Azul LIGADO e pronto para ler o cartão
  digitalWrite(LedVermelho, LED_OFF);  // Certifique-se de que o LED vermelho está desligado.
  digitalWrite(LedVerde, LED_OFF);     // Certifique-se de que o LED verde está desligado.

  // Leitura do estado atual do botão
  bool reading = digitalRead(BotaoAbrirPorta);

  // Implementação do debounce para evitar leituras erradas de múltiplos pressionamentos
  if (reading != lastButtonState) {
    lastDebounceTime = millis();  // Atualiza o tempo de debounce quando o estado muda
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Verifica se o botão foi pressionado (estado LOW após debounce)
    if (reading == LOW && !buttonPressed && !releAtivo) {
      buttonPressed = true;           // Indica que o botão foi pressionado
      releAtivo = true;               // Ativa o relé (destrava a porta)
      digitalWrite(Rele, LOW);        // Destrava a porta
      releDestravarTime = millis();   // Armazena o tempo em que o relé foi destravado
    }
  }

  // Se o relé está ativo e o tempo de destravamento passou, trava a porta novamente
  if (releAtivo && (millis() - releDestravarTime >= releDelay)) {
    digitalWrite(Rele, HIGH);  // Trava a porta
    releAtivo = false;         // Marca que o relé não está mais ativo
    buttonPressed = false;     // Reseta o estado do botão para permitir nova ativação
  }

  // Atualiza o último estado do botão
  lastButtonState = reading;
}

/////////////////////////////////////////////// Ler um ID da EEPROM /////////////////////////////////////////////////

void readID(uint8_t number) {
  uint8_t start = (number * 4) + 2;          // Descobrir a posição inicial
  for (uint8_t i = 0; i < 4; i++) {          // Repetir 4 vezes para obter os 4 Bytes
    storedCard[i] = EEPROM.read(start + i);  // Atribuir os valores lidos da EEPROM a uma matriz
  }
}

/////////////////////////////////////////////// Adicionar ID à EEPROM /////////////////////////////////////////////

void writeID(byte a[]) {
  if (!findID(a)) {                    // Antes de escrever na EEPROM, verifique se já vimos este cartão antes!
    uint8_t num = EEPROM.read(0);      // Obter o número de espaços utilizados, posição 0 armazena o número de cartões de ID.
    uint8_t start = (num * 4) + 6;     // Descobrir onde começa o próximo slot
    num++;                             // Incrementar o contador em um
    EEPROM.write(0, num);              // Escrever o novo contador no registrador
    for (uint8_t j = 0; j < 4; j++) {  // Loop 4 vezes
      EEPROM.write(start + j, a[j]);   // Escrever os valores da matriz na EEPROM na posição correta
    }
    EEPROM.commit();
    successWrite();
    Serial.println("ID adicionado na EEPROM com sucesso");
  } else {
    failedWrite();
    Serial.println("Erro! Tem alguma coisa errada com o ID do chip ou problema na EEPROM");
  }
}

///////////////////////////////////////////// Remover ID da EEPROM ////////////////////////////////////////////////

void deleteID(byte a[]) {
  if (!findID(a)) {  // Antes de excluir da EEPROM, verifique se temos este cartão!
    failedWrite();   // Se não
    Serial.println("Erro! Tem alguma coisa errada com o ID do chip ou problema na EEPROM");
  } else {
    uint8_t num = EEPROM.read(0);  // Obter o número de espaços utilizados, a posição 0 armazena o número de cartões de ID.
    uint8_t slot;                  // Descobrir o número do slot do cartão
    uint8_t start;                 // = (num * 4) + 6; // Descobrir onde começa o próximo slot
    uint8_t looping;               // O número de vezes que o loop se repete
    uint8_t j;
    slot = findIDSLOT(a);  // Descobrir o número do slot do cartão a ser excluído
    start = (slot * 4) + 2;
    looping = ((num - slot) * 4);
    num--;                                                  // Decrementar o contador em um
    EEPROM.write(0, num);                                   // Escrever o novo contador no registrador
    for (j = 0; j < looping; j++) {                         // Loop o número de deslocamentos do cartão
      EEPROM.write(start + j, EEPROM.read(start + 4 + j));  // Deslocar os valores da matriz para 4 posições anteriores na EEPROM
      for (uint8_t k = 0; k < 4; k++) {                     // Loop de deslocamento
        EEPROM.write(start + j + k, 0);
      }
      EEPROM.commit();
      successDelete();
      Serial.println("ID removido da EEPROM com sucesso");
    }
  }
}

//////////////////////////////////////////////// Verificar Bytes ////////////////////////////////////////////////

boolean checkTwo(byte a[], byte b[]) {
  if (a[0] != 0)                     // Certifique-se de que há algo na matriz primeiro
    match = true;                    // Assuma que eles correspondem inicialmente
  for (uint8_t k = 0; k < 4; k++) {  // Loop 4 vezes
    if (a[k] != b[k])                // SE a != b, então defina correspondência = falso, se um falhar, todos falham
      match = false;
  }
  if (match) {    // Verifique se a correspondência ainda é verdadeira
    return true;  // Retornar verdadeiro
  } else {
    return false;  // Retornar falso
  }
}

///////////////////////////////////////////////// Encontrar Slot /////////////////////////////////////////////////

uint8_t findIDSLOT(byte find[]) {
  uint8_t count = EEPROM.read(0);         // Ler o primeiro byte da EEPROM
  for (uint8_t i = 1; i <= count; i++) {  // Loop uma vez para cada entrada na EEPROM
    readID(i);                            // Ler um ID da EEPROM, ele está armazenado em storedCard[4]
    if (checkTwo(find, storedCard)) {     // Verifique se o storedCard lido da EEPROM
      // É o mesmo que o cartão de ID find[] passado
      return i;  // O número do slot do cartão
      break;     // Pare de procurar, encontramos
    }
  }
  return 0;  // Valor padrão indicando erro
}

///////////////////////////////////////////// Encontrar ID na EEPROM /////////////////////////////////////////////

boolean findID(byte find[]) {
  uint8_t count = EEPROM.read(0);         // Ler o primeiro byte da EEPROM
  for (uint8_t i = 1; i <= count; i++) {  // Loop uma vez para cada entrada na EEPROM
    readID(i);                            // Ler um ID da EEPROM, ele está armazenado em storedCard[4]
    if (checkTwo(find, storedCard)) {     // Verifique se o storedCard lido da EEPROM
      return true;
      break;  // Pare de procurar, encontramos
    } else {  // Se não, retorne falso
    }
  }
  return false;
}

///////////////////////////////////////// Escrito com "Sucesso" na EEPROM   //////////////////////////////////////

// Pisca o LED verde 3 vezes para indicar uma gravação bem-sucedida na EEPROM
void successWrite() {
  digitalWrite(LedAzul, LED_OFF);      // Certifique-se de que o LED azul está desligado.
  digitalWrite(LedVermelho, LED_OFF);  // Certifique-se de que o LED vermelho está desligado.
  digitalWrite(LedVerde, LED_OFF);     // Certifique-se de que o LED verde está desligado.
  delay(200);
  digitalWrite(LedVerde, LED_ON);  // Certifique-se de que o LED verde está ligado.
  delay(200);
  digitalWrite(LedVerde, LED_OFF);  // Certifique-se de que o LED verde está desligado.
  delay(200);
  digitalWrite(LedVerde, LED_ON);  // Certifique-se de que o LED verde está ligado.
  delay(200);
  digitalWrite(LedVerde, LED_OFF);  // Certifique-se de que o LED verde está desligado.
  delay(200);
  digitalWrite(LedVerde, LED_ON);  // Certifique-se de que o LED verde está ligado.
  delay(200);
}

/////////////////////////////////////////// Escrita na EEPROM Falhou //////////////////////////////////////////////

// Pisca o LED vermelho 3 vezes para indicar uma gravação malsucedida na EEPROM.
void failedWrite() {
  digitalWrite(LedAzul, LED_OFF);      // Certifique-se de que o LED azul está desligado.
  digitalWrite(LedVermelho, LED_OFF);  // Certifique-se de que o LED vermelho está desligado.
  digitalWrite(LedVerde, LED_OFF);     // Certifique-se de que o LED verde está desligado.
  delay(200);
  digitalWrite(LedVermelho, LED_ON);  // Certifique-se de que o LED vermelho está ligado.
  delay(200);
  digitalWrite(LedVermelho, LED_OFF);  // Certifique-se de que o LED vermelho está desligado.
  delay(200);
  digitalWrite(LedVermelho, LED_ON);  // Certifique-se de que o LED vermelho está ligado.
  delay(200);
  digitalWrite(LedVermelho, LED_OFF);  // Certifique-se de que o LED vermelho está desligado.
  delay(200);
  digitalWrite(LedVermelho, LED_ON);  // Certifique-se de que o LED vermelho está ligado.
  delay(200);
}

///////////////////////////////////////// Sucesso ao Remover UID da EEPROM ///////////////////////////////////////

// Pisca o LED azul 3 vezes para indicar uma exclusão bem-sucedida na EEPROM.
void successDelete() {
  digitalWrite(LedAzul, LED_OFF);      // Certifique-se de que o LED azul está desligado.
  digitalWrite(LedVermelho, LED_OFF);  // Certifique-se de que o LED vermelho está desligado.
  digitalWrite(LedVerde, LED_OFF);     // Certifique-se de que o LED verde está desligado.
  delay(200);
  digitalWrite(LedAzul, LED_ON);  // Certifique-se de que o LED azul está ligado.
  delay(200);
  digitalWrite(LedAzul, LED_OFF);  // Certifique-se de que o LED azul está desligado.
  delay(200);
  digitalWrite(LedAzul, LED_ON);  // Certifique-se de que o LED azul está ligado.
  delay(200);
  digitalWrite(LedAzul, LED_OFF);  // Certifique-se de que o LED azul está desligado.
  delay(200);
  digitalWrite(LedAzul, LED_ON);  // Certifique-se de que o LED azul está dligado.
  delay(200);
}

/////////////////////////////// Verifica se o cartão lido é um CARTÃO MESTRE   ///////////////////////////////////

// Verifique se o ID passado é o cartão mestre de programação.
boolean isMaster(byte test[]) {
  if (checkTwo(test, masterCard))
    return true;
  else
    return false;
}

bool monitorBotaoWipebutton(uint16_t timeout) {
  uint32_t startTime = millis();
  while ((millis() - startTime) < timeout) {
    if (digitalRead(BotaoWipe) == HIGH) {
      return false;  // Botão foi liberado
    }
  }
  return true;  // Botão ainda está pressionado após o timeout
}
