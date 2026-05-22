# Graph Report - .  (2026-04-23)

## Corpus Check
- Corpus is ~4,253 words - fits in a single context window. You may not need a graph.

## Summary
- 25 nodes · 24 edges · 4 communities detected
- Extraction: 67% EXTRACTED · 33% INFERRED · 0% AMBIGUOUS · INFERRED: 8 edges (avg confidence: 0.84)
- Token cost: 0 input · 0 output

## Community Hubs (Navigation)
- [[_COMMUNITY_OTA Web Interface|OTA Web Interface]]
- [[_COMMUNITY_RFID Card System|RFID Card System]]
- [[_COMMUNITY_Hardware Peripherals|Hardware Peripherals]]
- [[_COMMUNITY_System Core|System Core]]

## God Nodes (most connected - your core abstractions)
1. `Access Control System` - 8 edges
2. `ESP32 Microcontroller` - 6 edges
3. `RFID RC522 Reader` - 4 edges
4. `OTA Firmware Update` - 4 edges
5. `LCD Display (Liquid Crystal)` - 3 edges
6. `Main OTA Page` - 3 edges
7. `Relay Module` - 2 edges
8. `Master Card` - 2 edges
9. `RFID Tags` - 2 edges
10. `MFRC522.h Library` - 2 edges

## Surprising Connections (you probably didn't know these)
- `OneCircuit Logo` --brands--> `Access Control System`  [INFERRED]
  avatar.png → README.md
- `OneCircuit Logo` --brands--> `OTA Firmware Update`  [INFERRED]
  avatar.png → main.html
- `OTA Firmware Update` --uses--> `SPIFFS Filesystem`  [INFERRED]
  main.html → access.html
- `Access Control System` --includes--> `OTA Firmware Update`  [INFERRED]
  README.md → main.html
- `Graphify Tool` --documents--> `Access Control System`  [EXTRACTED]
  AGENTS.md → README.md

## Hyperedges (group relationships)
- **Access Control Hardware Stack** — esp32_microcontroller, rfid_rc522_reader, relay_module, lcd_display [EXTRACTED 1.00]
- **OTA Web Interface Pages** — main_page, upload_page, access_page [EXTRACTED 1.00]
- **Control Libraries** — mfrc522_library, liquidcrystal_i2c_library [INFERRED 0.85]

## Communities

### Community 0 - "OTA Web Interface"
Cohesion: 0.4
Nodes (5): SPIFFS Update Page, Main OTA Page, SPIFFS Filesystem, Temperature Sensor, Upload Firmware Page

### Community 1 - "RFID Card System"
Cohesion: 0.5
Nodes (4): Master Card, MFRC522.h Library, RFID RC522 Reader, RFID Tags

### Community 2 - "Hardware Peripherals"
Cohesion: 0.67
Nodes (4): ESP32 Microcontroller, LCD Display (Liquid Crystal), LiquidCrystal_I2C.h Library, Relay Module

### Community 3 - "System Core"
Cohesion: 0.67
Nodes (4): Access Control System, Graphify Tool, OneCircuit Logo, OTA Firmware Update

## Knowledge Gaps
- **2 isolated node(s):** `Temperature Sensor`, `Graphify Tool`
  These have ≤1 connection - possible missing edges or undocumented components.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `Access Control System` connect `System Core` to `RFID Card System`, `Hardware Peripherals`?**
  _High betweenness centrality (0.286) - this node is a cross-community bridge._
- **Why does `OTA Firmware Update` connect `System Core` to `OTA Web Interface`?**
  _High betweenness centrality (0.203) - this node is a cross-community bridge._
- **Why does `Upload Firmware Page` connect `OTA Web Interface` to `System Core`?**
  _High betweenness centrality (0.087) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `Access Control System` (e.g. with `OTA Firmware Update` and `OneCircuit Logo`) actually correct?**
  _`Access Control System` has 2 INFERRED edges - model-reasoned connections that need verification._
- **Are the 2 inferred relationships involving `ESP32 Microcontroller` (e.g. with `MFRC522.h Library` and `LiquidCrystal_I2C.h Library`) actually correct?**
  _`ESP32 Microcontroller` has 2 INFERRED edges - model-reasoned connections that need verification._
- **Are the 3 inferred relationships involving `OTA Firmware Update` (e.g. with `Access Control System` and `SPIFFS Filesystem`) actually correct?**
  _`OTA Firmware Update` has 3 INFERRED edges - model-reasoned connections that need verification._
- **What connects `Temperature Sensor`, `Graphify Tool` to the rest of the system?**
  _2 weakly-connected nodes found - possible documentation gaps or missing edges._