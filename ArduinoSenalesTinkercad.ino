/*

RECOLECCIÓN DE DATOS

FALTA: CORREGIR FUNCIONAMIENTO DEL BOTON 2 Y VERIFICAR LOS DATOS
RECOLECTADOS
*/


#include <Adafruit_LiquidCrystal.h>
Adafruit_LiquidCrystal lcd_1(0);

float val = 0;
int analogPin = 0;
int buttonStart = 4;
int buttonInfo = 2;
bool startData = false;
bool startInfo = false;

float *signal;
int arregloSize = 300;

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
    startData = true;
    lcd_1.clear();
  	lcd_1.setCursor(0, 0);
  	lcd_1.print("Almacenando");
  	lcd_1.setCursor(0, 1);
  	lcd_1.print("Datos...");
  }
  
  if(digitalRead(buttonInfo) == HIGH) {
    startInfo = true;
    lcd_1.clear();
  	lcd_1.setCursor(0, 0);
  	lcd_1.print("Mostrando");
  	lcd_1.setCursor(0, 1);
  	lcd_1.print("Datos...");
  }
  
  if (startData){
  	almacenarDatos();
    
  }
  
  if (startInfo){
    startData = false;
  	mostrarDatos();
  }
}

void almacenarDatos() {
  signal = new float[arregloSize];
   for (int i = 0; i < arregloSize; i++) {
    val = analogRead(analogPin);
    val = val*(5.0/1023.0);
    signal[i] = val;
    Serial.println(signal[i]);
    delay(10);
  }
}

void mostrarDatos(){
  /*
  lcd_1.setCursor(2, 1);
  lcd_1.print(val);
  delay(10);
  Serial.println(val);
  lcd_1.clear();*/
}