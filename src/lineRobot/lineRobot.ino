//Motor A
const int inv = 7;    //Input from inverter
const int Vinh = 10;  // Pin  1 of L293

//Motor B
const int Vinh2 = 9;  // Pin  9 of L293
const int inv2 = 3;   // Input from inverter

//Infrarot sensor
int Analogwert;
int Distanz;

//line sensor
int QRE1113_Pin1 = 1;
int QRE1113_Pin2 = 2;
int QRE1113_Pin3 = 3;

//This will run only one time.
void setup() {
  pinMode(inv, OUTPUT);
  pinMode(Vinh, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);
  //Set pins as outputs
  pinMode(inv, OUTPUT);
  pinMode(Vinh, OUTPUT);

  pinMode(inv2, OUTPUT);
  pinMode(Vinh2, OUTPUT);
}


void loop() {
  //infrarot
  Analogwert = analogRead(A0);
  Distanz = pow((3027.4 / Analogwert), 1.2134);
  Serial.println("Distanz:");
  Serial.println(Distanz);
  delay(100);

  //line sensor
  int QRE_Value1 = analogRead(QRE1113_Pin1);
  Serial.println("line detector1:");
  Serial.println(QRE_Value1);
  int QRE_Value2 = analogRead(QRE1113_Pin2);
  Serial.println("line detector2:");
  Serial.println(QRE_Value2);
  int QRE_Value3 = analogRead(QRE1113_Pin3);
  Serial.println("line detector3:");
  Serial.println(QRE_Value3);

  //This code  will turn Motor A clockwise for 2 sec.
  //Pause
  //digitalWrite(inv, HIGH);//ruchtung "vorwärts"
  // digitalWrite(Vinh, LOW);//motor aus

  //digitalWrite(inv2, HIGH);//ruchtung "vorwärts"
  //digitalWrite(Vinh2, LOW);//motor aus
  //delay(2000);

  //digitalWrite(inv, LOW);//richtung rückwärts
  //digitalWrite(Vinh, LOW);//motor aus

  //digitalWrite(inv2, LOW);//richtung rückwärts
  //digitalWrite(Vinh2, LOW);//motor aus
  //delay(2000);
  digitalWrite(inv, LOW);    //richtung rückwärts
  digitalWrite(Vinh, HIGH);  //motor an

  digitalWrite(inv2, LOW);    //richtung rückwärts
  digitalWrite(Vinh2, HIGH);  //motor an
  delay(2000);

  //lets go

  digitalWrite(inv, HIGH);   //richtung vorwärts
  digitalWrite(Vinh, HIGH);  //motor an

  digitalWrite(inv2, HIGH);   //richtung vorwärts
  digitalWrite(Vinh2, HIGH);  //motor an
  delay(2000);
  //pause
}
