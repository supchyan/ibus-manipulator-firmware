#include <IBusBM.h>

#define SPEED_1      2    // linear actuator pins
#define DIR_1        A2   // -- // --

#define SPEED_2      3    // -- // --
#define DIR_2        9    // -- // --

#define SPEED_3      11   // -- // --
#define DIR_3        13   // -- // --

#define SPEED_4      A3   // -- // --
#define DIR_4        5    // -- // --

#define SPEED_5      6    // gear motor pins
#define DIR_5        10   // -- // --

IBusBM bus {};

int R_stick_X;  // ch 1 R Stick - Left/Right (all values 1000->2000)
int R_stick_Y;  // ch 2 R Stick - Up/Down
int L_stick_X;  // ch 4 L Stick - Right/Left
int L_stick_Y;  // ch 3 L Stick - Up/Down
int SWC;        // ch 5 SWC switch (THIS ONE IS THE CUSTOM REDEFINITION IN CONTROLLER SETTINGS)

// mapped values of default receiver readings
float R_stick_X_map = 0;
float R_stick_Y_map = 0;

float L_stick_X_map = 0;
float L_stick_Y_map = 0;

/**
* Converts iBus input from [1000 -> 2000] to [-1 -> 1].
*/
float ibus_map(int val) {
  return 2 * (float(val - 1000) / 1000.0 - .5);
}

void setup() {
  pinMode(SPEED_1, OUTPUT);
  pinMode(DIR_1, OUTPUT);

  pinMode(SPEED_2, OUTPUT);
  pinMode(DIR_2, OUTPUT);

  pinMode(SPEED_3, OUTPUT);
  pinMode(DIR_3, OUTPUT);

  pinMode(SPEED_4, OUTPUT);
  pinMode(DIR_4, OUTPUT);

  pinMode(SPEED_5, OUTPUT);
  pinMode(DIR_5, OUTPUT);
  
  Serial.begin(9600);

  while(!Serial) {};
  
  // Uses RX0 pin, so disconnect the pin
  // before pushing firmware to a controller
  bus.begin(Serial);
}

void loop() {
  // get values on radio channels
  R_stick_X = bus.readChannel(0); // 0 here is the 1 channel on controller lol
  R_stick_Y = bus.readChannel(1);
  
  L_stick_Y = bus.readChannel(2);
  L_stick_X = bus.readChannel(3);

  SWC = bus.readChannel(4);

  // map values
  R_stick_X_map = ibus_map(R_stick_X);
  R_stick_Y_map = ibus_map(R_stick_Y);

  L_stick_Y_map = ibus_map(L_stick_Y);
  L_stick_X_map = ibus_map(L_stick_X);

  // -------------------------------------------------
  // EXPRECTED BEHAVIOUR
  // When swc switch is in `UP` position:
  // Treat both sticks inputs as hand move commands.
  //
  // When SWC switch is in `DOWN` position:
  // Treat Right Stick X input as a rotation command for a hand.
  // -------------------------------------------------

  // modify motors direction 
  // depend on mapped stick values
  if (R_stick_X_map > .1) {
    digitalWrite(DIR_1, HIGH); // hand component direction
    digitalWrite(DIR_5, HIGH); // rotation direction
  }
  if (R_stick_X_map < -.1) {
    digitalWrite(DIR_1, LOW); // hand component direction
    digitalWrite(DIR_5, LOW); // rotation direction
  }

  if (R_stick_Y_map > .1) {
    digitalWrite(DIR_2, HIGH); // hand component direction
  }
  if (R_stick_Y_map < -.1) {
    digitalWrite(DIR_2, LOW); // hand component direction
  }

  if (L_stick_X_map > .1) {
    digitalWrite(DIR_3, HIGH); // hand component direction
  }
  if (L_stick_X_map < -.1) {
    digitalWrite(DIR_3, LOW); // hand component direction
  }

  if (L_stick_Y_map > .1) {
    digitalWrite(DIR_4, HIGH); // hand component direction
  }
  if (L_stick_Y_map < -.1) {
    digitalWrite(DIR_4, LOW); // hand component direction
  }

  if (SWC > 800) {
    if (SWC < 1800) { // SWC `UP` position
      analogWrite(SPEED_1, 0); // modify hand component speed
      analogWrite(SPEED_2, 0); // modify hand component speed

      analogWrite(SPEED_3, 0); // modify hand component speed
      analogWrite(SPEED_4, 0); // modify hand component speed

      analogWrite(SPEED_5, abs(round(255 * R_stick_X_map))); // modify rotation speed
    }
    else { // SWC `DOWN` position
      analogWrite(SPEED_1, abs(255 * round(R_stick_X_map))); // modify hand component speed
      analogWrite(SPEED_2, abs(255 * round(R_stick_Y_map))); // modify hand component speed

      analogWrite(SPEED_3, abs(255 * round(L_stick_X_map))); // modify hand component speed
      analogWrite(SPEED_4, abs(255 * round(L_stick_Y_map))); // modify hand component speed
      
      analogWrite(SPEED_5, 0); // modify rotation speed
    }
  }
}
