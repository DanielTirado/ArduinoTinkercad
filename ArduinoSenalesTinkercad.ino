/*

RECOLECCIÓN DE DATOS Y MUESTRA DE DATOS RECOGIDOS.

*/


#include <Adafruit_LiquidCrystal.h>
Adafruit_LiquidCrystal lcd_1(0);

float val = 0;
int analogPin = 0;
int buttonStart = 4;
int buttonInfo = 2;
bool startData = false;
bool startInfo = false;

int arregloSize = 300;
float *signal = new float[arregloSize];

int i = 0;

void setup()
{
  pinMode(2, INPUT);
  pinMode(4, INPUT);
  Serial.begin(9600);
  lcd_1.begin(16, 2);
}

void loop()
{ 
  if(digitalRead(buttonStart) == HIGH) {
    i = 0;
    startData = true;
  	lcd_1.setCursor(0, 0);
  	lcd_1.print("Almacenando");
  	lcd_1.setCursor(0, 1);
  	lcd_1.print("Datos...");
  }
  
  if (startData){
    almacenarDatos();
  }
  if(digitalRead(buttonInfo) == HIGH) {
    i = 0;
    startInfo = true;
  	lcd_1.setCursor(0, 0);
  	lcd_1.print("Mostrandolos");
  	lcd_1.setCursor(0, 1);
  	lcd_1.print("Datos...");
  }
  if (startInfo){
    startData = false;
    mostrarDatos();
  }
  
}

void almacenarDatos() {
  val = analogRead(analogPin);
  val = val*(5.0/1023.0);
  signal[i] = val;
  Serial.println(signal[i]);
  delay(10);
  i++;
  if (i >= arregloSize){
    i = 0;
  }
}

void mostrarDatos(){
  Serial.println(signal[i]);
  delay(10);
  i++;
  if (i >= arregloSize){
    i = 0;
    startInfo= false;
    startData = true;
  }
}
  