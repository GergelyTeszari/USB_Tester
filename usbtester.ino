const int numMultiplexerPins = 4;  // A1-A4
const int numDigitalOutputPins = 13;  // 2-A0
const int analogInputPin = A5;
int progress = 169;
int lines = 0;

void setup() {
  // Set the digital pins as outputs
  for (int i = 0; i < numDigitalOutputPins; i++) {
    pinMode(i + 2, OUTPUT);
    digitalWrite(i + 2, LOW);
  }
  
  // Set the multiplexer pins as outputs
  for (int i = 0; i < numMultiplexerPins; i++) {
    pinMode(A1 + i, OUTPUT);
    digitalWrite(A1 + i, LOW);
  }

  // Set the analog input pin as an input
  pinMode(analogInputPin, INPUT);

  // Start serial communication
  Serial.begin(9600);
}

void loop() {
  float readings[numDigitalOutputPins][numDigitalOutputPins];
  
  for (int currentDigitalOutput = 0; currentDigitalOutput < numDigitalOutputPins; currentDigitalOutput++) {
    digitalWrite(currentDigitalOutput + 2, HIGH);
    for (int binaryNumber = 0; binaryNumber < numDigitalOutputPins; binaryNumber++) {
      // Set the multiplexer pins to the binary number
      for (int i = 0; i < 4; i++) {
            digitalWrite(A1 + i, (binaryNumber & (1 << i)) >> i);
      }
      
      // Read the analog input pin and store the value if the pin is not the one that's currently high
      if (currentDigitalOutput != binaryNumber){
        readings[currentDigitalOutput][binaryNumber] = analogRead(analogInputPin);
      } else {
        readings[currentDigitalOutput][binaryNumber] = 1.0;
      }
      progress--;
      if ((((progress/169)*100)/5)>lines){
        lines++;
        Serial.print("=");
      }
    }

    digitalWrite(currentDigitalOutput + 2, LOW);
    
    // Clearing multiplexer
    for (int i = 0; i < 4; i++) {
          digitalWrite(A1 + i, LOW);
    }
  }
  Serial.println();

  

  // Print the matrix of analog readings to the serial monitor
  Serial.println("Completed analisis, printing...");
  for (int i = 0; i < numDigitalOutputPins; i++) {
    Serial.print(i);
    Serial.print(":\t");
    for (int j = 0; j < numDigitalOutputPins; j++) {
      Serial.print(readings[i][j]);
      Serial.print("\t|\t");
    }
    Serial.println();
  }
  
  String result = (isSymmetric(readings) == 0) ? "Warning! USB port seems corrupted!" : "USB port's connections are symmetric.";
  Serial.println(result);
  endlessLoop();
}

bool isSymmetric(float matrix[13][13]) {
    for (int i = 0; i < 13; i++) {
        for (int j = i+1; j < 13; j++) {
            if ((int)matrix[i][j] != (int)matrix[j][i]) {
                return false;
            }
        }
    }
    return true;
}


void endlessLoop()
{
  while(true) {
    delay(1000);
  }
}
