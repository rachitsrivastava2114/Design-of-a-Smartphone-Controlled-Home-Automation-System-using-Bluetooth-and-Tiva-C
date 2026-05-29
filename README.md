# Smartphone-Controlled Home Automation System

<p align="center">
  <a>
    <img src="images/tivac.png" alt="Hardware Setup" width="700">
  </a>

  <h3 align="center">Embedded Home Automation using Bluetooth and Tiva C</h3>

  <p align="center">
    Wireless control of LEDs, motor, and relay using HC-05 Bluetooth and TM4C123GH6PM
    <br />
    <strong>Embedded Systems Design Project</strong>
    <br /><br />
    <a href="https://github.com/rachitsrivastava2114/Design-of-a-Smartphone-Controlled-Home-Automation-System-using-Bluetooth-and-Tiva-C/issues">Report Bug</a>
    ·
    <a href="https://github.com/rachitsrivastava2114/Design-of-a-Smartphone-Controlled-Home-Automation-System-using-Bluetooth-and-Tiva-C">View Project</a>
  </p>
</p>

---

## Table of Contents

- [Project Overview](#project-overview)
  - [Objectives](#objectives)
  - [Key Features](#key-features)
  - [System Architecture](#system-architecture)
- [Hardware Components Used](#hardware-components-used)
- [Software \& Tools](#software--tools)
- [Pin Configuration](#pin-configuration)
- [Command Mapping](#command-mapping)
- [Working Principle](#working-principle)
- [System Flow](#system-flow)
- [Software Flow](#software-flow)
- [Project Visualization](#project-visualization)
- [Results](#results)
- [Applications](#applications)
- [Future Improvements](#future-improvements)
- [Author](#author)
- [License](#license)

---

## Project Overview

This project implements a **smartphone-controlled home automation system** using the **TM4C123GH6PM Tiva C LaunchPad** and an **HC-05 Bluetooth module**. It allows wireless control of **onboard LEDs**, a **DC motor/fan**, and a **relay module** through serial commands sent from a smartphone.

The system uses **UART1 communication** for Bluetooth interfacing and follows an **interrupt-driven architecture**, which improves responsiveness and reduces unnecessary CPU usage.

This project demonstrates practical concepts of:
- UART communication
- GPIO interfacing
- Interrupt handling
- Motor and relay control
- Real-time embedded system design

---

## Objectives

- Design a microcontroller-based home automation system
- Enable wireless control using a smartphone via Bluetooth
- Control LEDs, relay, and motor in real time
- Implement **interrupt-based UART communication**
- Develop a low-cost and efficient automation prototype

---

## Key Features

- 📱 Smartphone-based wireless control
- 🔵 Bluetooth communication via **HC-05**
- 💡 Control of **Red, Blue, and Green onboard LEDs**
- 🔁 Motor control in **forward** and **reverse** direction
- ⚡ Relay switching for load control
- ⚙️ Interrupt-driven command handling
- 🧠 Lightweight and efficient embedded implementation

---

## System Architecture

The system is centered around the **TM4C123GH6PM Tiva C LaunchPad**, which acts as the main controller. A smartphone sends commands through the **HC-05 Bluetooth module**, which communicates with the microcontroller through **UART1**.

Based on the received command:
- LEDs are toggled
- Motor direction is controlled
- Relay is switched ON/OFF

The relay and motor driver are powered separately for better stability, while the Tiva C board handles logic and control.

<p align="center">
  <img src="images/system-block-diagram.png" alt="System Block Diagram" width="800">
</p>

---

## Hardware Components Used

- **TM4C123GH6PM Tiva C LaunchPad**
- **HC-05 Bluetooth Module**
- **Relay Module**
- **L293D Motor Driver**
- **DC Motor / Fan**
- **Onboard RGB LEDs**
- **Battery**
- **Buck Converter**
- **USB Power Supply**
- Breadboard and connecting wires

---

## Software & Tools

- **Code Composer Studio (CCS)** – for Embedded C development
- **TivaWare Peripheral Driver Library** – for peripheral configuration
- **Bluetooth Serial Terminal App** – to send commands from smartphone

---

## Pin Configuration

| Tiva C Pin | Connected Device | Function |
|-----------|------------------|----------|
| PB0 | HC-05 TX | UART1 RX (receives Bluetooth data) |
| PB1 | HC-05 RX | UART1 TX (transmits data) |
| PB2 | Relay Module | Relay ON/OFF control |
| PB3 | Motor Driver IN1 | Motor forward control |
| PB4 | Motor Driver IN2 | Motor reverse control |
| PF1 | Onboard Red LED | Red LED indication |
| PF2 | Onboard Blue LED | Blue LED indication |
| PF3 | Onboard Green LED | Green LED indication |
| 5V | HC-05 VCC | Power supply |
| GND | All Modules | Common ground |

---

## Command Mapping

The system uses **single-character commands** received over UART.

| Command | Action |
|---------|--------|
| `A` | Toggle Red LED |
| `B` | Toggle Green LED |
| `C` | Toggle Blue LED |
| `F` | Toggle Motor Forward + Relay ON/OFF |
| `R` | Toggle Motor Reverse + Relay ON/OFF |
| `X` | Turn OFF all LEDs, stop motor, relay OFF |

---

## Working Principle

- The smartphone sends a command using a Bluetooth serial terminal app.
- The **HC-05** receives the command and transfers it to **UART1** of the Tiva C board.
- A **UART interrupt** is triggered whenever data arrives.
- Inside the interrupt service routine:
  - the command is read,
  - matched using a `switch-case`,
  - and the corresponding control action is executed.
- The system then returns to the idle main loop and waits for the next command.

### Operation Summary

- ✅ `A`, `B`, `C` → Toggle LEDs
- ✅ `F` → Start/Stop motor in forward direction
- ✅ `R` → Start/Stop motor in reverse direction
- ✅ `X` → Emergency stop / reset all outputs

---

## System Flow

<p align="center">
  <img src="images/block_diagram.png" alt="System Flowchart" width="450">
</p>

## Software Flow

<p align="center">
  <img src="images/flow_project.png" alt="Software Flow" width="450">
</p>

### Flow Summary

<p align="center">
  <img src="images/system_flowchart.png" alt="Flow Summary" width="450">
</p>

## Project Visualization

from graphviz import Digraph

def create_system_diagram():
    # Initialize a new directed graph
    # format='svg' is great for READMEs because it scales without losing quality
    dot = Digraph(name='Hardware System', format='svg')
    
    # Set global graph attributes (Top-to-Bottom layout, nice font)
    dot.attr(rankdir='TB', fontname='Helvetica,Arial,sans-serif')
    dot.attr('node', shape='box', style='rounded, filled', fillcolor='#f8f9fa', fontname='Helvetica')
    dot.attr('edge', fontname='Helvetica', fontsize='10')

    # Define the Nodes (The hardware components)
    dot.node('App', 'Smartphone App\n(Bluetooth Commands)')
    dot.node('HC05', 'Bluetooth Module\n(HC-05)')
    dot.node('MCU', 'TM4C123GH6PM\nMicrocontroller', fillcolor='#e2ebf0') # Highlight the MCU
    dot.node('LED', 'RGB LED')
    dot.node('Driver', 'Motor Driver')
    dot.node('Motor', 'DC Motor')

    # Define the Edges (The connections and communication protocols)
    dot.edge('App', 'HC05', label=' Bluetooth')
    dot.edge('HC05', 'MCU', label=' UART')
    dot.edge('MCU', 'LED')
    dot.edge('MCU', 'Driver')
    dot.edge('Driver', 'Motor')

    # Save and render the file
    filename = 'hardware_architecture'
    dot.render(filename, cleanup=True)
    print(f"Diagram successfully generated as {filename}.svg")

if __name__ == '__main__':
    create_system_diagram()
