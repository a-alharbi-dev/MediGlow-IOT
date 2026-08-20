#include <WiFi.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include <time.h>

// ===================== WiFi =====================
const char* ssid = "Med";
const char* password = "11223366m";

// ===================== ThingSpeak =====================
String thingspeakApiKey = "8YVCQ7URU0KT6IQS";

// ===================== Time (GMT+3) =====================
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3 * 3600;
const int   daylightOffset_sec = 0;

// ===================== Pins =====================
#define TRIG 12
#define ECHO 13

#define RED_PIN   25
#define GREEN_PIN 26
#define BLUE_PIN  27

#define BUZZER_PIN 14

// ===================== Expiry date =====================
int expDay = 27;
int expMonth = 12;
int expYear = 2025;
unsigned long MED_EXPIRY_EPOCH = 0;

// ===================== Levels (cm) =====================
float fullLevel  = 2.5;
float emptyLevel = 4.88;

// ===================== Timers =====================
unsigned long lastPrint = 0;
unsigned long lastCloudUpdate = 0;
const unsigned long cloudInterval = 20000;

// ===================== Confirmation =====================
const unsigned long confirmTime = 5000;

// ===================== Buzzer =====================
bool buzzerActive = false;
unsigned long buzzerStart = 0;
const unsigned long buzzerDuration = 5000;

// ===================== Medication =====================
bool medActive = false;
unsigned long medStartTime = 0;
const unsigned long medDuration = 120000; // ADD: كانت 60000 (دقيقة)
int medHour = 12;
int medMinute = 45;

// ===================== Expiry ADD =====================
bool expiryBuzzed = false; // ADD

// ===================== LOW confirmation =====================
unsigned long lowCandidateSince = 0;
unsigned long lowClearSince = 0;
bool lowConfirmed = false;
bool lowLatched = false;

// =====================================================

void setup() {
  Serial.begin(115200);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  noBuzz();
  rgbBlue();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  MED_EXPIRY_EPOCH = convertDateToEpoch(expDay, expMonth, expYear);
}

// =====================================================

void loop() {

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return;

  int hour   = timeinfo.tm_hour;
  int minute = timeinfo.tm_min;
  int second = timeinfo.tm_sec;

  float level = readUltrasonic();
  float fillPercent = getFillPercent(level);
  long daysLeft = getDaysLeft();

  if (millis() - lastPrint >= 1000) {
    lastPrint = millis();
    Serial.printf(
      "⏰ %02d:%02d:%02d | Dist %.2f | Fill %.1f%% | LOW=%d | med=%d\n",
      hour, minute, second, level, fillPercent, lowConfirmed, medActive
    );
  }

  bool lowRaw = (fillPercent <= 20);

  if (lowRaw && !lowConfirmed) {
    if (lowCandidateSince == 0)
      lowCandidateSince = millis();
    else if (millis() - lowCandidateSince >= confirmTime) {
      lowConfirmed = true;
      lowCandidateSince = 0;
    }
  }

  if (!lowRaw && lowConfirmed) {
    if (lowClearSince == 0)
      lowClearSince = millis();
    else if (millis() - lowClearSince >= confirmTime) {
      lowConfirmed = false;
      lowClearSince = 0;
      lowLatched = false;
    }
  }

  if (!lowRaw) lowCandidateSince = 0;

  if (lowConfirmed && !lowLatched && !medActive) {
    Serial.println("⚠️ LOW LEVEL CONFIRMED");
    lowLatched = true;

    buzz();
    buzzerActive = true;
    buzzerStart = millis();
  }

  // ===================== MEDICATION =====================
  bool medTriggerNow = (hour == medHour && minute == medMinute && second < 5);

  if (medTriggerNow && !medActive) {
    medActive = true;
    medStartTime = millis();

    Serial.println("🚨 MEDICATION TIME");

    buzz();
    buzzerActive = true;
    buzzerStart = millis();
  }

  if (medActive && millis() - medStartTime >= medDuration) {
    medActive = false;
  }

  // ===================== EXPIRY ADD ONLY =====================
  if (daysLeft <= 0 && !expiryBuzzed) {
    expiryBuzzed = true;
    buzz();
    buzzerActive = true;
    buzzerStart = millis();
  }

  if (buzzerActive && millis() - buzzerStart >= buzzerDuration) {
    noBuzz();
    buzzerActive = false;
  }

  // ===================== RGB (ORIGINAL LOGIC + ADD) =====================
  if (medActive)         rgbRed();
  else if (lowConfirmed) rgbYellow();
  else if (daysLeft <= 0) rgbGreen(); // ADD
  else                   rgbBlue();

  if (millis() - lastCloudUpdate >= cloudInterval) {
    lastCloudUpdate = millis();
    sendToThingSpeak(medActive ? 1 : 0, fillPercent, daysLeft);
  }

  delay(10);
}

// =====================================================
float readUltrasonic() {
  float sum = 0;
  int valid = 0;

  for (int i = 0; i < 7; i++) {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    long duration = pulseIn(ECHO, HIGH, 25000);
    if (duration == 0) continue;

    float cm = duration * 0.034 / 2;
    if (cm < fullLevel - 0.2 || cm > emptyLevel + 0.2) continue;

    sum += cm;
    valid++;
    delay(5);
  }

  if (valid == 0) return emptyLevel;
  return sum / valid;
}

// =====================================================
float getFillPercent(float level) {
  if (level >= emptyLevel) return 0;
  if (level <= fullLevel)  return 100;
  return constrain(
    ((emptyLevel - level) / (emptyLevel - fullLevel)) * 100,
    0, 100
  );
}

// =====================================================
unsigned long convertDateToEpoch(int d, int m, int y) {
  struct tm t = {0};
  t.tm_mday = d;
  t.tm_mon = m - 1;
  t.tm_year = y - 1900;
  return mktime(&t);
}

long getDaysLeft() {
  time_t now;
  time(&now);
  return (MED_EXPIRY_EPOCH - now) / 86400;
}

// ===================== RGB =====================
void rgbRed()    { analogWrite(RED_PIN,255); analogWrite(GREEN_PIN,0);   analogWrite(BLUE_PIN,0); }
void rgbYellow() { analogWrite(RED_PIN,100); analogWrite(GREEN_PIN,150); analogWrite(BLUE_PIN,0); }
void rgbGreen()  { analogWrite(RED_PIN,0);   analogWrite(GREEN_PIN,255); analogWrite(BLUE_PIN,0); }
void rgbBlue()   { analogWrite(RED_PIN,0);   analogWrite(GREEN_PIN,0);   analogWrite(BLUE_PIN,255); }

// ===================== Buzzer =====================
void buzz()   { digitalWrite(BUZZER_PIN, HIGH); }
void noBuzz() { digitalWrite(BUZZER_PIN, LOW); }

// ===================== ThingSpeak =====================
void sendToThingSpeak(int medActive, float fillPercent, long daysLeft) {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  String url = "http://api.thingspeak.com/update?api_key=" + thingspeakApiKey;
  url += "&field1=" + String(medActive);
  url += "&field2=" + String(daysLeft);
  url += "&field3=" + String(fillPercent);

  http.begin(url);
  http.GET();
  http.end();
}
