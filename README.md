# Esquemático para ligaçao dos componentes
![Controle-de-Acesso-com-ESP32-RFID-RC222-RELE-LEDs-CARTAOMESTRE_bb](https://user-images.githubusercontent.com/66142021/235301753-bf644ea6-087a-4726-8a6c-6c3e3e5c0418.png)
# PinOut do ESP32
![ESP32-38 PIN-DEVBOARD](https://user-images.githubusercontent.com/66142021/235302011-d1a5d855-c954-4d34-af21-e0c48a9aa48f.png)

# Acesso Total: Controle de Acesso com ESP32 e RFID RC522

Introdução
Funcionamento do Controle de Acesso
Vantagens do Controle de Acesso com ESP32 e RFID RC522
Aplicações do Controle de Acesso com ESP32 e RFID RC522
Considerações Finais
Referências

Introdução
O controle de acesso é uma necessidade vital em muitas instalações, como empresas, escolas e hospitais. O uso de tecnologias como o ESP32 e o RFID RC522 pode tornar essa tarefa mais fácil e segura.
O ESP32 é um microcontrolador de baixo custo com conectividade Wi-Fi e Bluetooth integrada, enquanto o RFID RC522 é um leitor de cartões RFID que permite a identificação rápida e precisa de usuários autorizados.

Funcionamento do Controle de Acesso
O controle de acesso com ESP32 e RFID RC522 funciona da seguinte maneira: primeiro, o usuário aproxima seu cartão RFID do leitor RC522. Em seguida, o leitor envia os dados do cartão para o microcontrolador ESP32, que verifica se o cartão é válido e se o usuário tem permissão para acessar o local.
Se o cartão for válido e o usuário tiver permissão, o ESP32 aciona um relé que libera a trava da porta ou cancela o alarme de segurança. Caso contrário, o acesso é negado e um alerta é emitido.

Vantagens do Controle de Acesso com ESP32 e RFID RC522
Uma das principais vantagens do controle de acesso com ESP32 e RFID RC522 é a segurança que proporciona. Com a identificação por cartão RFID, é possível garantir que apenas pessoas autorizadas tenham acesso ao local.
Além disso, o uso do ESP32 permite que o sistema seja facilmente integrado a outros dispositivos inteligentes, como câmeras de segurança e sistemas de automação predial, tornando a gestão do acesso ainda mais eficiente.

Aplicações do Controle de Acesso com ESP32 e RFID RC522
O controle de acesso com ESP32 e RFID RC522 pode ser aplicado em uma variedade de ambientes, desde escritórios e fábricas até escolas e hospitais.
Também pode ser utilizado em residências, permitindo que os moradores controlem o acesso à sua casa de forma segura e conveniente, sem precisar carregar chaves ou lembrar senhas.

Considerações Finais
O controle de acesso com ESP32 e RFID RC522 é uma solução eficiente e segura para gerenciar o acesso a locais restritos. Com a crescente demanda por segurança em ambientes corporativos e residenciais, essa tecnologia tem um grande potencial de mercado.
No entanto, é importante ressaltar que a implementação desse sistema deve ser feita por profissionais capacitados e seguindo as normas de segurança adequadas, a fim de garantir a integridade do sistema e a privacidade dos usuários.

Referências
ESP32 - https://www.espressif.com/en/products/socs/esp32
RFID RC522 - https://www.nxp.com/products/rfid-nfc/reader-ics/highly-integrated-14443-a-ic-card-reader-with-uart-spi-interface:MFRC52202HN1

# Projeto: Controle de Acesso com ESP32 RFID-RC222

O projeto que será descrito aqui consiste em utilizar o microcontrolador ESP32 e o leitor de cartões identificação por radiofrequência modelo RC522 para criar um sistema de controle de acesso para uma porta ou ambiente. O objetivo é que o sistema permita a entrada somente de pessoas autorizadas e facilite o cadastro e exclusão dessas pessoas sem a necessidade de alterar o código manualmente.

Para implementar esse projeto, é necessário ter alguns componentes como o próprio ESP32, o leitor de cartões identificação por radiofrequência RC522, um módulo relé, um visor de cristal líquido (LCD) e algumas tags identificação por radiofrequência. É importante escolher uma tag única e especifica para ser utilizado como cartão mestre, que será responsável por cadastrar novas tags ou excluir aquelas que não são mais autorizadas.

O primeiro passo é realizar a conexão dos componentes com o ESP32. O leitor de cartões identificação por radiofrequência RC522 pode ser conectado através dos pinos SDA, SCK, MOSI, MISO e IRQ do ESP32. O visor de cristal líquido pode ser conectado através dos pinos SDA e SCL. O módulo relé pode ser conectado através de um pino digital do ESP32.

Após a conexão dos componentes, é necessário programar o microcontrolador para que ele seja capaz de ler as tags identificação por radiofrequência e identificar se elas são autorizadas ou não. Para isso, pode ser utilizada a biblioteca MFRC522.h para controlar o leitor de cartões identificação por radiofrequência e a biblioteca LiquidCrystal_I2C.h para controlar o visor de cristal líquido.

Para permitir o cadastro automático de novas tags, é possível utilizar o cartão mestre como uma forma de autorização. Quando o cartão mestre é apresentado ao leitor de cartões identificação por radiofrequência, o sistema entrará em modo de cadastro, permitindo que novas tags sejam cadastradas. Para excluir uma tag, basta apresentar o cartão mestre e selecionar a opção de exclusão.

Quando uma tag é apresentada ao leitor de cartões identificação por radiofrequência, o microcontrolador irá verificar se ela é autorizada ou não. Caso a tag seja autorizada, uma mensagem será exibida no visor de cristal líquido indicando que a porta foi destravada. Além disso, o módulo relé será acionado, indicando que a porta foi aberta. Caso a tag não seja autorizada, o sistema não permitirá a entrada e exibirá uma mensagem indicando que o acesso foi negado.

Para tornar o sistema mais seguro, é possível adicionar outras funcionalidades, como a utilização de senhas ou biometria. Dessa forma, seria necessário apresentar tanto a tag identificação por radiofrequência quanto a senha ou realizar a identificação biométrica para permitir o acesso.

Em resumo, o projeto consiste em utilizar o ESP32 e o leitor de cartões identificação por radiofrequência modelo RC522 para criar um sistema de controle de acesso para uma porta ou ambiente. O sistema permite o cadastro automático de novas tags utilizando um cartão mestre e a exclusão de tags não autorizadas. Quando uma tag autorizada é apresentada ao leitor de cartões identificação por radiofrequência, a porta é destravada e uma mensagem é exibida no visor de cristal líquido. O sistema é facilmente escalável, podendo ser adicionadas novas funcionalidades para aumentar a segurança do sistema.
