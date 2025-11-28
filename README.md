Autonomous Hazard Inspection Robot

Arduino-based autonomous robot for hazardous inspection — ultrasonic sensing, line following, gas & IR temperature monitoring, and telemetry via ESP8266 (ThingSpeak).
Features

    Obstacle detection & avoidance (HC-SR04 + SG90 sweep)
    Line following (dual IR sensors)
    Gas detection (MQ-135)
    Contactless temperature (MLX90614)
    Wi-Fi telemetry (ESP8266 -> ThingSpeak)

Structure

Autonomous-Hazard-Inspection-Robot/
├── Code/
│   ├── Arduino_Main.ino
│   └── ESP8266_Client.ino
├── Docs/
│   ├── Project_Report.pdf
│   └── Test_Procedure.md
├── Hardware/
│   ├── Circuit_Schematic.png
│   └── System_Architecture.png
└── .gitignore

Quick Start

    Wire hardware as per Docs/Test_Procedure.md.
    In ESP8266_Client.ino, set your Wi-Fi + ThingSpeak API key.
    Flash UNO + ESP8266, open Serial Monitor at 9600 baud.

    This repo is a cleaned educational version; credentials removed.
