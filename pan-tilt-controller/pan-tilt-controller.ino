// ============================================================
// JARVIS PAN-TILT CONTROLLER v1.0
// Forge Two Engineering — Oskar Trujillo
//
// Pan:  SG90 Servo on Pin 9  (absolute angle: 0-180)
// Tilt: 28BYJ-48 Stepper on Pins 2,3,4,5 via ULN2003
//       (relative steps: + = up, - = down)
//
// Serial commands at 9600 baud:
//   P[0-180]   Pan to absolute angle
//   T[steps]   Tilt by relative steps (+ up, - down)
//   C          Center/home both axes
//   S          Toggle scan mode
//   ?          Report current positions
//
// This is the same serial protocol the Raspberry Pi will
// use for face tracking. Swap the Serial Monitor for a
// Python script and the system works identically.
// ============================================================

#include <Servo.h>
#include <Stepper.h>

// --- PIN ASSIGNMENTS ---
const int PAN_PIN      = 9;
const int STEP_IN1     = 2;
const int STEP_IN2     = 3;
const int STEP_IN3     = 4;
const int STEP_IN4     = 5;

// --- MOTOR CONFIGURATION ---
const int STEPS_PER_REV = 2048;
const int STEPPER_SPEED  = 10;

// --- POSITION TRACKING ---
int panAngle    = 90;
int tiltSteps   = 0;
const int TILT_MAX =  500;
const int TILT_MIN = -500;

// --- SCAN MODE VARIABLES ---
bool scanning         = false;
int scanPanAngle      = 0;
int scanPanDirection  = 1;
unsigned long lastScanTime = 0;
const int SCAN_INTERVAL    = 30;

// --- MOTOR OBJECTS ---
Servo panServo;
Stepper tiltStepper(STEPS_PER_REV, STEP_IN1, STEP_IN3, STEP_IN2, STEP_IN4);

// --- INPUT BUFFER ---
String inputBuffer = "";


void setup() {
  panServo.attach(PAN_PIN);
  panServo.write(panAngle);
  tiltStepper.setSpeed(STEPPER_SPEED);
  Serial.begin(9600);
  while (!Serial) { ; }

  Serial.println();
  Serial.println(F("=========================================="));
  Serial.println(F("    JARVIS Pan-Tilt Controller v1.0"));
  Serial.println(F("    Forge Two Engineering"));
  Serial.println(F("=========================================="));
  Serial.println(F("  Pan:  SG90 Servo     (Pin 9)"));
  Serial.println(F("  Tilt: 28BYJ-48       (Pins 2-5)"));
  Serial.println(F("------------------------------------------"));
  Serial.println(F("  Commands:"));
  Serial.println(F("   P[0-180]  Pan to angle"));
  Serial.println(F("   T[steps]  Tilt by steps (+/-)"));
  Serial.println(F("   C         Center / Home"));
  Serial.println(F("   S         Toggle scan mode"));
  Serial.println(F("   ?         Show positions"));
  Serial.println(F("=========================================="));
  Serial.println();
  reportPosition();
}


void loop() {
  readSerialInput();
  if (scanning) {
    runScanMode();
  }
}


void readSerialInput() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (inputBuffer.length() > 0) {
        processCommand(inputBuffer);
        inputBuffer = "";
      }
    } else {
      inputBuffer += c;
    }
  }
}


void processCommand(String cmd) {
  cmd.trim();
  cmd.toUpperCase();
  char type = cmd.charAt(0);

  switch (type) {
    case 'P': {
      if (scanning) { scanning = false; Serial.println(F("[Scan mode stopped]")); }
      int newAngle = cmd.substring(1).toInt();
      newAngle = constrain(newAngle, 0, 180);
      smoothPan(newAngle);
      panAngle = newAngle;
      Serial.print(F("Pan -> ")); Serial.print(panAngle); Serial.println(F(" degrees"));
      break;
    }
    case 'T': {
      if (scanning) { scanning = false; Serial.println(F("[Scan mode stopped]")); }
      int steps = cmd.substring(1).toInt();
      int newPosition = tiltSteps + steps;
      if (newPosition > TILT_MAX || newPosition < TILT_MIN) {
        if (newPosition > TILT_MAX) { steps = TILT_MAX - tiltSteps; }
        else { steps = TILT_MIN - tiltSteps; }
        Serial.println(F("[Tilt limit reached]"));
      }
      if (steps != 0) { tiltStepper.step(steps); tiltSteps += steps; }
      Serial.print(F("Tilt -> step ")); Serial.print(tiltSteps);
      Serial.print(F(" (moved ")); Serial.print(steps); Serial.println(F(")"));
      break;
    }
    case 'C': {
      if (scanning) { scanning = false; Serial.println(F("[Scan mode stopped]")); }
      Serial.println(F("Centering..."));
      smoothPan(90); panAngle = 90;
      if (tiltSteps != 0) {
        Serial.print(F("Tilt homing (")); Serial.print(-tiltSteps); Serial.println(F(" steps)..."));
        tiltStepper.step(-tiltSteps); tiltSteps = 0;
      }
      Serial.println(F("Both axes centered.")); reportPosition();
      break;
    }
    case 'S': {
      scanning = !scanning;
      if (scanning) {
        scanPanAngle = panAngle; scanPanDirection = 1;
        Serial.println(F("Scan mode ON — Jarvis is surveying..."));
        Serial.println(F("(Send any command to stop)"));
      } else {
        Serial.println(F("Scan mode OFF"));
        panAngle = scanPanAngle; reportPosition();
      }
      break;
    }
    case '?': { reportPosition(); break; }
    default: {
      Serial.print(F("Unknown command: ")); Serial.println(cmd);
      Serial.println(F("Try P90, T100, T-50, C, S, or ?"));
      break;
    }
  }
}


void smoothPan(int targetAngle) {
  int currentAngle = panAngle;
  if (currentAngle < targetAngle) {
    for (int a = currentAngle; a <= targetAngle; a++) { panServo.write(a); delay(10); }
  } else {
    for (int a = currentAngle; a >= targetAngle; a--) { panServo.write(a); delay(10); }
  }
}


void runScanMode() {
  unsigned long now = millis();
  if (now - lastScanTime >= SCAN_INTERVAL) {
    lastScanTime = now;
    scanPanAngle += scanPanDirection;
    if (scanPanAngle >= 180) { scanPanAngle = 180; scanPanDirection = -1; }
    else if (scanPanAngle <= 0) { scanPanAngle = 0; scanPanDirection = 1; }
    panServo.write(scanPanAngle);
  }
}


void reportPosition() {
  Serial.println(F("-------------------------"));
  Serial.print(F("  Pan:  ")); Serial.print(panAngle);
  Serial.println(F(" deg (0=left, 90=center, 180=right)"));
  Serial.print(F("  Tilt: step ")); Serial.print(tiltSteps);
  Serial.print(F(" (range: ")); Serial.print(TILT_MIN);
  Serial.print(F(" to ")); Serial.print(TILT_MAX); Serial.println(F(")"));
  Serial.println(F("-------------------------"));
}
