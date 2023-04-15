//Motor A
const int inv = 7; //Input from inverter
const int Vinh  = 10;  // Pin  1 of L293
//Motor B
const int Vinh2  = 9;  // Pin  9 of L293
const int inv2  = 3;  // Input from inverter

//Infrarot sensor
int Analogwert;
int Distanz;

//line sensor
int QRE1113_Pin = 1;

//This will run only one time.
void setup(){
  pinMode(A0, INPUT)
    Serial.begin(9600);
    //Set pins as outputs
    pinMode(inv, OUTPUT);
    pinMode(Vinh, OUTPUT);
  
    pinMode(inv2, OUTPUT);
    pinMode(Vinh2, OUTPUT);        
}


void loop(){
  //infrarot
  Analogwert = analogRead(A0);
  Distanz=pow((3027.4/Analogwert), 1.2134);
    Serial.println(Distanz);
    delay(100);
  
  //line sensor
  int QRE_Value = analogRead(QRE1113_Pin);
  Serial.println(QRE_Value); 
  
//This code  will turn Motor A clockwise for 2 sec.
    digitalWrite(inv, LOW);
    digitalWrite(Vinh, HIGH);
  
    digitalWrite(inv2, HIGH);
    digitalWrite(Vinh2, HIGH);
    delay(2000);
  
  	digitalWrite(Vinh, HIGH);
  	digitalWrite(inv, HIGH);

  	digitalWrite(Vinh2, HIGH);
  	digitalWrite(inv2, LOW);
  	delay(2000);
}
