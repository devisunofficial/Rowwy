#include <esp_now.h>
#include <WiFi.h>

#define FL_IN1 2
#define FL_IN2 4
#define FL_PWM 5

#define FR_IN1 12
#define FR_IN2 13
#define FR_PWM 14

// Driver 2: Rear Motors
#define RL_IN1 16
#define RL_IN2 17
#define RL_PWM 18

#define RR_IN1 19
#define RR_IN2 21
#define RR_PWM 22

#define MOTOR_STBY 23

typedef struct struct_message {
    int forward;  // -255 to 255 (Pitch/Tilt from MPU6050)
    int strafe;   // -255 to 255 (Roll/Tilt from MPU6050)
    int turn;     // -255 to 255 (Yaw or dedicated steering input)
    int brake;    // 0 to 1023 (Analog value from linear potentiometer)
} struct_message;

struct_message incomingData;

const int BRAKE_THRESHOLD = 400; 

void moveMotor(int in1_pin, int in2_pin, int pwm_pin, int speed) {
    if (speed > 0) {
        digitalWrite(in1_pin, HIGH);
        digitalWrite(in2_pin, LOW);
        analogWrite(pwm_pin, speed);
    } else if (speed < 0) {
        digitalWrite(in1_pin, LOW);
        digitalWrite(in2_pin, HIGH);
        analogWrite(pwm_pin, abs(speed));
    } else {
        digitalWrite(in1_pin, LOW);
        digitalWrite(in2_pin, LOW);
        analogWrite(pwm_pin, 0);
    }
}

void applyHardBrake() {
    digitalWrite(FL_IN1, HIGH); digitalWrite(FL_IN2, HIGH); analogWrite(FL_PWM, 255);
    digitalWrite(FR_IN1, HIGH); digitalWrite(FR_IN2, HIGH); analogWrite(FR_PWM, 255);
    digitalWrite(RL_IN1, HIGH); digitalWrite(RL_IN2, HIGH); analogWrite(RL_PWM, 255);
    digitalWrite(RR_IN1, HIGH); digitalWrite(RR_IN2, HIGH); analogWrite(RR_PWM, 255);
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingDataBuffer, int len) {
    memcpy(&incomingData, incomingDataBuffer, sizeof(incomingData));
    
    if (incomingData.brake > BRAKE_THRESHOLD) {
        applyHardBrake();
        return; 
    }

    int fl_speed = incomingData.forward + incomingData.strafe + incomingData.turn;
    int fr_speed = incomingData.forward - incomingData.strafe - incomingData.turn;
    int rl_speed = incomingData.forward - incomingData.strafe + incomingData.turn;
    int rr_speed = incomingData.forward + incomingData.strafe - incomingData.turn;

    fl_speed = constrain(fl_speed, -255, 255);
    fr_speed = constrain(fr_speed, -255, 255);
    rl_speed = constrain(rl_speed, -255, 255);
    rr_speed = constrain(rr_speed, -255, 255);

    moveMotor(FL_IN1, FL_IN2, FL_PWM, fl_speed);
    moveMotor(FR_IN1, FR_IN2, FR_PWM, fr_speed);
    moveMotor(RL_IN1, RL_IN2, RL_PWM, rl_speed);
    moveMotor(RR_IN1, RR_IN2, RR_PWM, rr_speed);
}

void setup() {
    Serial.begin(115200);

    pinMode(FL_IN1, OUTPUT); pinMode(FL_IN2, OUTPUT); pinMode(FL_PWM, OUTPUT);
    pinMode(FR_IN1, OUTPUT); pinMode(FR_IN2, OUTPUT); pinMode(FR_PWM, OUTPUT);
    pinMode(RL_IN1, OUTPUT); pinMode(RL_IN2, OUTPUT); pinMode(RL_PWM, OUTPUT);
    pinMode(RR_IN1, OUTPUT); pinMode(RR_IN2, OUTPUT); pinMode(RR_PWM, OUTPUT);
    pinMode(MOTOR_STBY, OUTPUT);

    digitalWrite(MOTOR_STBY, HIGH);

    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
    
    Serial.println("Receiver Ready. Waiting for ESP-NOW data...");
}

void loop() {
}