//definimos variables para controlar la pantalla LCD
#include <LiquidCrystal.h> //Librería que controla a pantalla LCD
LiquidCrystal lcd(13,11,5,4,3,2); // Indicamos ó LCD que clavijas vamos a usar para comunicarnos
const int controlPin6=6; //el pin 6 está conectado al pin 3 del LCD. Controla el brillo de la pantalla

//definimos variables para controlar el motor
const int controlPin9=9; //el pin 9 está conectado al pin 2 del puente H.Controla el sentido del motor y encendido/apagado
const int controlPin10=10; //el pin 10 está conectado al pin 7 do “puente H”.Controla el sentido del motor y encendido/apagado
const int velocidadMotor=600;
int ventanaAbierta=0; // indica ventana abierta o cerrada

//definimos variables para controlar el sensor
const int sensorPin= A0;// el sensor de CO2 está conectado al pin A0 
const int numeroLecturas=20; //número de lecturas que facemos con el lector de CO2
int lecturaSensor;//almacenamos el valor que devuelve o sensor 
float sumaValoresSensor; //suma de las lecturas que facemos con el lector de CO2
float mediaValoresSensor; //media de las lecturas que facemos con el lector de CO2
int partesMillonCO2; //partes por millon de CO2
// El sensor se comporta como una línea recta y = mx + n.
// Donde "y" son voltios y "x" es el exponente de la concentración.
// Tomamos dos puntos con un medidor de CO2 para calcular nuestra línea recta.
// Cuando el medidor da un valor de 520ppm, nuestro sensor da un valor de 408.
// Cuando el medidor da un valor de 1500 ppm, nuestro sensor da un valor de 264.
// Así (x1, y1) = (log 520, 408) y (x2, y2) = (log 1500,264)
// Con estos valores calculamos la pendiente (m) y la ordenada (n).
const float m=-313.06;
const float n=1258.26;
void setup() 
{
pinMode(controlPin6,OUTPUT);//el pin 6 está conectado al pin 3 del LCD.Controla el contraste de la pantalla
analogWrite(controlPin6,0);//No hay contraste. Brilla más.
lcd.begin(16,2); //Inicializamos el lcd con dos filas
lcd.print("Detector de CO2");//escribimos
lcd.setCursor(0,1);//Colocamos el cursor a la izquierda en la segunda fila 
lcd.print("--calentando---");//escribimos
pinMode(controlPin9,OUTPUT);//inicializamos la clavija digital 9 de salida
pinMode(controlPin10,OUTPUT);//inicializamos la clavija digital 10 de salida
delay(10000); //Esperamos 7 segundos mientras se calienta el sensor
}
void loop() 
{
sumaValoresSensor=0;  //inicializamos a cero 
//leemos varios valores que devuelve el sensor y hacemos la media
for (int n=0;n<numeroLecturas;n=n+1) //
{
  lecturaSensor=analogRead(sensorPin);
  sumaValoresSensor= lecturaSensor+sumaValoresSensor;
  delay(50); //espera 50 milisegundos antes de tomar más valores
}
mediaValoresSensor=sumaValoresSensor/numeroLecturas; //hacemos la media de valores 
delay(3000);
partesMillonCO2=pow(10,(mediaValoresSensor-n)/m);//calculamos las partes por millón segundo nuestra recta
lcd.clear();//borramos el lcd
lcd.setCursor(0,0);//colocamonos a la izquierda y la primeira fila
lcd.print("Detector de CO2");//escribimos
lcd.setCursor(0,1);//colocamonos a la izquierda y en la segunda fila
lcd.print(partesMillonCO2);//escribimos
lcd.setCursor(5,1);
lcd.print("ppm");//escribimos
if (partesMillonCO2>1000) 
{
  if (ventanaAbierta==0)
{     //abrimos la ventana
      // pin 9 y 1n0 controlan el motor.Cuando están os dous apagados
      //el motor no funciona. Si el 10 está encendido y el 9 apagado, o
      //al revés nos dice el sentido de giro.
      analogWrite(controlPin9,0);
      analogWrite(controlPin10,velocidadMotor);
     delay(18000); 
     //Dejamos que el motor funcione durante 18 segundos, mientras se abre la ventana.
     analogWrite(controlPin10,0); //apagamos o motor
     ventanaAbierta=1;
     //dejamos la ventana abierta durante 6 segundos
     //para que ventile antes de tomar máis valores
    delay(6000);}
}  
else
{
  if (ventanaAbierta==1)
{     //cerramos la ventana
     analogWrite(controlPin9,velocidadMotor);//cambiamos la dirección del motor
     analogWrite(controlPin10,0);
     delay(18000); //el motor funciona durante 18 segundos
     ventanaAbierta=0;
     analogWrite(controlPin9,0);//apagamos el motor}
}  
}

}
