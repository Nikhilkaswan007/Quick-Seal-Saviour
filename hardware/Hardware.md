# Hardware Setup Guide - Quick Seal Saviour

## 📋 Table of Contents
- [Component List](#component-list)
- [Wiring Diagram](#wiring-diagram)
- [Pin Configuration](#pin-configuration)
- [Hardware Assembly](#hardware-assembly)
- [Power Requirements](#power-requirements)
- [Testing & Calibration](#testing--calibration)
- [Troubleshooting](#troubleshooting)
- [Safety Guidelines](#safety-guidelines)

## 🛠️ Component List

### Main Components

| Component | Model/Specification | Quantity | Purpose | Estimated Cost |
|-----------|-------------------|----------|---------|---------------|
| **Microcontroller** | ESP32 DevKit V1 | 1 | Main processing unit | $10-15 |
| **Heart Rate Sensor** | MAX30105 Breakout | 1 | Vital signs monitoring | $15-20 |
| **OLED Display** | SSD1306 128x64 I2C | 1 | Real-time data display | $5-8 |
| **Moisture Sensors** | Analog Soil Moisture | 2 | Blood detection | $3-5 each |
| **Piezoelectric Sensors** | Piezo Disc 27mm | 3+ | Impact detection | $2-3 each |
| **Blood Pressure Cuff** | Standard BP Cuff | 1 | Automated sealing | $15-25 |
| **DC Motor** | 12V Gear Motor | 1 | Cuff inflation control | $10-15 |
| **Solenoid Valve** | 12V Pneumatic Valve | 1 | Pressure regulation | $8-12 |
| **Air Pump** | Mini Diaphragm Pump | 1 | Cuff inflation | $20-30 |
| **Buzzer** | Active Buzzer 5V | 1 | Audio alerts | $2-3 |
| **LEDs** | Red/Green 5mm LEDs | 4 | Visual indicators | $1-2 |
| **Push Buttons** | Tactile Push Button | 6 | User interface | $1-2 |
| **Resistors** | 10kΩ Pull-up | 10 | Button debouncing | $1-2 |
| **Jumper Wires** | Male-Female/Male-Male | 1 Set | Connections | $5-10 |
| **Breadboard/PCB** | Full-size Breadboard | 1 | Circuit assembly | $5-10 |
| **Power Supply** | 12V 2A Adapter | 1 | System power | $10-15 |

### Additional Components

| Component | Specification | Purpose |
|-----------|--------------|---------|
| **Voltage Regulator** | LM2596 Buck Converter | Step down 12V to 5V/3.3V |
| **Relay Module** | 2-Channel 5V Relay | Motor/valve control |
| **Wire Management** | Fabric with embedded wires | Impact area detection |
| **Enclosure** | Waterproof Project Box | Protection housing |
| **Pressure Sensor** | BMP180/BMP280 | Cuff pressure monitoring |

## 🔌 Pin Configuration

### ESP32 Pin Assignment

```cpp
// Sensor Pins
#define moisturePin1        35    // Analog input for moisture sensor 1
#define moisturePin2        34    // Analog input for moisture sensor 2
#define WIRE_PIN_1          12    // Digital input for wire 1 monitoring
#define WIRE_PIN_2          14    // Digital input for wire 2 monitoring  
#define WIRE_PIN_3          27    // Digital input for wire 3 monitoring

// Control Pins
#define START_BUTTON_PIN    32    // Start system button
#define STOP_BUTTON_PIN     33    // Emergency stop button
#define buzz                13    // Buzzer output

// User Interface Buttons
#define butt1               15    // BPM monitoring toggle
#define butt2               16    // Wire monitoring toggle
#define butt3               17    // Temperature monitoring toggle
#define butt4               18    // Moisture monitoring toggle

// Output Control Pins
#define MOTOR_CONTROL_PIN   25    // DC motor control (via relay)
#define VALVE_CONTROL_PIN   26    // Solenoid valve control (via relay)
#define LED_RED_PIN         2     // Red status LED
#define LED_GREEN_PIN       4     // Green status LED

// I2C Pins (Default for ESP32)
#define SDA_PIN             21    // I2C Data line
#define SCL_PIN             22    // I2C Clock line
```

## 📐 Wiring Diagram

### Main System Connections

```
ESP32 DevKit V1

│  3V3  ┌───
│  GND  │                         
│   21  ├─ SDA ─────────────────  
│   22  ├─ SCL ─────────────────
│   35  ├─ Moisture Sensor 1  
│   34  ├─ Moisture Sensor 2   
│   32  ├─ Start Button         
│   33  ├─ Stop Button         
│   15  ├─ Button 1 (BPM)      
│   16  ├─ Button 2 (Wire)     
│   17  ├─ Button 3 (Temp)     
│   18  ├─ Button 4 (Moisture) 
│   25  ├─ Motor Relay         
│   26  ├─ Valve Relay        
│   13  ├─ Buzzer              
│   12  ├─ Wire Monitor 1     
│   14  ├─ Wire Monitor 2      
│   27  ├─ Wire Monitor 3   
│    2  ├─ Red LED             
│    4  ├─ Green LED           
└────┬──┴ 
     │                         
     └─ 5V Power Input           
                                 ││ │   │
        MAX30105 Sensor          ││ │   │
        ┌──────────────────┐     ││ │   │
        │  VCC ────────────┼──3V3┘│ │   │
        │  GND ────────────┼──GND─┘ │   │
        │  SDA ────────────┼────────┘   │
        │  SCL ────────────┼────────────┘
        └──────────────────┘

        SSD1306 OLED Display
        ┌──────────────────┐
        │  VCC ────────────┼──3V3
        │  GND ────────────┼──GND  
        │  SDA ────────────┼──Pin 21
        │  SCL ────────────┼──Pin 22
        └──────────────────┘
```

### Power Distribution

```
12V Power Supply
       │
       ├─── LM2596 Buck Converter ───── 5V Rail
       │                               │
       │                               ├─── ESP32 (5V Input)
       │                               ├─── Relay Module
       │                               └─── Buzzer
       │
       ├─── Motor (12V Direct)
       └─── Solenoid Valve (12V Direct)
```

## 🔧 Hardware Assembly

### Step 1: Prepare the Main Board

1. **Mount ESP32** on breadboard or custom PCB
2. **Install voltage regulator** for power distribution
3. **Connect power rails** (3.3V, 5V, GND)
4. **Add decoupling capacitors** (100nF near each IC)

### Step 2: Sensor Installation

#### MAX30105 Heart Rate Sensor
```
MAX30105 → ESP32
VCC     → 3.3V
GND     → GND
SDA     → Pin 21
SCL     → Pin 22
```

#### OLED Display Connection
```
SSD1306 → ESP32
VCC     → 3.3V
GND     → GND  
SDA     → Pin 21 (shared I2C)
SCL     → Pin 22 (shared I2C)
```

#### Moisture Sensors
```
Sensor 1 → ESP32 Pin 35
Sensor 2 → ESP32 Pin 34
VCC      → 3.3V
GND      → GND
```

### Step 3: Control System Assembly

#### Button Configuration
```cpp
// Each button needs pull-up resistor (10kΩ)
Button Pin → 10kΩ Resistor → 3.3V
Button Pin → Push Button → GND
```

#### Motor Control Circuit
```
ESP32 Pin 25 → 1kΩ Resistor → Relay Input
Relay COM    → +12V
Relay NO     → DC Motor (+)
Motor (-)    → GND
```

#### Valve Control Circuit
```
ESP32 Pin 26 → 1kΩ Resistor → Relay Input  
Relay COM    → +12V
Relay NO     → Solenoid Valve (+)
Valve (-)    → GND
```

### Step 4: Impact Detection System

#### Wire Monitoring Setup
```cpp
// Embed thin wires in fabric/material
Wire 1 → ESP32 Pin 12 → 10kΩ Pull-up → 3.3V
Wire 2 → ESP32 Pin 14 → 10kΩ Pull-up → 3.3V  
Wire 3 → ESP32 Pin 27 → 10kΩ Pull-up → 3.3V

// When wire breaks, pin reads LOW
```

### Step 5: Pneumatic System Assembly

#### Blood Pressure Cuff Integration
1. **Connect cuff** to air pump via tubing
2. **Install pressure sensor** in line
3. **Connect solenoid valve** for pressure release
4. **Test inflation/deflation cycle**

## ⚡ Power Requirements

### Power Consumption Analysis

| Component | Voltage | Current | Power |
|-----------|---------|---------|-------|
| ESP32 | 3.3V | 160mA | 0.53W |
| MAX30105 | 3.3V | 50mA | 0.17W |
| OLED Display | 3.3V | 20mA | 0.07W |
| Moisture Sensors | 3.3V | 5mA each | 0.03W |
| Buzzer | 5V | 30mA | 0.15W |
| LEDs | 3.3V | 20mA each | 0.07W |
| **Standby Total** | | | **~1.0W** |
| DC Motor | 12V | 500mA | 6.0W |
| Solenoid Valve | 12V | 200mA | 2.4W |
| Air Pump | 12V | 800mA | 9.6W |
| **Emergency Total** | | | **~19W** |

### Recommended Power Supply
- **Primary**: 12V 3A DC Adapter (36W capacity)
- **Backup**: 12V Li-ion Battery Pack (emergency operation)
- **Regulation**: LM2596 for 5V/3.3V rails

## 🧪 Testing & Calibration

### Initial System Test

1. **Power-On Test**
   ```cpp
   // Check all voltage rails
   3.3V Rail: 3.2V - 3.4V ✓
   5V Rail:   4.8V - 5.2V ✓
   12V Rail: 11.5V - 12.5V ✓
   ```

2. **I2C Device Detection**
   ```cpp
   // Run I2C scanner
   MAX30105:   Address 0x57 ✓
   SSD1306:    Address 0x3C ✓
   ```

3. **Sensor Calibration**
   - **Heart Rate**: Place finger on sensor, verify BPM reading
   - **Moisture**: Test with water drops, verify percentage change
   - **Temperature**: Compare with reference thermometer
   - **Wire Monitoring**: Disconnect wires, verify detection

### Performance Validation

| Test Parameter | Expected Range | Pass Criteria |
|----------------|----------------|---------------|
| Heart Rate Accuracy | ±5 BPM | Compare with medical device |
| Moisture Response Time | <2 seconds | Water drop test |
| Temperature Accuracy | ±1°C | Reference thermometer |
| Wire Break Detection | <100ms | Oscilloscope measurement |
| Motor Response Time | <5 seconds | Full inflation cycle |

## 🔍 Troubleshooting

### Common Issues

#### 1. ESP32 Not Starting
- **Check**: Power supply voltage (should be 5V or 3.3V)
- **Check**: USB cable connection
- **Solution**: Press BOOT button while uploading code

#### 2. I2C Devices Not Found
- **Check**: SDA/SCL connections (Pin 21/22)
- **Check**: Pull-up resistors (4.7kΩ recommended)
- **Solution**: Run I2C scanner sketch

#### 3. Inconsistent Sensor Readings
- **Check**: Power supply stability
- **Check**: Ground connections
- **Solution**: Add decoupling capacitors

#### 4. Motor Not Responding  
- **Check**: Relay coil voltage (should be 5V)
- **Check**: Motor power supply (12V)
- **Solution**: Test relay with multimeter

#### 5. False Bleeding Detection
- **Check**: Moisture sensor calibration
- **Check**: Environmental humidity
- **Solution**: Adjust threshold values in code

### Debug LED Patterns

| Pattern | Meaning | Action |
|---------|---------|--------|
| Solid Green | Normal Operation | None required |
| Solid Red | System Error | Check connections |
| Blinking Red | Sensor Error | Verify sensor connections |
| Alternating Red/Green | Emergency Mode | System responding to emergency |

## ⚠️ Safety Guidelines

### Electrical Safety
- **Never exceed voltage ratings** of components
- **Use proper fuses** in power circuits (2A for 12V line)
- **Isolate high voltage** from low voltage circuits
- **Add reverse polarity protection** diodes

### Mechanical Safety  
- **Limit cuff pressure** to safe medical ranges (300mmHg max)
- **Include pressure relief valve** for emergency release
- **Test all mechanical connections** before use
- **Use medical-grade materials** for patient contact

### Software Safety
- **Implement watchdog timers** for system reliability
- **Add emergency stop functionality** in all modes
- **Validate all sensor inputs** before acting
- **Include manual override** for all automated functions

### Medical Safety
⚠️ **CRITICAL**: This device is for emergency use only and should not replace professional medical care.

- **Always seek immediate professional medical attention** for serious injuries
- **Test device regularly** to ensure proper operation  
- **Train users** on proper operation and limitations
- **Maintain device** according to manufacturer specifications

## 📚 Additional Resources

- [ESP32 Pinout Reference](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)
- [MAX30105 Datasheet](https://datasheets.maximintegrated.com/en/ds/MAX30105.pdf)
- [SSD1306 OLED Guide](https://randomnerdtutorials.com/esp32-ssd1306-oled-display-arduino-ide/)
- [Blood Pressure Cuff Mechanics](https://en.wikipedia.org/wiki/Sphygmomanometer)

## 🔄 Revision History

| Version | Date | Changes |
|---------|------|---------|
| 1.0 | 2024-12-23 | Initial hardware design |
| 1.1 | 2025-01-03 | Added safety guidelines |
| 1.2 | Current | Enhanced documentation |

---

**⚠️ Disclaimer**: This hardware guide is for educational and research purposes. Always consult with medical professionals and follow local regulations when developing medical devices.