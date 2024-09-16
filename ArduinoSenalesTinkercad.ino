#include <Adafruit_LiquidCrystal.h>
using namespace std;
Adafruit_LiquidCrystal lcd_1(0);

float val = 0;
int analogPin = 0;
int buttonStart = 4;
int buttonInfo = 2;
bool startData = false;
bool startInfo = false;
float tolerancia = 0.01;

int arregloSize = 200;
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
    datos();
    startInfo= false;
    startData = true;
  }
  
}

void almacenarDatos() {
  val = analogRead(analogPin);
  val = val*(5.0/1023.0);
  signal[i] = val;
  delay(10);
  i++;
  if (i >= arregloSize){
    i = 0;
  }
}

void mostrarDatos(float vol, float frec){
  lcd_1.clear();
  lcd_1.setCursor(0, 1);
  lcd_1.print("V=");
  lcd_1.print(vol,2);
  lcd_1.setCursor(7, 1);
  lcd_1.print("f=");
  lcd_1.print(frec,9);
  delay(3000);
}

void datos(){
  float min = signal[0];
  float max = signal[0];
  float amplitud;
  float frecuencia;
  
  for (int j=1; j<arregloSize; j++){
    if (signal[j]<min){
    	min = signal[j];
    }
    if (signal[j]>max){
    	max = signal[j];
    }  
  }
  
  //Amplitud
  if (min*max < 0){
  	amplitud = abs(min)+ max;
  } else if(min*max>0){
  	amplitud = abs(abs(max)-abs(min));
  }
  
  //Frecuencia. Primero hallamos el periodo.
  int t1 = 0;
  int t2 = 0;
  for (int k=1; k<arregloSize; k++){
    if (abs(abs(max) - signal[k])<tolerancia && t2!=k){
    	t1 = k;
        k++;
    }
    if (abs(abs(max) - signal[k])<tolerancia && t1 == k){
    	t2 = k;
    	break;
    }
  }
  frecuencia = 1/(abs(t2-t1)*0.01);
  
  mostrarDatos(amplitud, frecuencia);
  delete[] signal;
}
