int pump = 11; //bomba de agua
int led = 13; //led interno de la tarjeta arduino
int symonsays = 0; //nombre asignado al módulo emisor

void setup(){
  pinMode(led,OUTPUT);
  pinMode(pump,OUTPUT); 
  digitalWrite(led,LOW);
  Serial.begin(9600); //comunicación serial a 9600 baudios,
  //la velocidad por default
  digitalWrite(led,LOW); 
  }
  void loop()
  {
    if(Serial.available()>0)//si hay comunicación disponible
    {
      symonsays = Serial.read();//el módulo emisor será el dato serial
     //de lectura, y de ser así entonces:
      if(symonsays == '1')
      {
        digitalWrite(pump,HIGH);  
        }
        else
        {
          digitalWrite(pump,LOW);
        }
      }
      }
