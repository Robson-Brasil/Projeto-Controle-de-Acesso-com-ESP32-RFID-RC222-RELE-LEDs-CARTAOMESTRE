const char* host = "esp32-rfid";

WebServer server(3232);

DNSServer dns;

// IP Estático
IPAddress local_IP(192, 168, 15, 60);
IPAddress gateway(192, 168, 15, 1);
IPAddress subnet(255, 255, 255, 0);

// DNS Estático
IPAddress primaryDNS(1, 1, 1, 1);
IPAddress secondaryDNS(8, 8, 8, 8);
