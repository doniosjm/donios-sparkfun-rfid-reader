/* Name       : donios-sparkfun-rfid-reader
   Author     : Jeff Donios (jeff@donios.com)
   Description: The sketch for reading RFID from the Sparkfun RFID USB Reader Board and ID-20LA Reader.
                Based on https://learn.sparkfun.com/tutorials/sparkfun-rfid-starter-kit-hookup-guide?_ga=1.106804739.1470785767.1489209126
                and modified to run on Arduino Leonardo
   Repository : https://github.com/doniosjm/donios-sparkfun-rfid-reader
  
   Pinout     : Arduino ----- RFID module
                Leonardo
              
                5V            VCC
                GND           GND
                D0            TX
*/

const int TAG_LEN = 16; // total tag length
const int ID_LEN  = 13; // tag we want to keep
char buff[ID_LEN]; // buffer for serial reads

void setup() {
  // initialise & set baud rate
  Serial.begin(9600);
  Serial1.begin(9600);

  while (!Serial) ;
  while (!Serial1) ;

  Serial.println("RFID Reads");
}

void loop() {
  int i = 0; // byte cound
  int readByte; // hold each byte read from the serial buffer
  bool readingData = false; // reading data off the wire

  // This makes sure the whole tag is in the serial buffer before reading
  // The Arduino can read faster than the ID module can deliver!
  if (Serial1.available() == TAG_LEN) {
    readingData = true;
  }

  if (readingData == true) {
    while (Serial1.available()) {
      readByte = Serial1.read();

      /* This will skip the first byte (2, STX, start of text) and the last three,
        ASCII 13, CR/carriage return, ASCII 10, LF/linefeed, and ASCII 3, ETX/end of
        text, leaving only the unique part of the tag string. It puts the byte into
        the first space in the array, then steps ahead one spot */
      if (readByte != 2 && readByte != 13 && readByte != 10 && readByte != 3) {
        buff[i] = readByte;
        i++;
      }

      // If we see ASCII 3, ETX, the tag is over
      if (readByte == 3) {
        readingData = false;
      }
    }
  }

  // don't do anything if buffer is empty
  if (strlen(buff) == 0) {
    return;
  }
  else {
    Serial.println(buff);
    // clear out buffer for a new read
    for (int c = 0; c < ID_LEN; c++) {
      buff[c] = 0;
    }
  }
}

