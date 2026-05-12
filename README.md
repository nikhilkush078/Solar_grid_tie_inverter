# ☀️ Solar Grid-Tie Inverter (Educational Model)

> **A hands-on exploration of power electronics, synchronization, and renewable energy conversion.**

---

## 📌 Project Overview
[cite_start]The primary aim of this project is to build a simple, transformer-based solar grid-tie inverter as a personal learning exercise[cite: 597]. [cite_start]Rather than focusing on high efficiency or commercial standards, the goal is to create a clear, hands-on model that demonstrates the essential "building blocks" of power electronics[cite: 598].

### 🎯 Key Learning Objectives
<p align="center">
  <img src="https://img.shields.io/badge/Logic-SPWM_Generation-orange?style=for-the-badge" alt="SPWM">
  <img src="https://img.shields.io/badge/Control-Grid_Synchronization-blue?style=for-the-badge" alt="Sync">
  <img src="https://img.shields.io/badge/Power-Voltage_Regulation-green?style=for-the-badge" alt="Regulation">
  <img src="https://img.shields.io/badge/Stability-Feedback_Loops-red?style=for-the-badge" alt="Feedback">
</p>

* [cite_start]**SPWM Generation:** Learning how to create the pulse width modulation required for a sine wave[cite: 601].
* [cite_start]**Basic Synchronization:** Exploring how an inverter aligns its output with an external reference signal[cite: 602].
* [cite_start]**Voltage Regulation:** Using a basic buck-boost stage to handle varying input from a solar source[cite: 603].
* [cite_start]**Feedback Loops:** Understanding how the system monitors itself to maintain steady operation[cite: 604].

---

## 🛠️ System Architecture

### 1. The Control Center (Arduino Nano)
[cite_start]The **Arduino Nano (U3)** acts as the brain of the system[cite: 612, 613]. [cite_start]It bridges the gap between physical sensors and power switches[cite: 614].
- [cite_start]**Signal Generation:** Produces the SPWM signals used to drive the inverter stage[cite: 615].
- [cite_start]**Intelligent Monitoring:** Reads data from feedback circuits to ensure correct pulse timing[cite: 616].

### 2. SEPIC Converter & Manual Adjustment
[cite_start]Responsible for managing power from the solar panel[cite: 618].
- [cite_start]**Manual Control:** Includes a **Potentiometer (RP1)** to adjust a reference signal sent to the Arduino[cite: 619, 620]. [cite_start]This allows the user to "dial in" the output behavior[cite: 621].
- [cite_start]**Flexible Topology:** The SEPIC design (L1, L2, C3, Q3) can both step up and step down solar voltage[cite: 622].

### 3. The Inverter Stage (Center-Tap Design)
[cite_start]Uses a **Center-Tapped Transformer (U1)** to keep the circuit simple and avoid high-side drivers[cite: 623, 624].
- [cite_start]**Push-Pull Operation:** Two MOSFETs act as switches, pulling current through primary windings in turns[cite: 625].

### 4. Synchronization & Feedback
- [cite_start]**Zero Crossing Detector (U4.1):** Uses an optocoupler to watch the AC waveform and send timing pulses to the Arduino[cite: 629, 630].
- [cite_start]**Output Feedback:** A bridge rectifier and voltage divider (at A0) provide a safe, low-voltage monitor of the output[cite: 631].
- [cite_start]**Input Feedback:** A simple divider (at A1) monitors solar panel voltage[cite: 632].

---

## 📋 Component List

| Category | Components |
| :--- | :--- |
| **Control** | [cite_start]Arduino Nano, 7805 Regulator, RP1 Potentiometer [cite: 827, 828, 829] |
| **DC-DC Stage** | [cite_start]2N7002 MOSFET, 1mH Inductors, BC557 Transistor [cite: 831, 834] |
| **Inverter Stage** | [cite_start]Center-Tapped Transformer, N-Channel MOSFETs, NPN Transistors [cite: 836, 837, 838] |
| **Feedback** | [cite_start]ILD74 Optocoupler, KAB100E Bridge Rectifier, 220ΚΩ/1kΩ Resistors [cite: 840, 841, 843, 844] |
| **Power** | [cite_start]Solar Panel (18V Max) [cite: 847] |

---

## ⚙️ How it Works

1.  [cite_start]**Capture:** The **Solar Panel** provides variable DC power[cite: 849, 850].
2.  [cite_start]**Regulate:** The **SEPIC Converter** stabilizes this input to a steady DC level[cite: 851].
3.  [cite_start]**Synchronize:** The **Zero Crossing Detector** acts as a "metronome," telling the Arduino when the AC wave hits 0V[cite: 858, 859, 860].
4.  [cite_start]**Invert:** The Arduino generates **SPWM** signals that drive the **Center-Tapped Transformer** to create 230V AC[cite: 862, 863, 865, 870].
5.  [cite_start]**Correct:** The **Feedback Loop** monitors the output and adjusts the SPWM width to keep the system stable[cite: 871, 872, 875].

---

## 📺 Project Demonstration
Check out the project in action here:
[![Solar Inverter Demo](https://img.shields.io/badge/YouTube-Watch%20Now-red?style=for-the-badge&logo=youtube)](https://www.youtube.com/watch?v=dQw4w9WgXcQ)
*(Note: Replace the link with your actual project video)*

---

## 👤 Author
**Nikhil Kushwah**
[cite_start]*Department of Electrical Engineering, SATI Vidisha* [cite: 595]

---
[cite_start]*Disclaimer: This project is strictly for educational demonstration and serves as an introductory step into renewable energy conversion[cite: 605].*
