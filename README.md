# Mobile Jarvis

### A room-aware AI robot that sees you, speaks to you, and drives to you.

**Status:** 🟡 In Progress — Pan-tilt tracking head built, voice and mobility phases next

---

## What Is This?

Mobile Jarvis is an autonomous AI assistant on wheels. It combines computer vision, voice interaction, and physical mobility into a single robot that can:

- **See** — Detect and track your face using OpenCV and a camera mounted on a pan-tilt head
- **Speak** — Listen to voice commands via Whisper, think using Claude API, and respond out loud through a speaker
- **Move** — Drive toward you on a wheeled chassis, navigate around obstacles, and patrol a room
- **React** — Detect your body pose and activity (sitting, standing, sleeping) and respond accordingly

You walk into your room. Jarvis spots your face from across the room, drives toward you, stops at a comfortable distance, looks up at you, and says "Welcome home."

---

## Build Phases

### Phase 1: Voice Jarvis ✅ Planning Complete
- Claude API integration — text chat working in terminal
- Whisper speech-to-text — mic input to Claude to spoken reply
- Microphone + speaker hardware setup

### Phase 2: Vision — Camera Integration 🔲 Next
- USB camera mounted on pan-tilt head
- OpenCV + YOLOv8 for face/person detection
- Claude Vision API — Jarvis can "see" and describe the room
- Triggers: person detected → Jarvis greets you

### Phase 3: Intelligence — Action Detection 🔲 Upcoming
- MediaPipe Pose for body pose detection
- Activity classifier: studying → productive mode, lying down → check-in
- Context-aware prompts to Claude API

### Phase 4: Mobility — The Robot Body 🔲 Upcoming
- 2WD chassis with DC motors
- L298N motor driver controlled by Arduino
- Navigation logic: drive toward detected face, avoid obstacles
- Ultrasonic sensors for collision avoidance

### Phase 5: Launch — Compete & Present 🔲 Future
- SARSEF, Conrad Challenge, Hack Club hackathons
- Polished demo video
- Full project documentation

---

## Hardware — What We're Using

| Component | Role | Status |
|-----------|------|--------|
| Raspberry Pi 5 | Main brain — runs all AI software | 🔲 Need to buy |
| Arduino Uno R4 WiFi | Motor controller — pan, tilt, drive | ✅ Owned |
| SG90 Micro Servo | Pan axis (left/right) | ✅ Owned |
| 28BYJ-48 Stepper Motor | Tilt axis (up/down) | ✅ Owned |
| ULN2003 Driver Board | Drives the stepper motor | ✅ Owned |
| Bambu Lab A1 3D Printer | Prints robot shell and all brackets | ✅ Owned |
| USB Camera Module | Eyes — face detection and tracking | 🔲 Need to buy |
| 2WD Chassis Kit | Wheeled platform with DC motors | 🔲 Need to buy |
| L298N Motor Driver | Controls wheel motors | 🔲 Need to buy |
| Speaker + Microphone | Voice input and output | 🔲 Need to buy |
| Battery Pack | Powers the mobile platform | 🔲 Need to buy |

---

## Team Split

**Ivan Martinez (Brain):** All Python code — Claude API, Whisper, OpenCV, YOLOv8, MediaPipe, navigation logic, face tracking algorithms

**Oskar Trujillo (Body):** All physical hardware — wheeled chassis, DC motors, L298N wiring, pan-tilt head, 3D printed shell, Arduino motor code, Raspberry Pi setup, power system, camera/mic/speaker mounting

---

## Serial Command Protocol

The Raspberry Pi sends text commands to the Arduino over USB serial at 9600 baud:

| Command | Action | Example |
|---------|--------|---------|
| `P[0-180]` | Pan servo to angle | `P90` (center) |
| `T[steps]` | Tilt stepper by steps (+/-) | `T100` (tilt up) |
| `C` | Center both axes | `C` |
| `S` | Toggle scan mode | `S` |
| `?` | Report positions | `?` |
| `MF` | Drive forward | *Phase 4* |
| `MB` | Drive backward | *Phase 4* |
| `ML` | Turn left | *Phase 4* |
| `MR` | Turn right | *Phase 4* |
| `MS` | Stop driving | *Phase 4* |

---

## Code

- [`pan-tilt-controller/`](pan-tilt-controller/) — Arduino sketch for the pan-tilt tracking head (working)
- More code coming as each phase is completed

---

## Links

- 🏠 [Forge Two Engineering](https://github.com/ForgeTwoEngineering)
- 📺 [YouTube — Build Journey](https://www.youtube.com/channel/UCjRHb705PTnDNKvWwySeZPQ)
- 📧 forgetwoengineering@gmail.com
