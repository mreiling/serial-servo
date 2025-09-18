#pragma once

#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include <esphome/core/hal.h>
#include "SCServo.h"

namespace esphome {
namespace serialservo {


class SerialServo : public Component {
 public:
  void setup() override;
  void dump_config() override;
  void loop() override;

  // void set_rx(int rx) { this->rx_ = rx;}
  // void set_tx(int tx) { this->tx_ = tx;}
  int write(int servo, int position, int speed);
  int ping(int servo);

protected:
  int rx_{18};
  int tx_{19};
};

template<typename... Ts> class ServoWriteAction : public Action<Ts...> {
 public:
  ServoWriteAction(SerialServo *servo) : servo_(servo) {}
  TEMPLATABLE_VALUE(int, servoid)
  TEMPLATABLE_VALUE(int, position)
  TEMPLATABLE_VALUE(int, speed)
  void play(Ts... x) override { this->servo_->write(this->servoid_.value(x...),this->position_.value(x...),this->speed_.value(x...)); }

 protected:
  SerialServo *servo_;
};

}  // namespace serialservo
}  // namespace esphome
