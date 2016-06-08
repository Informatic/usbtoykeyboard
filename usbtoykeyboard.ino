const int ROWS_COUNT = 4;
const int COLS_COUNT = 8;
const int ANALOG_COUNT = 2;

// Internal reserved CC ID for pitchbend (which isn't really a CC, but, welp)
const int CC_PITCHBEND = 128;

const int midiChannel = 1;
const int midiVelocity = 127;
const int midiBaseNote = 52;

const int rowsPins[ROWS_COUNT] = {
  10, 16, 14, 15
};

const int colsPins[COLS_COUNT] = {
  2, 3, 4, 5, 6, 7, 8, 9
};

// State variables
int currentRow = 0;

int state[ROWS_COUNT] = {  // FIXME: we assume COLS_COUNT is 8
  0, 0, 0, 0
};

int16_t value = 0, last = 0;

// @TODO
//const int analogPins[ANALOG_COUNT] = {
//  A0, A1
//};

//const int analogCC[ANALOG_COUNT] = {
//  CC_PITCHBEND, 1
//};

// Analog calibration data
//int analogMax[ANALOG_COUNT] = {830, 890};
//int analogMin[ANALOG_COUNT] = {180, 240};

//int analogState[ANALOG_COUNT] = {
//  0, 0
//};

// @TODO
// #include <ClickEncoder.h>
// #include <TimerOne.h>

//ClickEncoder encoder(A1, A0, A2);

void setup() {
  Serial.begin(115200);
  Serial1.begin(31250);
  
  for(int p = 0; p < ROWS_COUNT; p++) {
    pinMode(rowsPins[p], INPUT);
    digitalWrite(rowsPins[p], HIGH);
  }
  for(int p = 0; p < COLS_COUNT; p++) {
    pinMode(colsPins[p], INPUT_PULLUP);
  }

  // @TODO
  //Timer1.initialize(1000);
  //Timer1.attachInterrupt([]() {
  //  encoder.service();
  //});
}

void loop() {
  pinMode(rowsPins[currentRow], OUTPUT);

  // Scan keys
  for(int p = 0; p < COLS_COUNT; p++) {
    bool newState = digitalRead(colsPins[p]) == LOW;

    if(((state[currentRow] >> p) & 1) != newState) {
      int midiNote = currentRow*COLS_COUNT + (8-p) + midiBaseNote;
      if(newState) {
        MIDIEvent midiEvent = {0x09, 0x90 + midiChannel, midiNote, midiVelocity};
        midiOutput(midiEvent);
        
        Serial.print("u ");
        Serial.println(midiNote);
      } else {
        MIDIEvent midiEvent = {0x08, 0x80 + midiChannel, midiNote, midiVelocity};
        midiOutput(midiEvent);
        
        Serial.print("d ");
        Serial.println(midiNote);
      }
      
      state[currentRow] ^= 1 << p;
    }
  }
  
  // FIXME: Is it really needed? pinMode(..., OUTPUT); pulls it low anyway.
  digitalWrite(rowsPins[currentRow], HIGH);
  pinMode(rowsPins[currentRow], INPUT);
  currentRow = (currentRow+1) % ROWS_COUNT;

  // @TODO
  /*
  value += encoder.getValue();
  value = constrain(value, 0, 255);
  if (value != last) {
    last = value;
    Serial.print("Encoder Value: ");
    Serial.println(value);
  }
  
  ClickEncoder::Button b = encoder.getButton();
  if (b != ClickEncoder::Open) {
    Serial.print("Button: ");
    Serial.println(b);
  }
  */
  // Update analog controls every ROWS_COUNT interations. (FIXME: shall we use for(...) instead?)
  // @TODO
  /*
  if(currentRow == 0) {
    for(int p = 0; p < ANALOG_COUNT; p++) {
      int reading = analogRead(analogPins[p]);
      if(reading != analogState[p]) {
        if(analogCC[p] == CC_PITCHBEND) {
          int transformed = map(reading, analogMin[p], analogMax[p], 0, 0x4000);
          MIDIEvent midiEvent = {0x0e, 0xe0 + midiChannel, transformed & 0x7f, (transformed >> 7) & 0x7f};
          MIDIUSB.write(midiEvent);
          Serial.write('p');
        } else {
          int transformed = map(reading, analogMin[p], analogMax[p], 0, 127);
          MIDIEvent midiEvent = {0x0b, 0xb0 + midiChannel, analogCC[p], transformed};
          MIDIUSB.write(midiEvent);
          Serial.write('c');
        }
        
        analogState[p] = reading;
      }
    }
  }
  */
}

void midiOutput(MIDIEvent& evt)
{
  Serial1.write(evt.m1);
  Serial1.write(evt.m2);
  Serial1.write(evt.m3);

  MIDIUSB.write(midiEvent);
  MIDIUSB.flush();
}
