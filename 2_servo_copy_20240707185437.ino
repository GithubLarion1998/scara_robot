#include <Servo.h>

Servo servo0;
Servo servo1;

struct ServoCmd {
  int pos = 0;
  int target = 0;
};

ServoCmd cmd_servo0;
ServoCmd cmd_servo1;

constexpr unsigned int SERVO_PIN_1 = 10;
constexpr unsigned int SERVO_PIN_2 = 9;

void setup() {
  servo0.attach(SERVO_PIN_1);
  servo1.attach(SERVO_PIN_2);
  Serial.begin(9600);
}

int cmd_threshold(int target, int pos) {
  if (pos > target) {
    return -1;
  }
  else if (pos < target) {
    return 1;
  }
  return 0;
}

void update_servo(ServoCmd* cmd, Servo* servo) {
  int dx = cmd_threshold(cmd->target, cmd->pos);
  if (dx != 0 && cmd->pos <= 180 && cmd->pos >= 0){
    cmd->pos += dx;    
    servo->write(cmd->pos);
  }

}

void loop() {
  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    unsigned int pos = msg.indexOf(';');
    cmd_servo0.target = atoi(msg.substring(0, pos).c_str());
    cmd_servo1.target = atoi(msg.substring(pos + 1, msg.length()).c_str());    
    Serial.println(msg);
    Serial.print(cmd_servo0.target);
    Serial.print(" ");    
    Serial.println(cmd_servo1.target);
  }
  update_servo(&cmd_servo0, &servo0);
  update_servo(&cmd_servo1, &servo1);
}
