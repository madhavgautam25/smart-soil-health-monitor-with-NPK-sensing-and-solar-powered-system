🌱 Smart Soil Health Monitoring System
Solar-Powered IoT-Based Soil Analytics Platform
1. Executive Summary

The Smart Soil Health Monitoring System is a solar-powered IoT-based embedded solution designed to monitor critical soil parameters in real time.

The system integrates multiple sensors to measure:

Soil pH

Soil Moisture

Soil Temperature

Ambient Light Intensity

Soil Nutrients (Nitrogen, Phosphorus, Potassium – NPK)

Sensor data is processed using an ESP32 microcontroller, displayed locally on a TFT interface, and transmitted to the ThingSpeak cloud platform for remote monitoring and analysis.

The solution is specifically designed for rural and off-grid agricultural environments, leveraging renewable energy for sustainable operation.

2. Problem Statement

Farmers often rely on manual soil inspection and estimation-based fertilizer application. This can result in:

Overuse or underuse of fertilizers

Poor irrigation management

Increased production costs

Soil degradation over time

There is a need for a low-cost, portable, and real-time soil monitoring system that provides accurate data-driven insights.

3. Proposed Solution

The Smart Soil Health Monitoring System provides:

Real-time soil parameter monitoring

Nutrient-level detection via NPK sensing

Solar-powered energy independence

On-device display for field usage

Cloud-based data storage for remote access

This enables precision agriculture and optimized farm resource management.

4. System Architecture
4.1 Hardware Flow

Soil Sensors → ESP32 → TFT Display
↓
WiFi → ThingSpeak Cloud

4.2 Power Flow

Solar Panel → 3S Charging Module → Lithium Battery Pack → Buck Converter → System Components

5. Hardware Components
Component	Function
ESP32	Core microcontroller with WiFi capability
4-in-1 Soil Sensor	Measures pH, moisture, temperature, light
NPK Sensor (RS485)	Measures Nitrogen, Phosphorus, Potassium
MAX485 Module	RS485 to UART communication interface
1.8” TFT Display (ST7735)	Real-time local data display
12V 10W Solar Panel	Renewable energy source
3S Lithium Battery Pack	Energy storage
3S Charging Module	Battery management
Buck Converter	Voltage regulation (12V → 5V)
6. Software Stack

Arduino IDE

ESP32 Board Support Package

Adafruit_GFX Library

Adafruit_ST7735 Library

ModbusMaster Library

WiFi.h

HTTPClient.h

ThingSpeak Cloud Platform

7. Functional Description
7.1 Soil Parameter Monitoring

The 4-in-1 soil sensor continuously transmits:

pH value

Moisture level

Soil temperature

Light intensity

Data is parsed and processed via UART communication.

7.2 Nutrient Detection (NPK)

The NPK sensor communicates using RS485 Modbus protocol through a MAX485 interface. Nutrient values are read from holding registers and displayed on the TFT interface.

7.3 Local Display

All parameters are shown in real time on a 1.8-inch TFT display, enabling immediate field-level decision-making.

7.4 Cloud Integration

Data is uploaded to ThingSpeak at fixed intervals (15 seconds), enabling:

Historical data tracking

Graph visualization

Remote monitoring

8. Power Management

The system is powered through:

12V Solar Panel

3S Lithium Battery Pack

Buck Converter for voltage stabilization

This ensures uninterrupted operation in rural environments without dependency on grid power.

9. Key Features

Solar-powered portable system

Real-time soil analytics

Nutrient monitoring capability

Cloud connectivity

Low-power design

Scalable architecture

10. Applications

Precision Farming

Smart Irrigation Systems

Fertilizer Optimization

Soil Health Assessment

Agricultural Research

Sustainable Farming Initiatives

11. Limitations

Sensor calibration required for high-accuracy deployment

NPK sensor communication dependent on correct Modbus configuration

Internet dependency for cloud functionality

12. Future Enhancements

AI-based fertilizer recommendation engine

Mobile application dashboard

LoRa-based long-range communication

Automated irrigation control system

Machine learning for crop yield prediction

13. Project Showcase

This project was showcased at TechInnovate 3.0, organized by the Department of Interdisciplinary Courses in Engineering (DICE), Chitkara University, and was shortlisted among the top projects under Embedded Systems & IoT.

14. Academic Context

Developed as part of Embedded Systems & IoT coursework, demonstrating integration of:

Sensor interfacing

UART & RS485 communication

Cloud-based IoT deployment

Renewable energy integration

15. Mentor

Dr. Manvinder Sharma, 
Department of Interdisciplinary Courses in Engineering
Chitkara University

16. License

This project is developed for academic and research purposes.
