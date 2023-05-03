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

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "WiFly.h"

#define SSID "AndroidAP6831"
#define KEY "alina2003"

SoftwareSerial uart(2, 3);
WiFly wifly(&uart);

//This will run only one time.
void setup() {
  //Conofigure all pins
  pinMode(inv, OUTPUT);
  pinMode(Vinh, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(inv, OUTPUT);
  pinMode(Vinh, OUTPUT);
  pinMode(inv2, OUTPUT);
  pinMode(Vinh2, OUTPUT);

  // Start all serial interfaces
  uart.begin(9600);
  Serial.begin(9600);
  delay(1000);

  configureWifly();
  Serial.print("here");
  connectAndVerifyNetwork();
}

/**
 * Method used for WiFly configuration.
 * This method starts by resetting the WiFi-Interface.
 * After that, the local port is set to 80 (HTTP default).
 * Furthermore, the default string is set to "0" (none) and new requests are buffered to "*OPEN*".
 */
void configureWifly() {
  wifly.reset();
  Serial.println("Starting");

  // Update the preferences
  wifly.sendCommand("set ip local 80\r");
  wifly.sendCommand("set comm remote 0\r");
  wifly.sendCommand("set comm open *OPEN*\r");
}

/**
 * Method that establishes the connection to the WiFi network and verifies the connection.
 * The SSID and KEY variables are used for the connection.
 * By default, WPA2 PSK (Pre-Shared-Key) is used for network authentication.
 * After the connection has been established the controller waits 5 seconds for IP-Address association.
 * After the 5 seconds, the command to retrieve the current IP is invoked and then printed to the Serial monitor.
 */
void connectAndVerifyNetwork() {
  // Connect to the configured network
  while (wifly.join(SSID, KEY, WIFLY_AUTH_WPA2_PSK) == false) {
    Serial.println("Unable to connect");
  }

  Serial.println("Connected");

  // Attempt to fetch the IP-Address
  delay(7000);
  wifly.sendCommand("get ip\r");

  // Print the result
  unsigned char c;
  while (wifly.receive((uint8_t *)&c, 1, 300) > 0) {
    Serial.print((char)c);
  }
}

/**
 * Method that is invoked continuously as long as the controller is running.
 */

void loop() {
  if (wifly.available()) {
    if (uart.find("*OPEN*")) {
      delay(1000);
      if (uart.find("action=")) {
        Serial.println("Action detected");
        int action = uart.read() - 48;
        Serial.println(action);
        if (action == 1) {
          // Action 1: Surface measurement
          Serial.println(F("Request type 1"));
          uart.print(F("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 2\nConnection: close\n\n"));
          uart.print("ok");
          prototype(true);
        }
      } else {
        // Default destination: dashboard
        Serial.println(F("Requested dashboard"));
        uart.print(F("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 1071\nConnection: close\n\n"));
       uart.print(F("<!DOCTYPE html><html><head><title>IoT Surface LDR</title><link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/semantic-ui/2.5.0/semantic.min.css\" integrity=\"sha512-KXol4x3sVoO+8ZsWPFI/r5KBVB/ssCGB5tsv2nVOKwLg33wTFP3fmnXa47FdSVIshVTgsYk/1734xSk9aFIa4A==\" crossorigin=\"anonymous\" referrerpolicy=\"no-referrer\" /><script src=\"https://cdn.jsdelivr.net/npm/chart.js@4.2.1/dist/chart.umd.min.js\"></script></head><body><div class=\"ui container\" style=\"margin: 1rem\"><h1>IoT Surface LDR Dashboard</h1><h3 style=\"margin-top: 0; margin-bottom: 2.5rem;\">By Maximilian Fluegel, Alina Simon and Jannes Bikker</h3><div style=\"display: flex\"><div id=\"measureOutput\" style=\"width: 100px; height: 100px; background-color: black; border-radius: 1rem;\"></div><h3 style=\"margin: 1rem\">Recent colors:</h3><div id=\"historyOutput\" style=\"display: flex\"></div></div><button id=\"measureButton\" class=\"ui fluid labeled icon button primary\" style=\"margin-top: 1rem\" onclick=\"measure()\"><i id=\"measureButtonIcon\" class=\"tint icon\"></i>Start Single Measurement</button><button id=\"continuousMeasureButton\" class=\"ui fluid labeled positive icon button primary\" style=\"margin-top: 0.5rem\" onclick=\"measureContinuously()\"><i id=\"continuousMeasureButtonIcon\" class=\"play icon\"></i>Measure Continuously</button><button id=\"calibrateWhiteButton\" class=\"ui fluid labeled icon button orange\" style=\"margin-top: 0.5rem\" onclick=\"calibrate('action=2', 'calibrateWhiteButton')\"><i id=\"calibrateWhiteButtonIcon\" class=\"sync alternate icon\"></i>Calibrate White</button><button id=\"calibrateBlackButton\" class=\"ui fluid labeled icon button orange\" style=\"margin-top: 0.5rem\" onclick=\"calibrate('action=3', 'calibrateBlackButton')\"><i id=\"calibrateBlackButtonIcon\" class=\"sync alternate icon\"></i>Calibrate Black</button><button class=\"ui fluid labeled icon button secondary\" style=\"margin-top: 0.5rem; margin-bottom: 1rem;\" onclick=\"clearMeasurements()\"><i class=\"trash icon\"></i>Clear Measurements</button><canvas id=\"historyChart\"></canvas></div><script>const createDataset = (name, color) => {return {label: name,data: [],borderColor: color,};};const context = document.getElementById(\"historyChart\");const measureButton = document.getElementById(\"measureButton\");const measureButtonIcon = document.getElementById(\"measureButtonIcon\");const calibrateWhiteButton = document.getElementById(\"calibrateWhiteButton\");const calibrateBlackButton = document.getElementById(\"calibrateBlackButton\");const continuousMeasureButton = document.getElementById(\"continuousMeasureButton\");const continuousMeasureButtonIcon = document.getElementById(\"continuousMeasureButtonIcon\");const recentContainer = document.getElementById(\"historyOutput\");let labels = [];let continuousEnabled = false;let redDataset = createDataset(\"Red\", \"rgb(255, 0, 0)\");let greenDataset = createDataset(\"Green\", \"rgb(0, 255, 0)\");let blueDataset = createDataset(\"Blue\", \"rgb(0, 0, 255)\");const chart = new Chart(context, {type: \"line\",data: {labels: labels,datasets: [redDataset,greenDataset,blueDataset,]},});const insertColors = (colors) => {const actualColors = colors.split(\", \");chart.data.datasets[0].data.push(actualColors[0]);chart.data.datasets[1].data.push(actualColors[1]);chart.data.datasets[2].data.push(actualColors[2]);chart.data.labels.push(\"I\");chart.update();if (recentContainer.children.length > 10) {recentContainer.removeChild(recentContainer.lastChild);}const recentItem = document.createElement(\"div\");recentItem.style.width = \"25px\";recentItem.style.height = \"25px\";recentItem.style.marginTop = \"0.9rem\";recentItem.style.marginRight = \"0.25rem\";recentItem.style.backgroundColor = `rgb(${colors})`;recentItem.style.borderRadius = \"0.5rem\";recentContainer.insertBefore(recentItem, recentContainer.firstChild);document.getElementById(\"measureOutput\").style.backgroundColor = `rgb(${colors})`;};const measure = () => {measureButton.disabled = true;measureButtonIcon.classList.remove(\"tint\");measureButtonIcon.classList.add(\"loading\");measureButtonIcon.classList.add(\"spinner\");fetch(\"http://192.168.35.31/action=1\").then(r => r.text()).then(colors => {measureButton.disabled = false;measureButtonIcon.classList.remove(\"loading\");measureButtonIcon.classList.remove(\"spinner\");measureButtonIcon.classList.add(\"tint\");insertColors(colors);});};const measureContinuously = () => {if (continuousEnabled) {measureButton.disabled = false;calibrateWhiteButton.disabled = false;calibrateBlackButton.disabled = false;continuousEnabled = false;continuousMeasureButton.classList.remove(\"negative\");continuousMeasureButton.classList.add(\"positive\");continuousMeasureButtonIcon.classList.remove(\"loading\");continuousMeasureButtonIcon.classList.remove(\"spinner\");continuousMeasureButtonIcon.classList.add(\"play\");} else {measureButton.disabled = true;calibrateWhiteButton.disabled = true;calibrateBlackButton.disabled = true;continuousEnabled = true;continuousMeasureButton.classList.remove(\"positive\");continuousMeasureButton.classList.add(\"negative\");continuousMeasureButtonIcon.classList.remove(\"play\");continuousMeasureButtonIcon.classList.add(\"loading\");continuousMeasureButtonIcon.classList.add(\"spinner\");fetchContinuously();}};const fetchContinuously = () => {if (continuousEnabled) {fetch(\"http://192.168.35.31/action=1\").then(r => r.text()).then(colors => {insertColors(colors);fetchContinuously();});}};const calibrate = (color, button) => {const referenceButton = document.getElementById(button);const referenceButtonIcon = document.getElementById(`${button}Icon`);referenceButton.disabled = true;referenceButtonIcon.classList.remove(\"sync\");referenceButtonIcon.classList.add(\"loading\");referenceButtonIcon.classList.add(\"spinner\");fetch(`http://172.20.10.8/${color}`).then(c => c.text()).then(_ => {referenceButton.disabled = false;referenceButtonIcon.classList.remove(\"loading\");referenceButtonIcon.classList.remove(\"spinner\");referenceButtonIcon.classList.add(\"sync\");});};const clearMeasurements = () => {chart.data.datasets[0].data = [];chart.data.datasets[1].data = [];chart.data.datasets[2].data = [];chart.data.labels = [];chart.update();Array.from(recentContainer.children).forEach(child => recentContainer.removeChild(child));};</script></body></html>"));
            }
    }
  }
}
void prototype(bool http) {
  //infrarot
  Analogwert = analogRead(A0);
  int Distanz = pow((3027.4 / Analogwert), 1.2134);

  //line sensor
  int QRE_Value1 = analogRead(QRE1113_Pin1);
  int QRE_Value2 = analogRead(QRE1113_Pin2);
  int QRE_Value3 = analogRead(QRE1113_Pin3);

  digitalWrite(inv, HIGH);   //richtung vorwärts
  digitalWrite(Vinh, HIGH);  //motor an

  digitalWrite(inv2, HIGH);   //richtung vorwärts
  digitalWrite(Vinh2, HIGH);  //motor an
  delay(2000);

  if (http) {
    // Send the result via HTTP
    String result = "";
    result += "Distanz: " + Distanz;
    result += "line detector 1: " + QRE_Value1;
    result += "line detector 2: " + QRE_Value2;
    result += "line detector 3: " + QRE_Value3;

    Serial.println(F("Calculated values"));
    Serial.println(result);
    uart.print(F("\nContent-Length: "));
    uart.print(result.length());
    uart.print("\n");
    uart.println();
    uart.print(result);
  } else {
    Serial.println("Distanz:");
    Serial.println(Distanz);
    delay(100);
    Serial.println("line detector1:");
    Serial.println(QRE_Value1);
    Serial.println("line detector2:");
    Serial.println(QRE_Value2);
    Serial.println("line detector3:");
    Serial.println(QRE_Value3);
  }
}

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


/*
    digitalWrite(inv, LOW);//richtung rückwärts
    analogWrite(Vinh, 100);//motor an
  
    digitalWrite(inv2, LOW);//richtung rückwärts
    analogWrite(Vinh2, 200);//motor an
    delay(2000);
    */

//lets go

//pause