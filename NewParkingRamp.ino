
#include <Servo.h>
#include <LiquidCrystal.h>
#include <virtuabotixRTC.h>
#include <SoftwareSerial.h>

LiquidCrystal lcd(12, 11, 13, 10, 9, 8);
long ticketID;
int pin_switch = 2;
const int servoPin = 3;
int buttonCounter=6;
const int buzzer = 7;
 Servo servo;


virtuabotixRTC myRTC(4, 5, 6); 

void setup () 
{
  lcd.begin(16, 2);
    Serial.begin(9600);
  randomSeed(analogRead(1));  
  pinMode(buzzer, OUTPUT);
  pinMode(pin_switch, INPUT);
  servo.attach (servoPin);

 // seconds, minutes, hours, day of the week, day of the month, month, year
 myRTC.setDS1302Time(00, 40, 15, 2, 9, 6, 2020); //Here you write your actual time/date as shown above 

}
 
void loop()
{

 
  
  
 
    lcd.setCursor(0, 0);
    lcd.println("Free spaces:        ");          
    lcd.setCursor(7, 1);
    lcd.print(buttonCounter);
    lcd.println("        ");
  


  int Light = analogRead(A0);
  if (buttonCounter==0  || Light<300){ //After the car passes thro sensor ramp closes,
                                        //or if no more space on parking ramp stays closed
    servo.write(60);   
       
    }
     
        
else if ( digitalRead(pin_switch) == LOW )
    {
 
         servo.write(180);
            Serial.println("THIS IS YOUR BILL!");   //Prints out the reciet
            Serial.println("Enterence Time: ");       //and information
                        
                   myRTC.updateTime();
            
                  Serial.print("Current Date / Time: ");
                  Serial.print(myRTC.dayofmonth); 
                  Serial.print("/");
                  Serial.print(myRTC.month);
                  Serial.print("/");
                  Serial.print(myRTC.year);
                  Serial.print(" ");
                  Serial.print(myRTC.hours);
                  Serial.print(":");
                  Serial.print(myRTC.minutes);
                  Serial.print(":");
                  Serial.println(myRTC.seconds);//and information
            Serial.println("Charge is 1KM per Hour");
            Serial.println("First hour is for Free");
            ticketID = random(100000, 999999);
               Serial.print("Ticket ID is: ");
               Serial.println(ticketID);
              Serial.println("");
          Serial.println("");
          buttonCounter--;          //ammunt of parking spaces decreases
            delay(500);   }
            else if (Serial.available()){
    char item_specifier = Serial.read();
      write_items(item_specifier);
 }}

void write_items(char item)         // this is for communication between arduino and Qt
{
  if (item == 'c'){                  // if Qt sends C string to arduino it will close ramp
   servo.write(60);
    return;
  }
  else if (item=='o'){              // if Qt sends o it will open ramp
    servo.write(180);
    return;
  } 
   else if (item=='+'){           //if + increase spaces
    buttonCounter++;
    return;
  } 
   else if (item=='-'){           // if - decrease spaces
    buttonCounter--;
    return;
  } 

}
                
           

                
           
           
