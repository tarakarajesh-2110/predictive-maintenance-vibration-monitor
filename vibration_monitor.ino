#include <avr/wdt.h>

// ─── Pin Definitions ───────────────────────────
#define VIBRATION_PIN  A0
#define GREEN_LED       6
#define YELLOW_LED      7
#define RED_LED         8
#define BUZZER          9
#define RESET_BTN       2

// ─── Thresholds ────────────────────────────────
#define NORMAL_MAX     300
#define WARNING_MAX    600
#define WARNING_COUNT    5
#define CRITICAL_COUNT  10

// ─── Global Variables ──────────────────────────
int vibrationValue  = 0;
int warningCounter  = 0;
int criticalCounter = 0;
bool maintenanceDue = false;

// Rolling average (10 samples)
int  readings[10];
int  readIndex = 0;
long total     = 0;
int  average   = 0;

// ─── Rolling Average ───────────────────────────
int getRollingAverage(int newReading) {
  total -= readings[readIndex];
  readings[readIndex] = newReading;
  total += readings[readIndex];
  readIndex = (readIndex + 1) % 10;
  return total / 10;
}

// ─── State Functions ───────────────────────────
void normalState() {
  digitalWrite(GREEN_LED,  HIGH);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED,    LOW);
  digitalWrite(BUZZER,     LOW);
}

void warningState() {
  digitalWrite(GREEN_LED,  LOW);
  digitalWrite(YELLOW_LED, HIGH);
  digitalWrite(RED_LED,    LOW);
  digitalWrite(BUZZER,     LOW);
  Serial.println("WARNING: Vibration rising - monitor closely");
}

void criticalState() {
  digitalWrite(GREEN_LED,  LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED,    HIGH);
  digitalWrite(BUZZER,     HIGH);
  delay(100);
  digitalWrite(BUZZER,     LOW);
  Serial.println("CRITICAL: Maintenance Required Immediately!");
}

void resetSystem() {
  warningCounter  = 0;
  criticalCounter = 0;
  maintenanceDue  = false;
  Serial.println("Maintenance Done - System Reset");
  Serial.println("Resuming normal monitoring...");
  normalState();
}

// ─── Setup ─────────────────────────────────────
void setup() {
  Serial.begin(9600);

  pinMode(GREEN_LED,  OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED,    OUTPUT);
  pinMode(BUZZER,     OUTPUT);
  pinMode(RESET_BTN,  INPUT);

  // Initialize rolling average array
  for (int i = 0; i < 10; i++) readings[i] = 0;

  Serial.println("=================================");
  Serial.println(" Predictive Maintenance System  ");
  Serial.println(" Vibration Monitor v1.0         ");
  Serial.println("=================================");
  Serial.println("System Initialized - Monitoring Started");

  normalState();
}

// ─── Main Loop ─────────────────────────────────
void loop() {

  // Reset button check
  if (digitalRead(RESET_BTN) == HIGH && maintenanceDue) {
    resetSystem();
    delay(500);
    return;
  }

  // Read vibration sensor
  vibrationValue = analogRead(VIBRATION_PIN);
  average = getRollingAverage(vibrationValue);

  // Serial Monitor dashboard
  Serial.print("Vibration: ");
  Serial.print(vibrationValue);
  Serial.print(" | Avg: ");
  Serial.print(average);
  Serial.print(" | Warn#: ");
  Serial.print(warningCounter);
  Serial.print(" | Crit#: ");
  Serial.println(criticalCounter);

  // ─── State Machine ───
  if (!maintenanceDue) {

    if (average < NORMAL_MAX) {
      warningCounter  = 0;
      criticalCounter = 0;
      normalState();
      Serial.println("Status: NORMAL - System Healthy");

    } else if (average < WARNING_MAX) {
      warningCounter++;
      if (warningCounter >= WARNING_COUNT) {
        warningState();
      }

    } else {
      warningCounter++;
      criticalCounter++;
      if (criticalCounter >= CRITICAL_COUNT) {
        maintenanceDue = true;
        criticalState();
        Serial.println("MACHINE SHUTDOWN RECOMMENDED");
        Serial.println("Press RESET button after maintenance");
      }
    }

  } else {
    // Locked in fault — waiting for reset
    criticalState();
    Serial.println("Waiting for maintenance reset...");
  }

  delay(300);
}
