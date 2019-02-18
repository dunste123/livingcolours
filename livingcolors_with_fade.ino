//Include ir thing
#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

//defne the leds to the pins
int blueLed = 11;
int greenLed = 10;
int redLed = 9;
//Set the ir pin
int irPin = 8;
IRrecv irrecv(irPin);           // create instance of 'irrecv'
decode_results results;            // create instance of 'decode_results'
//We store the states of the leds because we need them
int ledValues[] = {0, 0, 0};
//We use the led step to keep track of where we are
int ledStep = -1;
int shouldFade = 0;
//stuff
int btnPin = 8;
int state = 0;
int val1;
int val2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

  //  pinMode(btnPin, INPUT);
  //  state = digitalRead(btnPin);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (shouldFade == 1) {
    ledStep++;
    setLeds();
    Serial.println((String) "Changing led step to " + ledStep);
    for (int i = 0; i < 20; i++) {
      checkBtn();
      delay(10);
    }
    //delay(2000);
  } else {
    shouldFade = 0;
  }
  checkBtn();
}

//This function changes the leds based on the values that you put in
void rgb(int red, int green, int blue) {
  ledValues[0] = red;
  ledValues[1] = green;
  ledValues[2] = blue;
  analogWrite(redLed, red);
  analogWrite(greenLed, green);
  analogWrite(blueLed, blue);
}
//a nice function to make a led fade
void rgbFade(int red, int green, int blue) {
  //  rgb(red, green, blue);
  fade(redLed, ledValues[0], red);
  fade(greenLed, ledValues[1], green);
  fade(blueLed, ledValues[2], blue);
  ledValues[0] = red;
  ledValues[1] = green;
  ledValues[2] = blue;
}
// some colors
void colorYellow() {
  rgb(255, 100, 0);
}
void colorCyan() {
  rgb(0, 100, 255);
}
void colorMagenta() {
  rgb(255, 0, 255);
}
//fade function that chooses if the led needs to fade up or down
void fade(int ledPin, int start, int level) {
  if (start < level) {
    fadeUp(ledPin, start, level);
  } else if (start > level) {
    fadeDown(ledPin, start, level);
  }
}
// function to make the led fade up
void fadeUp(int ledPin, int start, int level) {
  //check if no levels are above 255 or below 0
  if (level > 255 || level < 0) {
    return;
  }
  if (start > 255 || start < 0) {
    return;
  }
  //increment the value of the led on the pin
  for (int fadeValue = start ; fadeValue <= level; fadeValue += 5) {
    //check if we are above 255 (max level)
    if (fadeValue <= 255 || fadeValue < 0) {
      // sets the value (range from 0 to 255):
      analogWrite(ledPin, fadeValue);
      checkBtn();
      // wait for 30 milliseconds to see the dimming effect
      delay(30);
    }
  }
}
//function to make the led fade down
void fadeDown(int ledPin, int start, int level) {
  //check if no levels are above 255 or below 0
  if (level > 255 || level < 0) {
    return;
  }
  if (start > 255 || start < 0) {
    return;
  }
  //increment the value of the led on the pin
  for (int fadeValue = start ; fadeValue >= level; fadeValue -= 5) {
    //check if we are above 255 (max level)
    if (fadeValue <= 255 || fadeValue < 0) {
      // sets the value (range from 0 to 255):
      analogWrite(ledPin, fadeValue);
      checkBtn();
      // wait for 30 milliseconds to see the dimming effect
      delay(30);
    }
  }
}
//function to check if the button has been pressed
void checkBtn() {
  // have we received an IR signal?
  if (irrecv.decode(&results)) {
    //    Serial.println(results.value, HEX);  UN Comment to see raw values

    switch (results.value) {
      case 0xFF22DD:
        Serial.println(" PREV           ");
        shouldFade = 0;
        ledStep--;
        setLeds();
        break;

      case 0xFF02FD:
        Serial.println(" NEXT           ");
        shouldFade = 0;
        ledStep++;
        setLeds();
        break;

      case 0xFFC23D:
        Serial.println(" PLAY/PAUSE     ");
        shouldFade++;
        if (shouldFade > 1 ) {
          shouldFade = 0;
        }
        Serial.println((String )"btn pressed, shouldFade: " + shouldFade);
        break;
    }

    irrecv.resume(); // receive the next value
  }
  delay(500);
}
void setLeds() {
  Serial.println((String )"Led step: " + ledStep);
  switch (ledStep) {
    case 0:
      rgb(255, 0, 0);
      break;
    case 1:
      rgb(255, 0, 255);
      break;
    case 2:
      rgb(0, 0, 255);
      break;
    case 3:
      rgb(0, 60, 255);
      break;
    case 4:
      rgb(0, 100, 0);
      break;
    case 5:
      rgb(255, 100, 0);
      break;
    default:
      ledStep = -1;
      break;
  }
}
//void checkBtn() {
//  val1 = digitalRead( btnPin );
//  delay(10);
//  val2 = digitalRead( btnPin );
//  if (val1 == val2) {
//    if (val1 != state) {
//      if (val1 == HIGH) {
//        shouldFade++;
//        if (shouldFade > 1 ) {
//          shouldFade = 0;
//        }
//        Serial.println((String )"btn pressed, shouldFade: " + shouldFade);
//      }
//    }
//    state = val1;
//  }
//}
