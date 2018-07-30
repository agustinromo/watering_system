                /*Emisor Señales y Actuador de dirección*/                           
#include <SoftwareSerial.h> //librería para módulo maestro
//no es necesaria en módulo esclavo
#include <DHT.h> //librería específica para el sensor DHT11
#include <Servo.h> //librería específica para el servo 

Servo servoMotor; //servo

SoftwareSerial mySerial(2,3); //RX,TX en arduino para interconexión
//con RX y TX de módulo maestro
#define DHTPIN 7 //DHT
#define DHTTYPE DHT11

int led = 13; //led interno de la tarjeta arduino
int foto; //sensor de luz (fotocelda)
int ilum; //variable que guarda los datos del sensor de luz
const long A= 1000; //Datos usados para digitalización del sensor luz
const int B= 15;  //al final no se usaron
const int Rc= 10; 
int h; //variable que guarda los datos de humedad ambiente registrados 
//por el DHT11
int t;//variable que guarda los datos de temperatura ambiente registrados 
//por el DHT11
int npn = 5; //transistor npn 2n2222 que activa al módulo maestro 
//(interruptor del módulo) véase el esquema de conexión al arduino
int fc = 4; //sensor fc-28, sensor de humedad en la tierra 
DHT dht(DHTPIN, DHTTYPE); 
void setup()
{
  pinMode(fc,INPUT);
  pinMode(led,OUTPUT);
  pinMode(npn,OUTPUT);
  Serial.begin(9600); 
  //comunicación serial a 9600 baudios,
  //la velocidad por default
  mySerial.begin(9600);
  dht.begin();
  servoMotor.attach(9);
  digitalWrite(led,LOW);
}
void loop()
{
  ///DHT////
  delay(5000);
   h = dht.readHumidity();
   t = dht.readTemperature();
  if (isnan(h) || isnan(t) ) {
    Serial.println("Error en sensor DHT11");
    return; 
  }
  int hic = dht.computeHeatIndex(t, h, false); 

   ////////fotocelda//////////////// 
  foto = analogRead(A0); 
  //ilum = ((long)foto*A*10)/((long)B*Rc*(1024-foto)); 
  ilum = analogRead(foto); 
  //Serial.print("Luz: "); 
  //Serial.print(ilum); 
 
  /////////imprimiendo resultados de fotocelda dht11 y fc-28/////////////// 
  if(ilum > 256 && ilum < 512)///Nivel de luz: poco
  {
    ////fc-28/////
    if(digitalRead(fc) == LOW)
  {
    Serial.print("Humedad: ");
    Serial.print(h);
    Serial.print(" %");
    Serial.print(" Temperatura: ");
    Serial.print(t);
    Serial.print(" *C "); 
    Serial.print(" Luz: Poca.");
    Serial.println(" Humedad de suelo: si"); 
  }
   else 
   { 
    Serial.print("Humedad: ");
    Serial.print(h);
    Serial.print(" %");
    Serial.print(" Temperatura: ");
    Serial.print(t);
    Serial.print(" *C "); 
    Serial.print(" Luz: Poca.");
    Serial.println(" Humedad de suelo: no"); 
   } 
  
  }
  if(ilum > 512 && ilum < 768)///Nivel de luz: medio
  {
    if(digitalRead(fc) == LOW)
  {
    Serial.print("Humedad: ");
    Serial.print(h);
    Serial.print(" %");
    Serial.print(" Temperatura: ");
    Serial.print(t);
    Serial.print(" *C "); 
    Serial.print(" Luz: Media.");
    Serial.println(" Humedad de suelo: si"); 
  }
   else 
   { 
    Serial.print("Humedad: ");
    Serial.print(h);
    Serial.print(" %");
    Serial.print(" Temperatura: ");
    Serial.print(t);
    Serial.print(" *C "); 
    Serial.print(" Luz: Media.");
    Serial.println(" Huemdad de suelo: no"); 
   } 
  }
  if(ilum > 768)///Nivel de luz: máximo
  {
    if(digitalRead(fc) == LOW)
  {
    Serial.print("Humedad: ");
    Serial.print(h);
    Serial.print(" %");
    Serial.print(" Temperatura: ");
    Serial.print(t);
    Serial.print(" *C "); 
    Serial.print(" Luz: Máxima.");
    Serial.println(" Humedad de suelo: si"); 
  }
   else 
   { 
    Serial.print("Humedad: ");
    Serial.print(h);
    Serial.print(" %");
    Serial.print(" Temperatura: ");
    Serial.print(t);
    Serial.print(" *C "); 
    Serial.print(" Luz: Máxima.");
    Serial.println(" Humedad de suelo: no"); 
   } 
  }  
  actuar();
} 
void actuar()
{
  if(digitalRead(fc)==HIGH)//sensor de humedad de suelo en 0
  { //no se porque pero ps es con HIGH en lugar de LOW
    if(ilum >= 512)//luz externa entre nivel máximo y medio
    {
      if(t >= 20 && h <= 50)//temperatura ambiente >= 20ª y  
      //humedad ambiente <= 50% 
      { 
        digitalWrite(npn,HIGH);//el transistor 2n22 activa el módulo
        delay(5000);
        if(npn)//y una vez que activa el módulo, manda a llamar la 
         // función
        {      
          servoBlue();
          }
        }
      }
    }
    else 
    {
      digitalWrite(npn,LOW);
      }
  }
void servoBlue()//función de activación del módulo y el actuador servo
{
   // Desplazamos a la posición 0º
  servoMotor.write(90);            mySerial.write('1'); Serial.println("Riego activado");
  // Esperamos 1 segundo
  delay(1000);                     delay(5000);
  
  // Desplazamos a la posición 70º
  servoMotor.write(130);
  // Esperamos 1 segundo
  delay(1000);
  
  // Desplazamos a la posición 180º
  servoMotor.write(90);
  // Esperamos 1 segundo
  delay(1000); 

  servoMotor.write(50);
  // Esperamos 1 segundo
  delay(1000); 

   servoMotor.write(90);
  // Esperamos 1 segundo
  delay(1000); 

   // Desplazamos a la posición 70º
  servoMotor.write(130);
  // Esperamos 1 segundo
  delay(1000);
  
  // Desplazamos a la posición 180º
  servoMotor.write(90);
  // Esperamos 1 segundo
  delay(1000); 

   servoMotor.write(50);
  // Esperamos 1 segundo
  delay(1000); 

   servoMotor.write(90);       mySerial.write('0'); Serial.println("Riego Desactivado");
  // Esperamos 1 segundo
  delay(1000);                 delay(5000);
 }

