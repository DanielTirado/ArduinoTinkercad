#include <Adafruit_LiquidCrystal.h>
Adafruit_LiquidCrystal lcd_1(0);

float val = 0;
int analogPin = 0;
int buttonStart = 4;
int buttonInfo = 2;
bool startData = false;
bool startInfo = false;
float tolerancia = 0.03;
unsigned long finTime = 0;
unsigned long startTime =0;

int arregloSize = 230;
float *signal=new float[230];

int i = 0;

void setup()
{
  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
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
  delay(10);
  i++;
  if (i >= arregloSize){
    i = 0;
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
  float *dirsignal;
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
  dirsignal = signal;
  for (int j = 0; j < arregloSize; j++) {
    if (abs(*dirsignal-cero)<tolerancia){
    	t1 = j;
      	break;
    }
    dirsignal++;
  }
  
  for (int j = 0; j < arregloSize; j++) {
    if (abs(*dirsignal-cero)<tolerancia){
    	t2 = j;
      	break;
    }
    dirsignal++;
  }
  float diferencia = abs(abs(max) - abs(*maxnext));
  Serial.println(diferencia);
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
  periodo = 2*abs(t2-t1)/100;
  frecuencia = 1/periodo;
  
  mostrarDatos(amplitud, frecuencia);
  delete[] signal;
}