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

int estadoBotao = 0;       // Variável para ler o estado do botao

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
