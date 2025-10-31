#include "esphome/core/log.h"
#include "serialservo.h"

namespace esphome {
namespace serialservo {

static const char *const TAG = "serialservo";

//Create a new serial driver object
SCSCL sc;

void SerialServo::setup() {
  ESP_LOGI(TAG, "SerialServo initialized.");
  Serial1.begin(1000000, SERIAL_8N1, rx_, tx_);
  sc.pSerial = &Serial1;
}

void SerialServo::dump_config() {
  ESP_LOGCONFIG(TAG, "Serial Servo component");
}

void SerialServo::loop() {

}

int SerialServo::ping(int servo) {
  int res = sc.Ping(servo);
  if (res) {
    ESP_LOGI(TAG, "Sucessfully pinged servo %d", servo);
    return 1;
  } else {
    ESP_LOGI(TAG, "Unable to ping servo %d", servo);
    return 0;
  }
}

int SerialServo::write(int servo, int position, int speed) {
  int res = sc.WritePos(servo, position, 0, speed);
  ESP_LOGI(TAG, "Set servo %d to position %d at speed %d", servo,position,speed);
  return res;
}

int SerialServo::setservoid(int servo, int servoid, int newservoid) {
  int res = sc.unLockEprom(servoid);
  sc.writeByte(servoid, SMS_STS_ID, newservoid);
  sms_sts.LockEprom(newservoid);
  ESP_LOGI(TAG, "Set servo %d to id %d", servo,newservoid);
  return res;
}

}  // namespace serialservo
}  // namespace esphome
