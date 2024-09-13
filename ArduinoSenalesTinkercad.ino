/*
Montaje con dos pulsadores: uno para iniciar la muestra de datos y 
otro para detenerla. En realidad, un pulsador en alto permite que se 
haga la lectura del pin analógico, mientras que el otro pulsador en 
alto le asigna un valor de 0 a este pin.

Próximamente se modificará el montaje de tal manera que un pulsador 
solicite la información de la señal capturada (frecuencia y amplitud) 
calculada con algún algoritmo.

*/
#include <Adafruit_LiquidCrystal.h>
Adafruit_LiquidCrystal lcd_1(0);

int val = 0;
int analogPin = 0;
int button1Pin = 2;
int button2Pin = 4;
bool startData = false;

void setup()
{
  pinMode(2, INPUT);
  pinMode(4, INPUT);
  Serial.begin(9600);
  lcd_1.begin(16, 2);
}

void loop()
{ 
  lcd_1.setCursor(2, 1);
  if(digitalRead(button1Pin) == HIGH) {
    startData = true;
  }
  if(digitalRead(button2Pin) == HIGH) {
    startData = false;
  }
  
  if(startData) { 
    val = analogRead(analogPin);
  }
  else{
    val = 0;
  }
  lcd_1.print(val);
  delay(10);
  Serial.println(val);
  lcd_1.clear();
}