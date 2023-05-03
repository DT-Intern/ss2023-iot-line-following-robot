// Infrared sensor
int analogValue;
int distance;

// Line sensor
const int QRE1113_Pin1 = 1;
const int QRE1113_Pin2 = 2;
const int QRE1113_Pin3 = 3;

void setup() {
  // Put your setup code here, to run once:
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop() {
  // Put your main code here, to run repeatedly:

  // Infrared sensor
  analogValue = analogRead(A0);
  distance = pow((3027.4 / analogValue), 1.2134);
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(1500);

  // Line sensor
  int QRE_Value1 = analogRead(QRE1113_Pin1);
  Serial.print("Line detector 1: ");
  Serial.println(QRE_Value1);
  int QRE_Value2 = analogRead(QRE1113_Pin2);
  Serial.print("Line detector 2: ");
  Serial.println(QRE_Value2);
  int QRE_Value3 = analogRead(QRE1113_Pin3);
  Serial.print("Line detector 3: ");
  Serial.println(QRE_Value3);
  delay(1500);
}
