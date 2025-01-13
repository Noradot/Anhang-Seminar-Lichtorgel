const int volumePin = A0; 
int volume;
const int strobePin = 7;
const int resetPin = 8;
const int outPin = A5;
int level[7];

// RGB-LED 1 Pins
const int redPin1 = 3;    
const int greenPin1 = 5;  
const int bluePin1 = 6;   

// RGB-LED 2 Pins
const int redPin2 = 9;    
const int greenPin2 = 10; 
const int bluePin2 = 11;  

void setup() {
  Serial.begin(9600);

  // Definiere Pin-Modi
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  pinMode(outPin, INPUT);

  // Definiere RGB-LED 1 Pins
  pinMode(redPin1, OUTPUT);
  pinMode(greenPin1, OUTPUT);
  pinMode(bluePin1, OUTPUT);

  // Definiere RGB-LED 2 Pins
  pinMode(redPin2, OUTPUT);
  pinMode(greenPin2, OUTPUT);
  pinMode(bluePin2, OUTPUT);

  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, LOW);
  delay(1);

  // Reset von dem MSGEQ7 laut dem Datenblatt
  digitalWrite(resetPin, HIGH);
  delay(1);
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
  delay(1);
}

void loop() {
  // Lautstärke:
  volume = abs(analogRead(volumePin) - 512);

if (volume < 165) { 
    analogWrite(redPin1, map(volume, 0, 164, 0, 255)); // Rot ansteigend
    analogWrite(greenPin1, 0); // Grün aus
    analogWrite(bluePin1, 0); // Blau aus
  } else if (volume < 341) { // Niedrige bis mittlere Lautstärke 
    analogWrite(redPin1, 0); // Rot aus
    analogWrite(greenPin1, map(volume, 171, 340, 0, 255)); // Grün ansteigend
    analogWrite(bluePin1, 0); // Blau aus
  } else { // Mittlere bis hohe Lautstärke 
    analogWrite(redPin1, 0); // Rot aus
    analogWrite(greenPin1, 0); // Grün aus
    analogWrite(bluePin1, map(volume, 341, 512, 0, 255)); // Blau ansteigend
  }

  // Durch jedes Frequenzband den selben Ablauf:
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);

  for (int i = 0; i < 7; i++) {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30);
    level[i] = analogRead(outPin);
    digitalWrite(strobePin, HIGH);
    delayMicroseconds(30);
  }

  // Ausgabe der Frequenzbandwerte
  for (int i = 0; i < 7; i++) {
    Serial.print(level[i]);
    Serial.print(" ");
  }
 Serial.println();

  // RGB-LED 2 basierend auf den Frequenzbändern steuern
  int bass = level[0];   // Bass aus dem ersten Frequenzband
  int mid = level[3];    // Mitteltöne aus dem vierten Frequenzband
  int hoch = level[6];   // Höhen aus dem letzten Frequenzband


  // RGB-LED steuern
  analogWrite(redPin2, map(bass, 5, 25, 0, 255));                     // Bass -> Rot
  analogWrite(greenPin2, map(mid, 200, 1023, 0, 255));                 // Mitteltöne -> Grün
  analogWrite(bluePin2, map(hoch, 400, 700, 0, 255));                    // Höhen -> Blau

  delay(10); 
}
