// outputs
#define CHARGE_ON_PIN 3
#define CHARGE_OFF_PIN 4
#define LOAD_ON_PIN 5
#define LOAD_OFF_PIN 6

// inputs
#define ENABLE_CHARGE_PIN 7
#define ENABLE_LOAD_PIN 8

class DualCoilLatchingRelay {
  private:
    byte pinOn;
    byte pinOff;
    bool stateOn;
  public:
    DualCoilLatchingRelay(byte pinOn, byte pinOff) {
      this->pinOn = pinOn;
      this->pinOff = pinOff;
      init();
    }
    void init() {
      pinMode(pinOn, OUTPUT);
      pinMode(pinOff, OUTPUT);
      digitalWrite(pinOn, LOW);
      digitalWrite(pinOff, LOW);

      bool stateOn = true;
      off();
    }
    
    void on() {
      if(!stateOn) {
        pulse(pinOn);
      }

      stateOn = true;
    }
    
    void off() {
      if(stateOn) {
        pulse(pinOff);
      }
      
      stateOn = false;
    }

    void pulse(byte pin) {
      Serial.print("pulse: ");
      Serial.println(pin);
      digitalWrite(pin, LOW);
      delay(10);
      digitalWrite(pin, HIGH);
      delay(100);
      digitalWrite(pin, LOW);
    }
};

class InputEnable {
  private:
    byte pin;
    byte state;
    byte lastReading;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 200;
  public:
    InputEnable(byte pin) {
      this->pin = pin;
      lastReading = HIGH;
      init();
    }
    void init() {
      pinMode(pin, INPUT_PULLUP);
      update();
    }
    void update() {
      byte newReading = digitalRead(pin);
      
      if (newReading != lastReading) {
        lastDebounceTime = millis();
      }
      if (millis() - lastDebounceTime > debounceDelay) {
        // Update the 'state' attribute only if debounce is checked
        state = newReading;
      }
      lastReading = newReading;
    }
    byte getState() {
      update();
      return state;
    }
    bool isEnabled() {
      return (getState() == LOW);
    }
};

DualCoilLatchingRelay chargeRelay(CHARGE_ON_PIN, CHARGE_OFF_PIN);
DualCoilLatchingRelay loadRelay(LOAD_ON_PIN, LOAD_OFF_PIN);

InputEnable enableCharge(ENABLE_CHARGE_PIN);
InputEnable enableLoad(ENABLE_LOAD_PIN);

void setup() {
    Serial.begin(9600);
  }

void loop() {
  if (enableCharge.isEnabled()) {
    chargeRelay.on();
  //  Serial.println("charge on");
  } else {
    chargeRelay.off();
  //  Serial.println("charge off");
  }
  
  if (enableLoad.isEnabled()) {
    loadRelay.on();
  //  Serial.println("load on");
  } else {
    loadRelay.off();
  //  Serial.println("load off");
  }
}
