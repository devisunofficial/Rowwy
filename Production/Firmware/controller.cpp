#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define POT_PIN A0        
#define TOUCH_PIN D1      
uint8_t receiverMacAddress[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

typedef struct struct_message {
    int forward;  
    int strafe;  
    int turn;    
    int brake;  
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;
Adafruit_MPU6050 mpu;

int sensitivityLevel = 1;
bool lastTouchState = LOW;
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_DELAY = 250;

const int DEADZONE_BUFFER = 30; 

void setup() {
    Serial.begin(115200);

    pinMode(TOUCH_PIN, INPUT);
    analogReadResolution(12);

    Wire.begin();     if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) { delay(10); }
    }
    
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    memcpy(peerInfo.peer_addr, receiverMacAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
    
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }
}

void loop() {
    bool currentTouchState = digitalRead(TOUCH_PIN);
    if (currentTouchState == HIGH && lastTouchState == LOW && (millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
        sensitivityLevel = (sensitivityLevel + 1) % 3;
        lastDebounceTime = millis();
        Serial.print("Sensitivity Changed to Level: ");
        Serial.println(sensitivityLevel);
    }
    lastTouchState = currentTouchState;

    float maxTiltAngle = 45.0; 
    if (sensitivityLevel == 1) maxTiltAngle = 30.0; 
    if (sensitivityLevel == 2) maxTiltAngle = 15.0; 

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    float pitch = atan2(-a.x, sqrt(a.y * a.y + a.z * a.z)) * 180.0 / M_PI;
    float roll  = atan2(a.y, a.z) * 180.0 / M_PI;

    int rawForward = map(constrain(pitch, -maxTiltAngle, maxTiltAngle), -maxTiltAngle, maxTiltAngle, -255, 255);
    int rawStrafe  = map(constrain(roll, -maxTiltAngle, maxTiltAngle), -maxTiltAngle, maxTiltAngle, -255, 255);
    
    int rawTurn = map(constrain(g.gyro.z, -2.0, 2.0), -2.0, 2.0, -255, 255);

    myData.forward = (abs(rawForward) < DEADZONE_BUFFER) ? 0 : rawForward;
    myData.strafe  = (abs(rawStrafe) < DEADZONE_BUFFER)  ? 0 : rawStrafe;
    myData.turn    = (abs(rawTurn) < DEADZONE_BUFFER)    ? 0 : rawTurn;

    int potVal = analogRead(POT_PIN);
    myData.brake = map(potVal, 0, 4095, 0, 1023);

    esp_err_t result = esp_now_send(receiverMacAddress, (uint8_t *) &myData, sizeof(myData));
    
    delay(20); 
}