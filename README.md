# 🏭 Predictive Maintenance Vibration Monitor

Industrial embedded system that monitors machine vibration,
detects bearing wear patterns and triggers maintenance alerts
before catastrophic failure occurs.

## 📽️ Live Simulation
[

![Open in Wokwi](https://img.shields.io/badge/Open%20in-Wokwi-green)

](https://wokwi.com/projects/468105767087446017)

## 🔧 Hardware Components
| Component | Purpose |
|-----------|---------|
| Arduino Uno (ATmega328P) | Main microcontroller |
| Potentiometer | Simulates vibration sensor |
| Green LED | Normal operation indicator |
| Yellow LED | Warning indicator |
| Red LED | Critical fault indicator |
| Buzzer | Maintenance alarm |
| Push Button | Manual reset after maintenance |
| 220Ω Resistors x3 | LED current limiting |
| 10kΩ Resistor | Button pull-down |

## 💻 Software Concepts
- ADC sampling via analogRead()
- Rolling average (10 samples) for noise filtering
- Finite State Machine: Normal → Warning → Critical
- Predictive counter logic (not instant trigger)
- Manual reset safety lock

## 🚦 System States
| State | LED | Condition |
|-------|-----|-----------|
| Normal | 🟢 Green | Vibration below 300 |
| Warning | 🟡 Yellow | Vibration 300-600 for 5+ readings |
| Critical | 🔴 Red + Buzzer | Vibration 600+ for 10+ readings |
| Reset | 🟢 Green | Manual button press after maintenance |

## 🏭 Real World Applications
- CNC machine bearing monitoring
- Industrial pump fault detection
- Conveyor belt health monitoring
- Wind turbine predictive maintenance

## 📚 References
- Valvano – Embedded Systems: Introduction to ARM Cortex-M
- Barr – Programming Embedded Systems in C and C++
- Laplante – Real-Time Systems Design and Analysis
- Bräunl – Embedded Robotics, 4th Ed., Springer 2022

## 👥 Team
TH Deggendorf | Campus Cham
Embedded Systems — Coding/Simulation Project
