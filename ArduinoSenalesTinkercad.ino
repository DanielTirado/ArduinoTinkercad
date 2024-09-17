#include <Adafruit_LiquidCrystal.h>
Adafruit_LiquidCrystal lcd_1(0);

float val = 0;
int analogPin = 0;
int buttonStart = 4;
int buttonInfo = 2;
bool startData = false;
bool startInfo = false;
float tolerancia = 0.04;

int arregloSize = 230;
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
    lcd_1.clear();
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
    lcd_1.clear();
  	lcd_1.setCursor(0, 0);
  	lcd_1.print("Mostrando");
  	lcd_1.setCursor(0, 1);
  	lcd_1.print("Datos...");
  }
  if (startInfo){
    startData = false;
    datos();
    startInfo= false;
  }
  
}

void almacenarDatos() {
  val = analogRead(analogPin);
  val = val*(5.0/1023.0);
  signal[i] = val;
  Serial.println(val);
  delay(10);
  i++;
  if (i >= arregloSize){
    i = 0;
    startData = false;
  }
}

void mostrarDatos(float vol, float frec){
  lcd_1.setCursor(0, 1);
  lcd_1.print("V=");
  lcd_1.print(vol,2);
  lcd_1.setCursor(7, 1);
  lcd_1.print("f=");
  lcd_1.print(frec,2);
}

void datos(){
  lcd_1.clear();

  float min=signal[0];
  float max=signal[0];
  float amplitud, frecuencia, periodo, t1, t2;
  float *maxnext;
  int m= 0;
  maxnext = signal;
  for (m; m<arregloSize; m++){
    if (signal[m]<min){
    	min = signal[m];
    }
    if (signal[m]>max){
    	max = signal[m];
        *maxnext= signal[m+1];
    }
  }
  //Amplitud
  if (min*max < 0){
  	amplitud = abs(min)+ max;
  } else if(min*max >=0.0){
  	amplitud = abs(abs(max)-abs(min));
  }
  //FRECUENCIA:
  
  float cero = (min + max)/2;
  for (int j = 0; j < arregloSize; j++) {
    if (abs(abs(signal[j])-cero)<tolerancia){
    	t1 = j;
      	break;
    }
  }
  
  for (int j = 0; j < arregloSize; j++) {
    if (abs(abs(signal[j])-cero)<tolerancia && t1!=j){
    	t2 = j;
      	break;
    }
  }
  float diferencia = abs(abs(max) - abs(*maxnext));
  //Serial.println(diferencia);
   if (diferencia == 0){
        lcd_1.setCursor(0, 0);
        lcd_1.print("Cuadrada");
    } else if (diferencia < tolerancia && diferencia>0) {
        lcd_1.setCursor(0, 0);
        lcd_1.print("Senoidal");
    } else if (diferencia > tolerancia) {
        lcd_1.setCursor(0, 0);
        lcd_1.print("Triangular");
    } else {
        lcd_1.setCursor(0, 0);
        lcd_1.print("Desconocida");
    }
  periodo = 2*10*abs(t2-t1)/1000;
  frecuencia = 1/periodo;
  Serial.println(periodo);
  mostrarDatos(amplitud, frecuencia);
}