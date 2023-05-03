// Infrared sensor
int analogValue;
int distance;

// Motor A
const int inv = 7;    // Input from inverter
const int Vinh = 10;  // Pin 1 of L293
int motorSpeedA = 0;  // Motor A speed

// Motor B
const int Vinh2 = 9;  // Pin 9 of L293
const int inv2 = 3;   // Input from inverter
int motorSpeedB = 0;  // Motor B speed

// Filter variables
const int numSamples = 5;   // Number of samples to use for filtering
int samples[numSamples];    // Array to store distance samples
int currentIndex = 0;       // Index of current sample

void setup() {
  pinMode(A0, INPUT);
  Serial.begin(9600);
  pinMode(inv, OUTPUT);
  pinMode(Vinh, OUTPUT);
  pinMode(inv2, OUTPUT);
  pinMode(Vinh2, OUTPUT);
}

void loop() {
  // Infrared sensor
  analogValue = analogRead(A0);
  distance = pow((3027.4 / analogValue), 1.2134);
  Serial.println(distance);
  
  // Update filter with new distance measurement
  samples[currentIndex] = distance;
  currentIndex = (currentIndex + 1) % numSamples;
  
  // Calculate filtered distance as the average of all samples
  int filteredDistance = 0;
  for (int i = 0; i < numSamples; i++) {
    filteredDistance += samples[i];
  }
  filteredDistance /= numSamples;
  
  Serial.print("Filtered Distance: ");
  Serial.println(filteredDistance);

  // Motor control based on filtered distance
  if (filteredDistance < 8) {
    // If filtered distance is less than 8, stop Motor A and Motor B
    motorSpeedA = 0;
    motorSpeedB = 0;
    
    digitalWrite(inv, LOW);
    analogWrite(Vinh, motorSpeedA);
    digitalWrite(inv2, LOW);
    analogWrite(Vinh2, motorSpeedB);
  } else {
    // Otherwise, adjust motor speed proportionally based on filtered distance
    motorSpeedA = map(filteredDistance, 0, 25, 0, 255);  // Map distance to motor speed range
    motorSpeedB = motorSpeedA;
    
    digitalWrite(inv, HIGH);
    analogWrite(Vinh, motorSpeedA);
    digitalWrite(inv2, HIGH);
    analogWrite(Vinh2, motorSpeedB);
  }

  delay(100);  // Adjust delay time as needed
}
