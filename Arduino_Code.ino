//Required Libraries//
#include <LiquidCrystal.h> //For LCD
#include <EEPROM.h>        //For EEPROM
#include <SoftwareSerial.h> //For GSM

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);  //LCD is connected to 2,3,4,5,6,7 pins of arduino
SoftwareSerial SIM900(10, 9);  //GSM is connected to 10,9 pins of arduino 

char inchar; //It will hold the incoming character from GSM shield

float Voltage1 = A0;    //Voltmeter is connected to A0 pin of arduino
float Current1 = A1;    //Ammeter is connected to A1 pin of arduino


float I1 = 0; //Defining current variable for LCD
int V1 = 0;  //Defining voltage variable for LCD

int out1=8; //Relay is connected to pin 8 of arduino for connecting/disconnecting load

int power; //Defining power variable 
int energy; //Defining energy variable

int unt=0; //Defining and initialising unit(kwh) variable
int prs=0; //Defining and initialising price variable

int p1=0;
int p2=0;

int con1=0;
int con2=0;

int buz=13; //Buzzer is connected to pin 13 of arduino

void setup()
{ 
  SIM900.begin(9600);  //Start GSM
  Serial.begin(9600);  //Start Arduino

  pinMode(Voltage1,INPUT);        //Set voltage as input pin
  pinMode(Current1,INPUT);        //Set current as input pin
  
  pinMode(out1,OUTPUT);  //Set relay as output pin
  pinMode(buz,OUTPUT);   //Set buzzer as output pin
  
  #Displaying welcome message on LCD
  lcd.begin(20, 4);
  lcd.clear();
  lcd.setCursor(6,1);
  lcd.print("WELCOME");
  lcd.setCursor(5,2);
  lcd.print("Energy Meter");
 
  #Setting GSM Module
  SIM900.print("AT\r\n");  // Check communication
  delay(1000);
  SIM900.print("AT+CMGF=1\r\n");  // set SMS mode to text
  delay(1000);
  SIM900.print("AT+CNMI=2,2,0,0,0\r\n"); 
  delay(1000);
  sim1(); 
  delay(1000);
  SIM900.print("ON GSM"); // Message contents
  delay(500);
  SIM900.write(byte(26)); // (signals end of message)
  delay(1000);
  SIM900.println("AT+CMGD=1,4\r\n"); // delete all SMS
  delay(500);
  digitalWrite(buz,LOW); //Turn buzzer off
  lcd.clear();
  
  //Reading stored values from EEPROM
  unt = EEPROM.read(2); 
  p2 = EEPROM.read(0);
  prs = EEPROM.read(1);
  
  //Turning relay ON if available unit is greater than 0
  if(unt>0){digitalWrite(out1.high)}
  
  }
   
   
  //Recharging through GSM
  void loop() 
  {
  if(SIM900.available() >0)
  {
    inchar=SIM900.read(); 
    if (inchar=='R')
    {
 
      delay(10);
      inchar=SIM900.read(); 
      if (inchar=='1') //Send code R1 for 100 recharge in which you will get 20 units
      {
         sim1();
         delay(1000);
         SIM900.println("Recharge Card 100"); // Message contents
         delay(500);
         SIM900.write(byte(26)); // (signals end of message)
         delay(500);  
         digitalWrite(out1,HIGH); 
         prs = prs+100;
         unt = prs/5;
         con1=0;
         con2=0;
        }
        else if (inchar=='2') //Send code R2 for 200 recharge in which you will get 40 units
        {
          sim1();
          delay(1000);
          SIM900.println("Recharge Card 200"); // Message contents
          delay(500);
          SIM900.write(byte(26)); // (signals end of message)
          delay(500);     
          digitalWrite(out1,HIGH); 
          prs = prs+200;
          unt = prs/5;
          con1=0;
          con2=0;
         }
         else if (inchar=='3') //Send code R3 for 300 recharge in which you will get 60 units
         {
           sim1();
           delay(1000);
           SIM900.println("Recharge Card 300"); // Message contents
           delay(500);
           SIM900.write(byte(26)); // (signals end of message)
           delay(500);   
           digitalWrite(out1,HIGH); 
           prs = prs+300;
           unt = prs/5;
           con1=0;
           con2=0;
          }
          else if (inchar=='4') //Send code R4 for 400 recharge in which you will get 80 units
           {
            sim1();
            delay(1000);
            SIM900.println("Recharge Card 400"); // Message contents
            delay(500);
            SIM900.write(byte(26)); // (signals end of message)
            delay(500);  
            digitalWrite(out1,HIGH); 
            prs = prs+400;
            unt = prs/5;
            con1=0;
            con2=0;
           }
           else if (inchar=='5') //Send code R5 for 500 recharge in which you will get 100 units
           {
            sim1();
            delay(1000);
            SIM900.println("Recharge Card 500"); // Message contents
            delay(500);
            SIM900.write(byte(26)); // (signals end of message)
            delay(500);    
            digitalWrite(out1,HIGH); 
            prs = prs+500;
            unt = prs/5;
            con1=0;
            con2=0;
           }
           else if (inchar=='D') //Send code RD for getting details about unit,price,energy,power
           {
            sim1();
            delay(1000);
            SIM900.print("Unit="); // Message contents
            SIM900.println(unt); // Message contents
            SIM900.print("Price="); // Message contents
            SIM900.println(prs); // Message contents
            SIM900.print("Energy="); // Message contents
            SIM900.println(energy); // Message contents
            SIM900.print("Power="); // Message contents
            SIM900.println(power); // Message contents
            delay(500);
            SIM900.write(byte(26)); // (signals end of message)
            delay(500);    
            con1=0;
            con2=0;
           }
           delay(10);
           SIM900.println("AT+CMGD=1"); // delete all SMS
      
      }
    }
  }
   
 for (int n=0;n<1000;n++)
  {
  V1= ((analogRead(Voltage1)*(4.5/1023)*60));
  I1= ((analogRead(Current1)*(4.5/1023)*0.4545));
  inst_power=V1*I1; 
  sum_inst_power=sum_inst_power+abs(inst_power);
  delay(0.05);
  }
  
  powerCount =sum_inst_power/1000.0;
  time=millis();
  energyCount =(powerCount*time/1000)/3600;
    
  displayEnergyData(); 
  reconnection(); 
  energyCalculation(); 
  writeEngData(); 
} //pulse counterand calcounter////////////////////////////////////////////////////////////

  
  void reconnection()
  { 
    if(recharged_amount >= 6 && r==1 )
    { 
      r=0; 
      d = 1; 
      generateReconSignal(); 
    
      } 
      } //energyCalculation****************************************************************** ************** 
      
      void energyCalculation()
      { 
        if(recharged_amount>= 6.00)
        { 
          remaining_units += recharged_amount ; 
          last_recharge = recharged_amount ; 
          temp_recharged= recharged_amount ; 
          recharged_amount = 0.00; 
          } 
          if (energyCount >= 1)
          
          { 
          energyCount = 0 ; 
          monthly_consumption += 1; //0.1kWh = 1Wh 
          total_consumption += 1 ; //0.1kWh = 1Wh 
          remaining_units -= 1; //0.1kWh = 1Wh 
          }       
          }
 //Power calculation//////////////////////////////////////////////////////////////////////////Powercalculation with a maximum error of 1.25W 
 
 void powerCalculation()
 { 
  float averageFrequency = powerCount/(900); 
  averagePower = averageFrequency/frequencyPerKiloWatt ; 
  powerCount =0; 
  } //Display ////////////////////////////////////////////////////////////////////////////////// 
  
  
  
  void displayEnergyData()
  { 

    lcd.setCursor(0,1); 
    lcd.print(remaining_units); 
    delay(20); 
    lcd.setCursor(11,1); 
    lcd.print(last_recharge); 
    delay(20); 
    lcd.setCursor(0,2); 
    lcd.print(total_consumption); 
    delay(20); 
    lcd.setCursor(11,2); 
    lcd.print(monthly_consumption); 
    delay(20); 
    lcd.setCursor(0,3); 
    
    delay(20); 
    lcd.setCursor(11,3); 

    delay(20); 
    } //discon&recon////////////////////////////////////////////////////////////////////////////////////////// 
    void generateDiconSignal()
    { 
      digitalWrite(contactor,LOW); 
      } 
      
      void generateReconSignal()
      { 
        digitalWrite(contactor,HIGH); 
      } //Generating notification//////////////////////////////////////////////////////////////////// 

    
void month_reset() 
{ 
  if(mon_reset==1) 
  { 
    monthly_consumption=0; 
    } 
    } //////////////////////////////////////////////////////////////////////////////////// 
 //copying values from EEProm///////////////////////////////////////////////////// 
void readfromEEPROM()
{ 
  EEPROM.get(addr_recharged_amount,recharged_amount); 
  delay(200); 
  EEPROM.get(addr_remaining_units,remaining_units); 
  delay(200); 
  EEPROM.get(addr_total_consumption,total_consumption); 
  delay(200); 
  EEPROM.get(addr_last_recharge,monthly_consumption); 
  delay(200); 
  EEPROM.get(addr_monthly_consumption,last_recharge); 
  delay(200); 
  EEPROM.get(addr_energy_count,energyCount); 
  delay(200); 
  if(remaining_units>1) 
  { 
    generateReconSignal(); 
    } 
    
    } ///////////////////////put the serverevery 15 minits//////////////////////////////// 
    void time_check() 
    { 
      if(millis()<60000 && t==1) 
      { 
        time_int=0; 
        t=0; 
        } 
        if(millis()-time_int>60000) 
        { 
          time_int=millis(); 
          t=1; 
          powerCalculation(); 
          } 
          } //////////////////////////////////////write energy data/////////////////////////////////// 
          void writeEngData() 
          { 
            EEPROM.put(addr_recharged_amount,recharged_amount); 
            delay(200); 
            EEPROM.put(addr_remaining_units,remaining_units); 
            delay(200); 
            EEPROM.put(addr_total_consumption,total_consumption); 
            delay(200); 
            EEPROM.put(addr_last_recharge,last_recharge);
            delay(200); 
            EEPROM.put(addr_monthly_consumption,monthly_consumption); 
            delay(200); 
            EEPROM.put(addr_energy_count,energyCount); 
            delay(200); 
          }
           
void writeEngDataGSM() 
{ 
  EEPROM.put(addr_recharged_amount,recharged_amount); 
  delay(200); 
  EEPROM.put(addr_monthly_consumption,monthly_consumption); 
  delay(200); 
  } /////////shieldon/////////// 
  

void sim1()  
{
SIM900.println("AT+CMGS=\"1234567890\"\r\n"); // Sends message  1  
}
