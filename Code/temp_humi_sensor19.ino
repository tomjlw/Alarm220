/* Overheat Alarm*/
 
#include "TM1637.h" 
#include "DHT.h"

/* Macro Define */
#define CLK               39          /* 4-Digit Display clock pin */
#define DIO               38          /* 4-Digit Display data pin */
#define BLINK_LED         RED_LED            /* blink LED for the music */
#define TEMP_HUMI_PIN     24                 /* pin of temperature&humidity sensor */
#define BUZZER_PIN               40            /* sig pin of the Grove Buzzer */
#define LED GREEN_LED                          /* define the desired blinking LED lights */

/* Global Variables */
TM1637 tm1637(CLK, DIO);                  /* 4-Digit Display object */
DHT dht(TEMP_HUMI_PIN, DHT22);            /* temperature&humidity sensor object */

int8_t t_bits[2] = {0};                   /* array to store the single bits of the temperature */
int8_t h_bits[2] = {0};                   /* array to store the single bits of the humidity */  
int length = 15;         /* the number of notes */
char notes[] = "ccggaagffeeddc "; //notes in the song
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 }; //length of each note
char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
int tempo = 200;
int danger= 27;                         /* the maximum allowed temperature */

/* The following two functions are used to play the music 'Twinkle Twinkle Little Star‘ when being called*/
void playTone(int tone, int duration) 
      {
        for (long i = 0; i < duration * 1000L; i += tone * 2) 
        {
          digitalWrite(BUZZER_PIN, HIGH);        // turn the Buzzer on (HIGH is the voltage level)
          delayMicroseconds(tone);             
          digitalWrite(BUZZER_PIN, LOW);      //      turn the Buzzer off (LOW is the voltage level
          delayMicroseconds(tone);
        }
      }

void playNote(char note, int duration) 
{
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) 
  {
    if (names[i] == note) 
    {
      playTone(tones[i], duration);
    }
  }
}
/* the setup() method runs once, when the sketch starts */
void setup() 
{ 
    pinMode(BUZZER_PIN, OUTPUT); 
    pinMode(LED, OUTPUT); 
    digitalWrite(6, LOW);               /*Set Pin 6 a low voltage at first. This will make vibrator staic at first.*/
  
    tm1637.init();                       /* initialize 4-Digit Display */
    tm1637.set(BRIGHT_TYPICAL);          /* set the brightness */    
    tm1637.point(POINT_ON);              /* light the clock point ":" */
        
    dht.begin();                         /* initialize temperature humidity sensor */
        
    pinMode(BLINK_LED, OUTPUT);            /* declare the LED pin as an OUTPUT */
}

/* the loop() method runs over and over again */
void loop() 
{   /* play tone */
   
    int _temperature = dht.readTemperature();             /* read the temperature value from the sensor */
    int _humidity = dht.readHumidity();                   /* read the humidity value from the sensor */           
    memset(t_bits, 0, 2);                                 /* reset array before we use it */
    memset(h_bits, 0, 2);                                 /* reset array before we use it */
    
    /* 4-Digit Display [0,1] is used to display temperature */
    t_bits[0] = _temperature % 10;
    _temperature /= 10;
    t_bits[1] = _temperature % 10;
    
    /* 4-Digit Display [2,3] is used to display humidity */ 
    h_bits[0] = _humidity % 10;
    _humidity /= 10;
    h_bits[1] = _humidity % 10;
    
    /* show it */
    tm1637.display(1, t_bits[0]);
    tm1637.display(0, t_bits[1]);
    
    tm1637.display(3, h_bits[0]);
    tm1637.display(2, h_bits[1]);
   
    if (dht.readTemperature()>=danger)
   { //Loop through each note
           digitalWrite(6, HIGH); //activate the vibratoe by setting the voltage of Pin 6 High by forming the electrical potential btween Pin 6 and GND   
       
          for(int i = 0; i < length; i++) 
          {
            //space indicates a pause
           digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
           delay(50);               // wait for a second
           digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
           if(notes[i] == ' ') 
            {
              delay(beats[i] * tempo);
            } 
            else 
            {
              playNote(notes[i], beats[i] * tempo);
            }
            delay(tempo / 2);    /* delay between notes */
          } 
          
   } 
   else{digitalWrite(6, LOW);}  /* otherwise the vibrator will keep staic*/
}






 
