# Test Procedure

1. Hardware Setup
   - HC-SR04 to A2 (ECHO) / A3 (TRIG); IR sensors A0/A1.
   - L298N: enA=10, in1=9, in2=8, in3=7, in4=6, enB=5.
   - MQ-135 analog=A4, digital=D4; MLX90614 via I2C; servo on A5.
   - ESP8266 via SoftwareSerial: UNO D2 (RX), D3 (TX).
2. Firmware
   - Flash Code/Arduino_Main.ino to UNO.
   - Flash Code/ESP8266_Client.ino to ESP8266 (edit Wi-Fi/API first).
3. Validation
   - Confirm obstacle avoidance + line following.
   - Check gas/temperature serial logs.
   - Verify ThingSpeak updates.
4. Safety
   - Separate motor supply, common GND.
   - Test with wheels lifted initially.
