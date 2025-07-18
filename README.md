# Sound-Activated Timer  
Author: Cătălin Gabriel Badea  
Date: 12.01.2025

## 🔧 Project Description

This project presents a practical and hands-free solution to time tracking during physical activities using sound detection.  
The system acts as a **sound-activated timer**, controlled entirely by **claps or loud sounds**.  

It was developed using an **Arduino-compatible microcontroller**, a **microphone sensor**, a **4-digit 7-segment display**, two **LEDs** for visual feedback, and a **buzzer** for audio signals.

Ideal for workouts or tasks where physical interaction with a stopwatch is inconvenient.

---

## 🎯 Functional Overview

- **Start/Stop Timer**: One loud sound (e.g., a clap) toggles between running and paused states.
- **Reset Timer**: Two quick claps reset the timer and trigger a high-pitch buzzer sound.
- **Live Time Display**: The 7-segment display shows time in the format `minutes.seconds` (e.g., `2.45` = 2 minutes 45 seconds).
- **Visual Feedback**:  
  - Green LED = Timer is active  
  - Red LED = Timer is paused  
- **Auditory Feedback**:  
  - Short beep every second  
  - High-pitch beep on reset

---

## 📹 Media

You can watch a short demonstration video of the timer in action [here](https://drive.google.com/drive/folders/1r08ukjEGmn38x3a43wrLAsxnVqnwrNJP).

---

## 🚀 Possible Future Expansions

- Add real-time clock (RTC) module for accurate tracking beyond seconds.
- Use Bluetooth or Wi-Fi (ESP32) for mobile notifications or remote control.
- Add OLED display for more compact, modern UI.
- Include capacitive touch or gesture-based input as an alternative to sound.
- Use machine learning (TinyML) to distinguish between types of sounds or voice commands.

---

📄 **More technical details and wiring diagrams are available in the project documentation.**

---

