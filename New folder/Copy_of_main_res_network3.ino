#define DEBUG
#include <MsTimer2.h>
#include "TimerThree.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_IS31FL3731.h>
#include <Adafruit_MCP4725.h>
//#include <Entropy.h>

uint16_t Left_Panel =  0x76;
uint16_t Center_Panel =  0x74;
uint16_t Right_Panel =  0x75;

//these for testing
//uint16_t Left_Panel =  0x74;
//////uint16_t Center_Panel =  0x74;
//uint16_t Right_Panel =  0x75;

// If you're using the full breakout...
Adafruit_IS31FL3731 ledmatrixL = Adafruit_IS31FL3731();
Adafruit_IS31FL3731 ledmatrixC = Adafruit_IS31FL3731();
Adafruit_IS31FL3731 ledmatrixR = Adafruit_IS31FL3731();

Adafruit_MCP4725 dac;
#define MAX9744_I2CADDR 0x4B
#define DAC_RESOLUTION    (8)

// If you're using the FeatherWing version
//Adafruit_IS31FL3731_Wing ledmatrix = Adafruit_IS31FL3731_Wing();
//void drawFastVLine(uint16_t x0, uint16_t y0, uint16_t length, uint16_t color);
//void drawFastHLine(uint8_t x0, uint8_t y0, uint8_t length, uint16_t color);
uint16_t x0 = 0;
uint16_t y0 = 15;
uint16_t lp = 0;
uint32_t  reward_pulse_C = 0; 
uint32_t  reward_pulse_R = 0; 
uint32_t  reward_pulse_L = 0; 
uint32_t  airpuff_pulse = 0; 
uint32_t  light_pattern_speed = 20;
uint8_t  light_pattern_brightness = 4;
uint32_t  tre = 0;

int8_t  vol = 20; 
int8_t  oldvol = 5; 
int8_t  int_flag1 = 0; 
int8_t  int_flag2 = 0; 
int8_t  int_flag3 = 0; 
int8_t int_active = 0;

uint16_t temp = 0;
uint16_t temp1 = 0;
uint16_t temp2 = 0;
uint16_t temp3 = 0;
uint16_t temp4 = 0;
uint16_t int_disable_start = 0;
uint16_t temp_time = 0;

uint16_t tonefreq;
uint16_t ontime;
uint16_t offtime;

int8_t  int1_lvl = 0; 
int8_t  int2_lvl = 0; 
int8_t  int3_lvl = 0; 
int8_t  int1_lvl_old = 0; 
int8_t  int2_lvl_old = 0; 
int8_t  int3_lvl_old = 0; 
uint8_t  temp_byte = 0; 
uint8_t  temp_byte1 = 0; 
uint8_t  left_right_flag = 0;
uint8_t  noise_tone_flag = 0;
uint8_t  panel_mode = 0;

uint16_t left_half_cycle = 0;
uint16_t right_half_cycle = 0;
uint8_t  number_of_left_cycles = 0;
uint8_t  number_of_right_cycles = 0;

uint16_t left_cycle_start = 0;
uint16_t right_cycle_start = 0;
uint8_t bright_dark_L = 7;
uint8_t bright_dark_R = 7;
uint8_t single_panel_L = 0;
uint8_t single_panel_R = 0;
uint8_t light_tone_flag = 0;
//=========================================================
//pin assignments
const byte Tone0 = 47;  
const byte Tone1 = 49;  
const byte AirPuff = 48;
const byte RewardL = 50;  
const byte RewardR = 51;  
const byte RewardC = 52;  
const byte TestOut = 53;  
const byte light_panel_signal_L = 38;  
const byte light_panel_signal_R = 40;  

const byte reset_indicator = 44;  
const byte nint1 = 10;
const byte interruptPin1 = 18;
const byte interruptPin2 = 19;
const byte nint2 = 11;
const byte interruptPin3 = 2;
const byte nint3 = 12;
const byte cclockpin = 22;     // default to pin 13

// for resistor networks
const byte light0 = 23;
const byte light1 = 25;
const byte light2 = 27;
const byte light3 = 29;
const byte light4 = 31;
const byte light5 = 33;
const byte light6 = 35;
const byte light7 = 37;

const byte Rew_L = 39;
const byte Rew_C = 41;
const byte Rew_R = 43;
//end of pin assignments



volatile byte state = LOW;
uint8_t  xstart = 3;
uint8_t  ystart = 0;
uint8_t  xstop = 6;
uint8_t  ystop = 9;
uint8_t noise_active = 0;
uint32_t pwidth = 100;
uint16_t debounce_count = 0;

#define PIN_RND 47
uint32_t Rnd;
byte LowBit;

//-----------------------------------
//    a  set audio volume
//    b  set rate discrimination
//    d  send air puff
//    e  tone 0 on/off
//    f  tone 1 on/off
//    i  move actuator 1
//    j  move actuator 2
//    k  valve control
//    l  noise sequence
//    p  enable interrupts
//    q  reward duration C
//    r  Light Panel L
//    s  Light Panel
//    t  Light Panel R
//    v  reward duration L
//    w  reward duration R
//    u  air puff pulse duration
//    x  reward R
//    y  reward L
//    z  reward C
//    >  noise on
//    <  noise off
//    ?  left/right flag
//    {  light_pattern_speed
//    }  light_pattern_brightness
//    *  Sensory Stimulus panel mode
//    (  pattern left_right_vertical
//    )  pattern right_left_vertical
//    [  pattern top to bottom
//    ]  pattern bottom to top
//    (  pattern left_right_vertical double bars
//    )  pattern right_left_vertical double bars
//    [  pattern top to bottom double bars
//    ]  pattern bottom to top double bars
//    #  Pulse durations
//
//-----------------------------------

/**
 * TCA9548 I2CScanner.pde -- I2C bus scanner for Arduino
 *
 * Based on code c. 2009, Tod E. Kurt, http://todbot.com/blog/
 *
 */

//#include "Wire.h"
extern "C" { 
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}

#define TCAADDR 0x70

void tcaselect(uint8_t i) {
  if (i > 7) return;

  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

// standard Arduino setup()
void setup()
{
    pinMode(Tone0, OUTPUT);
    pinMode(Tone1, OUTPUT);
    pinMode(RewardL, OUTPUT);
    pinMode(RewardC, OUTPUT);
    pinMode(RewardR, OUTPUT);
    pinMode(AirPuff, OUTPUT);
    pinMode(cclockpin, OUTPUT);
    pinMode(reset_indicator, OUTPUT);

    
    pinMode(light0, INPUT);
    pinMode(light1, INPUT);
    pinMode(light2, INPUT);
    pinMode(light3, INPUT);
    pinMode(light4, INPUT);
    pinMode(light5, INPUT);
    pinMode(light6, INPUT);
    pinMode(light7, INPUT);

    pinMode(Rew_L, INPUT);
    pinMode(Rew_C, INPUT);
    pinMode(Rew_R, INPUT);
    
    pinMode(TestOut, OUTPUT);
    pinMode(light_panel_signal_L, OUTPUT);
    pinMode(light_panel_signal_R, OUTPUT);
    digitalWrite(TestOut, LOW);
    digitalWrite(light_panel_signal_L, LOW);
    digitalWrite(light_panel_signal_R, LOW);
    
    digitalWrite(Tone0, LOW);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(Tone1, LOW);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(RewardL, LOW);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(RewardC, LOW);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(RewardR, LOW);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(AirPuff, LOW);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(reset_indicator, LOW);   // turn the LED on (HIGH is the voltage level)
    

    while (!Serial);
    delay(100);
    Wire.begin();    
    Serial.begin(115200);
//    Serial.println("\nTCAScanner ready!");

    pinMode(interruptPin1, INPUT_PULLUP);
    pinMode(interruptPin2, INPUT_PULLUP);
    pinMode(interruptPin3, INPUT_PULLUP);
    pinMode(nint1, OUTPUT);
    pinMode(nint2, OUTPUT);
    pinMode(nint3, OUTPUT);
    digitalWrite(nint1, LOW);
    digitalWrite(nint2, LOW);
    digitalWrite(nint3, LOW);

    attachInterrupt(digitalPinToInterrupt(interruptPin1), lick1, RISING);
    attachInterrupt(digitalPinToInterrupt(interruptPin2), lick2, RISING);
    attachInterrupt(digitalPinToInterrupt(interruptPin3), lick3, RISING);
    pinMode(PIN_RND,OUTPUT);

    int1_lvl = digitalRead(interruptPin1);
    int1_lvl_old = digitalRead(interruptPin1);
    int2_lvl = digitalRead(interruptPin2);
    int2_lvl_old = digitalRead(interruptPin2);
    int3_lvl = digitalRead(interruptPin3);
    int3_lvl_old = digitalRead(interruptPin3);

//    Entropy.initialize();
//    uint32_t Seed = Entropy.random();
//    Serial.print("Seed: ");
//    Serial.println(Seed,HEX);
  //  randomSeed(Seed);
  //do {
   // Rnd = random();
  //} while (!Rnd);                       // get nonzero initial value

    
    
    for (uint8_t t=0; t<8; t++) {
      tcaselect(t);
//      Serial.print("TCA Port #");
//Serial.println(t);

      for (uint8_t addr = 0; addr<=127; addr++) {
        if (addr == TCAADDR) continue;
      
        uint8_t data;
        if (! twi_writeTo(addr, &data, 0, 1, 1)) {
//           Serial.print("Found I2C 0x");
//           Serial.println(addr,HEX);
        }
      }
    }

 //   Serial.println("\ndone");
#ifndef DEBUG

  if (! ledmatrixL.begin(Left_Panel)) {
    Serial.println("IS31 Left not found");
    while (1);
  }
  
  if (! ledmatrixR.begin(Right_Panel)) {
   Serial.println("IS31 Right not found");
    while (1);
  }

  if (! ledmatrixC.begin(Center_Panel)) {
    Serial.println("IS31 Center not found");
    while (1);
  }
#endif



//  Serial.println("IS31s  found!");
  dac.begin(0x62);
  dac.setVoltage(00, false);          
  dac.begin(0x63);
  dac.setVoltage(00, false);          
  setvolume(vol);

initialize_light_panels();

//  Serial.println("LEDMatrix done");

// for some reason timer3 needs to be initializes this way for the pulse to work immediately
// on the first try

StartPulse(100,RewardL);
delay(1);
StartPulse(100,RewardL);
delay(1);
StartPulse(100,RewardL);
delay(1);

StartPulse(100,RewardC);
delay(1);
StartPulse(100,RewardC);
delay(1);
StartPulse(100,RewardC);
delay(1);

StartPulse(100,RewardR);
delay(1);
StartPulse(100,RewardR);
delay(1);
StartPulse(100,RewardR);
delay(1);

//Timer3.initialize(100);
//Timer3.attachInterrupt(StopPulse);
//Timer3.start();
reward_pulse_L = 10000;
reward_pulse_C = 10000;
reward_pulse_R = 10000;
airpuff_pulse = 10000;   


  pinMode(light0, INPUT);
  pinMode(light1, INPUT);
  pinMode(light2, INPUT);
  pinMode(light3, INPUT);
  pinMode(light4, INPUT);
  pinMode(light5, INPUT);
  pinMode(light6, INPUT);
  pinMode(light7, INPUT);

  pinMode(Rew_L, INPUT);
  pinMode(Rew_C, INPUT);
  pinMode(Rew_R, INPUT);

  digitalWrite(reset_indicator, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(12);
  digitalWrite(reset_indicator, LOW);   // turn the LED on (HIGH is the voltage level)
  oldvol = vol;

  static boolean output = HIGH;
  pinMode(cclockpin, OUTPUT);
  digitalWrite(cclockpin, LOW);

//  MsTimer2::set(16.67, flash); // 500ms period
  MsTimer2::set(20, flash); // 40ms period
  //MsTimer2::start();
  Serial.println("Rset");
  digitalWrite(light_panel_signal_L, LOW);
  digitalWrite(light_panel_signal_R, LOW);

  int_disable_start = millis();
  int_active = 0;
}

// -----------------------------------------------
void loop() {
     digitalWrite(TestOut, HIGH);
    if (vol != oldvol)
    {
      setvolume(vol);
      oldvol = vol;
    }


      temp_time = millis();

      if (number_of_left_cycles > 0)
      {
        temp1 = temp_time - left_cycle_start;
        if (temp1 > left_half_cycle)
        {
            process_left_side_stimulus();
            number_of_left_cycles = number_of_left_cycles - 1;
            left_cycle_start = temp_time;
        }
      }
      
      if (number_of_right_cycles > 0)
      {
        temp1 = temp_time - right_cycle_start;
        if (temp1 > right_half_cycle)
        {
            process_right_side_stimulus();
            number_of_right_cycles = number_of_right_cycles - 1;
            right_cycle_start = temp_time;
        }
      }
      

//  if lick interrupts not active check if enough time has passed to make them active again
    if (int_active == 0)
    {
      //see how much time has passed since lick interrupts were made inactive
      temp_time = millis();
      temp_time = temp_time - int_disable_start;
      if (temp_time > 500)   // 50 ms
      {
        //  enable lick interrupts
            int_active = 1;
            digitalWrite(TestOut, HIGH);
            // clear interrupt pending bits
            //EIFR = 0x1c;      //  EIFR = (1 << (INTF2 || INTF3 ||INTF3)); 
            EIFR = 0xff;      //  EIFR = (1 << (INTF2 || INTF3 ||INTF3)); 
            attachInterrupt(digitalPinToInterrupt(interruptPin1), lick1, RISING);
            attachInterrupt(digitalPinToInterrupt(interruptPin2), lick2, RISING);
            attachInterrupt(digitalPinToInterrupt(interruptPin3), lick3, RISING);
            digitalWrite(TestOut, LOW);
      }
    }
    digitalWrite(TestOut, LOW);
    
    if ( Serial.available())
    {
    digitalWrite(TestOut, HIGH);
    plse();
    digitalWrite(TestOut, LOW);
    }
     digitalWrite(TestOut, LOW);

    if (noise_active)
    {
      // noise generation
      LowBit = Rnd & 1;
      if (noise_active)
      {
        if (left_right_flag == 0)
          digitalWrite(Tone0,LowBit);         // about 6 us/bit
          else
          digitalWrite(Tone1,LowBit);         // about 6 us/bit
      }
      else
      {
        if (left_right_flag == 0)
          digitalWrite(Tone0,LOW);         // about 6 us/bit
          else
          digitalWrite(Tone1,LOW);         // about 6 us/bit
      }
   
      Rnd >>= 1;
      Rnd ^= LowBit ? 0x80000057ul : 0ul;
    }
    
  lp = lp + 1;  
}

// ------------------------------------------------------
// ------------------------------------------------------
//  act on serial data in
void plse()
        {
           char inChar = (char)Serial.read();

//-----------------------------------------------
// left/right flag and panel mode
            if (inChar == '?')
            {
              while (!Serial.available()) {}
              left_right_flag = Serial.read();
              return;
            }
//-----------------------------------------------
// light_pattern_speed
            if (inChar == '{')
            {
              while (!Serial.available()) {}
              light_pattern_speed = Serial.read();
              return;
            }
//-----------------------------------------------
// light_pattern_brightness
            if (inChar == '}')
            {
              while (!Serial.available()) {}
              light_pattern_brightness = Serial.read();
              while (!Serial.available()) {}
              temp = Serial.read();
              // if 0 then do not initialize light panels
              if (temp == 1)
                 initialize_light_panels();
              return;
            }
//-----------------------------------------------
// Sensory Stimulus panel mode
            if (inChar == '*')
            {
              while (!Serial.available()) {}
              panel_mode = Serial.read();   // 
              write_panel_pageL(0,15,0,9,4,light_pattern_brightness); //full panel
              write_panel_pageL(0,3,ystart,ystop,5,light_pattern_brightness);    // for one panel case
              write_panel_pageR(0,15,0,9,4,light_pattern_brightness); //full panel
              write_panel_pageR(0,3,ystart,ystop,5,light_pattern_brightness);    // for one panel case
              write_panel_pageC(0,15,0,9,4,light_pattern_brightness); //full panel
              write_panel_pageC(0,3,ystart,ystop,5,light_pattern_brightness);    // for one panel case
              
              while (!Serial.available()) {}
              temp = Serial.read();   // number of bursts
              while (!Serial.available()) {}
              temp1 = Serial.read();   // on time
              while (!Serial.available()) {}
              temp2 = Serial.read();   //off time
              while (!Serial.available()) {}
              left_right_flag = Serial.read();   //left right flag


              digitalWrite(TestOut,HIGH);  
              for (uint8_t count=0; count<temp; count++)
              {
              if (left_right_flag == 0)
                {
                ledmatrixL.setFrame(panel_mode);
                ledmatrixL.displayFrame(panel_mode);
                delay(temp1);
                ledmatrixL.setFrame(7);
                ledmatrixL.displayFrame(7);
                delay(temp1);
                }
                else if (left_right_flag == 1)
                {
                ledmatrixR.setFrame(panel_mode);
                ledmatrixR.displayFrame(panel_mode);
                delay(temp1);
                ledmatrixR.setFrame(7);
                ledmatrixR.displayFrame(7);
                delay(temp1);
                }
                else if (left_right_flag == 2)
                {
                ledmatrixC.setFrame(panel_mode);
                ledmatrixC.displayFrame(panel_mode);
                delay(temp1);
                ledmatrixC.setFrame(7);
                ledmatrixC.displayFrame(7);
                delay(temp1);
                }              
                else
                {
                ledmatrixL.setFrame(panel_mode);
                ledmatrixL.displayFrame(panel_mode);
                ledmatrixC.setFrame(panel_mode);
                ledmatrixC.displayFrame(panel_mode);
                ledmatrixR.setFrame(panel_mode);
                ledmatrixR.displayFrame(panel_mode);
                delay(temp1);
                ledmatrixL.setFrame(7);
                ledmatrixL.displayFrame(7);
                ledmatrixC.setFrame(7);
                ledmatrixC.displayFrame(7);
                ledmatrixR.setFrame(7);
                ledmatrixR.displayFrame(7);
                delay(temp1);
                }              
             // left_right_flag = left_right_flag^temp_byte;
            }
            }
//-----------------------------------------------
//        moving bars

            if (inChar == '(')
            {
            
              while (!Serial.available()) {}
              temp_byte = Serial.read();    
              switch (temp_byte)
              {
                case 1:
                  pattern_left_right_vertical();
                  return;
                case 2:
                  pattern_right_left_vertical();
                  return;
                case 3:
                  pattern_top_to_bottom_horizontal();
                  return;
                case 4:
                  pattern_bottom_to_top_horizontal();
                  return;
                case 5:
                  pattern_left_right_vertical_db();
                  return;
                case 6:
                  pattern_right_left_vertical_db();
                  return;
                case 7:
                  pattern_top_to_bottom_horizontal_db();
                  return;
                case 8:
                  pattern_bottom_to_top_horizontal_db();
                  return;
                return;
              }
            }
        

//-----------------------------------------------
// move arm1
            if (inChar == 'i')
            {
              dac.begin(0x63);
              while (!Serial.available()) {}
              temp = 100*Serial.read();            
              dac.setVoltage(temp, false);          
              return;
            }
//-----------------------------------------------
// move arm2
            if (inChar == 'j')
            {
              dac.begin(0x62);
              while (!Serial.available()) {}
              temp = 100*Serial.read();            
              dac.setVoltage(temp, false);          
              return; 
            }
//-----------------------------------------------
// reward L
            if (inChar == 'y')
            {
            digitalWrite(RewardL, HIGH);
            pinMode(Rew_L, OUTPUT);
            digitalWrite(Rew_L, HIGH);
            StartPulse(reward_pulse_L,RewardL);
            return;
            }
//-----------------------------------------------
// reward C
            if (inChar == 'z')
            {
              digitalWrite(RewardC, HIGH);
              pinMode(Rew_C, OUTPUT);
              digitalWrite(Rew_C, HIGH);
              StartPulse(reward_pulse_C,RewardC);
              return;
            }
//-----------------------------------------------
// reward R
            if (inChar == 'x')
            {
              digitalWrite(RewardR, HIGH);
              pinMode(Rew_R, OUTPUT);
              digitalWrite(Rew_R, HIGH);
              StartPulse(reward_pulse_R,RewardR);
              return;
            }
           
//-----------------------------------------------
// light panel L
            if (inChar == 'r')
            {
              while (!Serial.available()){}
              temp_byte = Serial.read();
              light_panel_L(temp_byte);
              return;
            }
//-----------------------------------------------
// light panel C
            if (inChar == 's')
            {
              while (!Serial.available()){}
              temp_byte = Serial.read();
              light_panel_C(temp_byte);
              return;
            }
//-----------------------------------------------
// light panel R
            if (inChar == 't')
            {
              while (!Serial.available()){}
              temp_byte = Serial.read();
              light_panel_R(temp_byte);
              return;
            }
//-----------------------------------------------
//  tone0 on/off     1=on   0 = off
            if (inChar == 'e')
            {
              while (!Serial.available()) {}
              temp = Serial.read();
              if (temp == 1)
              {
                setvolume(vol);
                //tone(Tone0,tonefreq);
                tone_ON(0,tonefreq);
              }
              else
              {
                setvolume(0);
//                noTone(Tone0);
                tone_OFF(0);
                digitalWrite(Tone0, LOW);                 
              }
              return;
            }
//-----------------------------------------------
//  tone1 on/off     1=on   0 = off
            if (inChar == 'f')
            {
              while (!Serial.available()) {}
              temp = Serial.read();
              if (temp == 1)
              {
                setvolume(vol);
//                tone(Tone1,tonefreq);
                tone_ON(1,tonefreq);
              }
              else
              {
                setvolume(0);
//                noTone(Tone1);
                tone_OFF(1);
                digitalWrite(Tone1, LOW);                 
              }
              return;
            }
//-----------------------------------------------
// AirPuff
            if (inChar == 'd')
            {
              digitalWrite(AirPuff, HIGH);
              StartPulse(airpuff_pulse,AirPuff);
              digitalWrite(AirPuff, LOW);
              return;
            }
//-----------------------------------------------
// airpuff duration 
            if (inChar == 'u')
            {
              while (!Serial.available()) {}
              tre = Serial.read();
              airpuff_pulse = 1000*tre;
              return;
            }
//-----------------------------------------------
// reward duration Center
            if (inChar == 'q')
            {
              while (!Serial.available()) {}
              tre = Serial.read();
              reward_pulse_C = 1000*tre;
              return;
            }
//-----------------------------------------------
// reward duration Left
            if (inChar == 'v')
            {
              while (!Serial.available()) {}
              tre = Serial.read();
              reward_pulse_L = 1000*tre;
              return;
             }
//-----------------------------------------------
// reward duration Right
            if (inChar == 'w')
            {
              while (!Serial.available()) {}
              tre = Serial.read();
              reward_pulse_R = 1000*tre;
              return;
            }
//-----------------------------------------------
// Pulse durations
            if (inChar == '#')
            {
              while (!Serial.available()) {}
              tre = Serial.read();
              reward_pulse_L = 1000*tre;
              while (!Serial.available()) {}
              tre = Serial.read();
              reward_pulse_C = 1000*tre;
              while (!Serial.available()) {}
              tre = Serial.read();
              reward_pulse_R = 1000*tre;
              while (!Serial.available()) {}
              tre = Serial.read();
              airpuff_pulse = 1000*tre;
 //             Serial.println("pulse Initialization Complete");
              return;
            }
//-----------------------------------------------
// enable interrupts
//            if (inChar == 'p')
            if (inChar == '!')
            {
              int_flag1 = 1;
              int_flag2 = 1;
              int_flag3 = 1;
              attachInterrupt(digitalPinToInterrupt(interruptPin1), lick1, RISING);
              attachInterrupt(digitalPinToInterrupt(interruptPin2), lick2, RISING);
              attachInterrupt(digitalPinToInterrupt(interruptPin3), lick3, RISING);
              return;
            }  
        
//-----------------------------------------------
//  noise on 
            if (inChar == '<')
            {
              setvolume(vol);
              noise_active = 1;
              return;
            }  
        
//-----------------------------------------------
//  noise off
            if (inChar == '>')
            {
              setvolume(0);
              noise_active = 0;
              return;
            }  
        
//-----------------------------------------------
// valve control
            if (inChar == 'k')
            {
              while (!Serial.available()) {}
              temp_byte = Serial.read();
              while (!Serial.available()) {}
              temp_byte1 = Serial.read();
//              Serial.println(temp_byte);
 //             Serial.println(temp_byte1);

              
              if (temp_byte == 0)   // 0 is left valve
              {
                if (temp_byte1 == 1)
                  digitalWrite(RewardL, HIGH);
                  else
                  digitalWrite(RewardL, LOW);
              }
              else if (temp_byte == 1)   // 1 is center valve
              {
                if (temp_byte1 == 1)
                  digitalWrite(RewardC, HIGH);
                  else
                  digitalWrite(RewardC, LOW);
              }
              else     // 2 is right valve
              {
                if (temp_byte1 == 1)
                  digitalWrite(RewardR, HIGH);
                  else
                  digitalWrite(RewardR, LOW);
              }
              return;
            }
//-----------------------------------------------
// rate discrimination
          if (inChar == 'b')
          {
            
              while (!Serial.available()) {}
              temp1 = Serial.read();   // 
              while (!Serial.available()) {}
              temp2 = Serial.read();   // 
              left_half_cycle = temp1 + 256*temp2;
              while (!Serial.available()) {}
              temp1 = Serial.read();   // 
              while (!Serial.available()) {}
              temp2 = Serial.read();   // 
              right_half_cycle = temp1 + 256*temp2;
              
              while (!Serial.available()) {}
              number_of_left_cycles = Serial.read();   // 
              while (!Serial.available()) {}
              number_of_right_cycles = Serial.read();   // 
              while (!Serial.available()) {}
              single_panel_L = Serial.read();   // 
              while (!Serial.available()) {}
              single_panel_R = Serial.read();   // 
              left_cycle_start = millis();
              right_cycle_start = millis();
              
              digitalWrite(light_panel_signal_L,HIGH);  
              digitalWrite(light_panel_signal_R,HIGH);  

              if (single_panel_L <2)   // if > 1 then tone mode else light mode
                  {
                  light_tone_flag = 0;
                  if (single_panel_L == 0)
                  {
                     light_panel_L(4);  //  turn on both panels
                     bright_dark_L = 4;
                  }
                  else
                  {
                      light_panel_L(7);  //  dark
                      bright_dark_L = 7;
                  }
    
                  if (single_panel_R == 0)
                  {
                      light_panel_R(4);  //  turn on both panels
                      bright_dark_R = 4;
                  }
                  else
                  {
                      light_panel_R(7);  //  dark
                      bright_dark_R = 7;
                  }
                  }
                  else  // tone mode
                  {
                  light_tone_flag = 1;
//                  tone(Tone0,tonefreq);
//                  tone(Tone1,tonefreq);
                    tone_ON(0,tonefreq);
                    tone_ON(1,tonefreq);
                   bright_dark_L = 4;
                   bright_dark_R = 4;
                  
                    
                  }
          }
//-----------------------------------------------
// noise sequence
          if (inChar == 'l')
          {
              while (!Serial.available()) {}
              noise_tone_flag = Serial.read();   // 
              while (!Serial.available()) {}
              temp = Serial.read();   // number of bursts
              while (!Serial.available()) {}
              temp1 = Serial.read();   // on time
              while (!Serial.available()) {}
              temp2 = Serial.read();   // on time
              ontime = temp1 + 256*temp2;
              while (!Serial.available()) {}
              temp1 = Serial.read();   // on time
              while (!Serial.available()) {}
              temp2 = Serial.read();   // on time
              offtime = temp1 + 256*temp2;

              while (!Serial.available()) {}
              left_right_flag = Serial.read();

              digitalWrite(TestOut,HIGH);  
              for (uint8_t count=0; count<temp; count++)
              {
              if (noise_tone_flag == 1)   //noise
              {
                temp3 = millis();
                temp4 = 0;
                setvolume(vol);
                pinMode(light0, OUTPUT);
                digitalWrite(light0, HIGH);
                while (temp4< ontime)
                {
                LowBit = Rnd & 1;
                Rnd >>= 1;
                Rnd ^= LowBit ? 0x80000057ul : 0ul;
                delay(1);
  
                if (left_right_flag == 1)
                  digitalWrite(Tone1,LowBit);         // about 6 us/bit
                else
                  digitalWrite(Tone0,LowBit);         // about 6 us/bit
                temp4 = millis();
                temp4 = temp4 - temp3;
                }
                setvolume(0);
                digitalWrite(Tone0,LOW);         // about 6 us/bit
                digitalWrite(Tone1,LOW);         // about 6 us/bit
                pinMode(light0, INPUT);
                delay(offtime);
              }
              else   //tone
              {
                setvolume(vol);
                pinMode(light0, OUTPUT);
                digitalWrite(light0, HIGH);
                if (left_right_flag == 1)
                {
  //                tone(Tone1,tonefreq);
                  tone_ON(1,tonefreq);
                }
                else
                {
  //                tone(Tone0,tonefreq);
                  tone_ON(0,tonefreq);
                }
                delay(ontime);
                digitalWrite(Tone1, LOW);                 
                digitalWrite(Tone0, LOW);                 
//                noTone(Tone0);
//                noTone(Tone1);
                tone_OFF(0);
                tone_OFF(1);
                setvolume(0);
                pinMode(light0,INPUT);
                delay(offtime);
              }
          }
              digitalWrite(TestOut,LOW);  

              return;
          }
//-----------------------------------------------
// set audio volume
            if (inChar == 'a')
            {
              while (!Serial.available()) {}
              vol = Serial.read();
              return;
            }
//-----------------------------------------------
// set tone frequency
            if (inChar == '=')
            {
              while (!Serial.available()) {}
              temp1 = Serial.read();
              while (!Serial.available()) {}
              temp2 = Serial.read();
              tonefreq = temp1 + 256*temp2;
              return;
            }
//-----------------------------------------------
// start camera clock
            if (inChar == '(')
            {
              MsTimer2::start();
              return;
            }  
// stop camera clock
            if (inChar == ')')
            {
              MsTimer2::stop();
              digitalWrite(cclockpin, LOW);
              return;
            }  
// ------------------------------------------------------
 
}
//void serialEvent() {

  //            plse();
//}

// ------------------------------------------------------
// ------------------------------------------------------

// Setting the volume is very simple! Just write the 6-bit
// volume to the i2c bus. That's it!
boolean setvolume(int8_t v) {
  // cant be higher than 63 or lower than 0
  if (v > 63) v = 63;
  if (v < 0) v = 0;
  tcaselect(2);
//  Serial.print("Setting volume to ");
//  Serial.println(v);
  Wire.beginTransmission(MAX9744_I2CADDR);
  Wire.write(v);
  if (Wire.endTransmission() == 0) 
    return true;
  else
    return false;
}

// ------------------------------------------------------
// lick 1
void lick1() {
/*
  if (int_flag1 == 1)
  {
    int_flag1 = 0;
    return;
  }
  */
    Serial.print("lckC");
    detachInterrupt(digitalPinToInterrupt(interruptPin1));
    int_disable_start = millis();
    int_active = 0;
}

// ------------------------------------------------------
// lick 2
void lick2() {
/*
  if (int_flag2 == 1)
  {
    int_flag2 = 0;
    return;
  }
  */
    Serial.print("lckR");
    detachInterrupt(digitalPinToInterrupt(interruptPin2));
    int_disable_start = millis();
    int_active = 0;
}
// ------------------------------------------------------
// lick 3
void lick3() {
/*
  if (int_flag3 == 1)
  {
    int_flag3 = 0;
    return;
  }
  */
  
    Serial.print("lckL");
    detachInterrupt(digitalPinToInterrupt(interruptPin3));
    int_disable_start = millis();
    int_active = 0;
}
// ------------------------------------------------------



// ------------------------------------------------------

void write_panel_pageL(uint8_t a,uint8_t b,uint8_t c,uint8_t d,uint8_t idx, uint8_t intensity) {
   
    ledmatrixL.setFrame(idx);
    for (uint8_t x=0; x<16; x++) {
      for (uint8_t y=0; y<9; y++) {
        ledmatrixL.drawPixel(x, y,0 );
      }
    }
    for (uint8_t x=a; x<=b; x++) {
      for (uint8_t y=c; y<=d; y++) {
        ledmatrixL.drawPixel(x, y,intensity);
      }
    }
}


void write_panel_pageR(uint8_t a,uint8_t b,uint8_t c,uint8_t d,uint8_t idx, uint8_t intensity) {


    ledmatrixR.setFrame(idx);
    for (uint8_t x=0; x<16; x++) {
      for (uint8_t y=0; y<9; y++) {
        ledmatrixR.drawPixel(x, y,0 );
      }
    }
    for (uint8_t x=a; x<=b; x++) {
      for (uint8_t y=c; y<=d; y++) {
        ledmatrixR.drawPixel(x, y,intensity);
      }
    }
}


void write_panel_pageC(uint8_t a,uint8_t b,uint8_t c,uint8_t d,uint8_t idx, uint8_t intensity) {

    ledmatrixC.setFrame(idx);
    for (uint8_t x=0; x<16; x++) {
      for (uint8_t y=0; y<9; y++) {
        ledmatrixC.drawPixel(x, y,0 );
      }
    }
    for (uint8_t x=a; x<=b; x++) {
      for (uint8_t y=c; y<=d; y++) {
        ledmatrixC.drawPixel(x, y,intensity);
      }
    }
}


void StartPulse(uint32_t pwidth, uint8_t port) {
  
  Timer3.initialize(pwidth);
  Timer3.attachInterrupt(StopPulse);
  //Timer3.start();
  TCNT1 = 1;
  }

void StopPulse() {
  Timer3.stop();
  Timer3.detachInterrupt();
  digitalWrite(RewardL, LOW);
  digitalWrite(RewardC, LOW);
  digitalWrite(RewardR, LOW);
  pinMode(Rew_L, INPUT);
  pinMode(Rew_C, INPUT);
  pinMode(Rew_R, INPUT);
 }

 void flash()
{
  static boolean output = HIGH;
  
  digitalWrite(cclockpin, output);
  output = !output;
}


void initialize_light_panels()
{
  
  //light_level = 1;
   
  // new 6  panel mode
  // x from a to b,   y from c to d
  
  ////left panel
  write_panel_pageL(0,15,0,9,7,0); //dark
  write_panel_pageL(0,15,0,9,6,0); // small panel
  write_panel_pageL(0,15,0,9,4,light_pattern_brightness); //full panel
  write_panel_pageL(5,9,3,6,3,0); // small panel
  write_panel_pageL(0,15,0,9,2,0); 
  write_panel_pageL(6,9,ystart,ystop,0,light_pattern_brightness); 
  write_panel_pageL(12,15,ystart,ystop,1,light_pattern_brightness); 
  write_panel_pageL(0,3,ystart,ystop,5,light_pattern_brightness);    // for one panel case
  
  //middle panel
  write_panel_pageC(0,15,0,9,7,0); 
  write_panel_pageC(0,15,0,9,6,0); 
  write_panel_pageC(0,15,0,9,5,0); 
  write_panel_pageC(0,15,0,9,4,light_pattern_brightness); //full panel
  write_panel_pageC(0,15,0,9,3,0); 
  write_panel_pageC(0,15,0,9,2,0); 
  write_panel_pageC(2,5,ystart,ystop,0,light_pattern_brightness); 
  write_panel_pageC(10,13,ystart,ystop,1,light_pattern_brightness); 
  
  //right panel
  write_panel_pageR(0,15,0,9,7,0); 
  write_panel_pageR(0,15,0,9,6,0); 
  write_panel_pageR(0,15,0,9,4,light_pattern_brightness); //full panel
  write_panel_pageR(5,9,3,6,3,0); // small panel
  write_panel_pageR(0,15,0,9,2,0); 
  write_panel_pageR(12,15,ystart,ystop,0,light_pattern_brightness); 
  write_panel_pageR(6,9,ystart,ystop,1,light_pattern_brightness); 
  write_panel_pageR(0,3,ystart,ystop,5,light_pattern_brightness);    // for one panel case
  
  
  //show dark pages
  ledmatrixL.displayFrame(7);
  ledmatrixC.displayFrame(7);
  ledmatrixR.displayFrame(7);
 
  //  Serial.println("LEDMatrix done");
}


void light_panel_L(uint8_t tbyte)
{
      if (tbyte == 5)
      {
        pinMode(light0, OUTPUT);
        digitalWrite(light0, HIGH);
      }
      else if (tbyte == 0)
      {
        pinMode(light1, OUTPUT);
        digitalWrite(light1, HIGH);
      }
      else if (tbyte == 1)
      {
        pinMode(light2, OUTPUT);
        digitalWrite(light2, HIGH);
      }
      else     // 7 turn off
      {
        pinMode(light0, INPUT);
        pinMode(light1, INPUT);
        pinMode(light2, INPUT);
        pinMode(light3, INPUT);
        pinMode(light4, INPUT);
        pinMode(light5, INPUT);
        pinMode(light6, INPUT);
        pinMode(light7, INPUT);
      }
      ledmatrixL.displayFrame(tbyte);
      return;
 }



void light_panel_C(uint8_t tbyte)

{
    if (tbyte == 0)
    {
      pinMode(light6, OUTPUT);
      digitalWrite(light6, HIGH);
    }
    else if (tbyte == 1)
    {
      pinMode(light7, OUTPUT);
      digitalWrite(light7, HIGH);
    }
    else    // 7 turn off
    {
      pinMode(light0, INPUT);
      pinMode(light1, INPUT);
      pinMode(light2, INPUT);
      pinMode(light3, INPUT);
      pinMode(light4, INPUT);
      pinMode(light5, INPUT);
      pinMode(light6, INPUT);
      pinMode(light7, INPUT);
  }
  ledmatrixC.displayFrame(tbyte);
  return;
}


//------------------------------------------
void light_panel_R(uint8_t tbyte)
{
  if (tbyte == 5)
  {
    pinMode(light3, OUTPUT);
    digitalWrite(light3, HIGH);
  }
  else if (tbyte == 0)
  {
    pinMode(light4, OUTPUT);
    digitalWrite(light4, HIGH);
  }
  else if (tbyte == 1)
  {
    pinMode(light5, OUTPUT);
    digitalWrite(light5, HIGH);
  }
  else     // 7 turn off
  {
    pinMode(light0, INPUT);
    pinMode(light1, INPUT);
    pinMode(light2, INPUT);
    pinMode(light3, INPUT);
    pinMode(light4, INPUT);
    pinMode(light5, INPUT);
    pinMode(light6, INPUT);
    pinMode(light7, INPUT);
  }
  ledmatrixR.displayFrame(tbyte);
  return;
}




void process_left_side_stimulus()
{
      if (bright_dark_L == 7)
        bright_dark_L = 4;
      else
        bright_dark_L = 7;
        //if single panel the make left always dark if needed
      if (single_panel_L == 1)
            bright_dark_L = 7;
      if (light_tone_flag == 0)            
          light_panel_L(bright_dark_L);  // light mode
       else
        {     // tone mode
          if (bright_dark_L == 7)
          {
                setvolume(0);
//                noTone(Tone0);
                tone_OFF(0);
                digitalWrite(light_panel_signal_L,LOW);  
          }
          else
          {
//                tone(Tone0,tonefreq);
                tone_ON(0,tonefreq);
                digitalWrite(light_panel_signal_L,HIGH);  
          }
      }
}



void process_right_side_stimulus()
{
      if (bright_dark_R == 7)
        bright_dark_R = 4;
      else
        bright_dark_R = 7;

     //if single panel the make right always dark if needed
      if (single_panel_R == 1)
            bright_dark_R = 7;
      if (light_tone_flag == 0)            
          light_panel_R(bright_dark_R);  // light mode
       else
        {     // tone mode
          if (bright_dark_R == 7)
          {
                setvolume(0);
//                noTone(Tone1);
                tone_OFF(1);
                digitalWrite(light_panel_signal_R,LOW);  
          }
          else
          {
  //              tone(Tone1,tonefreq);
                tone_ON(1,tonefreq);
                digitalWrite(light_panel_signal_R,HIGH);  
          }
                
        }
}




//------------------------------------------
void pattern_left_right_vertical()

            {
            digitalWrite(TestOut, HIGH);

            pinMode(light0, OUTPUT);
            digitalWrite(light0, HIGH);

            if (left_right_flag == 0)
            {
              ledmatrixL.setFrame(6);
              for (uint8_t x = 0; x < 16; x++)
              {
                for (uint8_t y = 0; y < 9; y++)
                {
                  ledmatrixL.drawPixel(x, y, light_pattern_brightness);
                }
                  
                ledmatrixL.displayFrame(6);
                delay(light_pattern_speed);
                
                for (uint8_t y = 0; y < 9; y++)
                {
                  ledmatrixL.drawPixel(x, y, 0);
                }
                ledmatrixL.displayFrame(6);

              }
            }
            else if (left_right_flag == 1)
            {
              ledmatrixR.setFrame(6);
              for (uint8_t x = 0; x < 16; x++)
              {
                for (uint8_t y = 0; y < 9; y++)
                {
                  ledmatrixR.drawPixel(x, y, light_pattern_brightness);
                }
                  
                ledmatrixR.displayFrame(6);
                delay(light_pattern_speed);
                
                for (uint8_t y = 0; y < 9; y++)
                {
                  ledmatrixR.drawPixel(x, y, 0);
                }
                ledmatrixR.displayFrame(6);
              }
            }
            else if (left_right_flag == 2)
            {
                ledmatrixC.setFrame(6);
                for (uint8_t x = 0; x < 16; x++)
                {
                  for (uint8_t y = 0; y < 9; y++)
                  {
                    ledmatrixC.drawPixel(x, y, light_pattern_brightness);
                  }
                    
                  ledmatrixC.displayFrame(6);
                  delay(light_pattern_speed);
                  
                  for (uint8_t y = 0; y < 9; y++)
                  {
                    ledmatrixC.drawPixel(x, y, 0);
                  }
                  ledmatrixC.displayFrame(6);
                }
              }
              else      // all panels
              {
                ledmatrixL.setFrame(6);
                ledmatrixC.setFrame(6);
                ledmatrixR.setFrame(6);
                for (uint8_t x = 0; x < 16; x++)
                {
                  for (uint8_t y = 0; y < 9; y++)
                  {
                    ledmatrixL.drawPixel(x, y, light_pattern_brightness);
                    ledmatrixC.drawPixel(x, y, light_pattern_brightness);
                    ledmatrixR.drawPixel(x, y, light_pattern_brightness);
                  }
                    
                  ledmatrixL.displayFrame(6);
                  ledmatrixC.displayFrame(6);
                  ledmatrixR.displayFrame(6);
                  delay(light_pattern_speed);
                  
                  for (uint8_t y = 0; y < 9; y++)
                  {
                    ledmatrixL.drawPixel(x, y, 0);
                    ledmatrixC.drawPixel(x, y, 0);
                    ledmatrixR.drawPixel(x, y, 0);
                  }
                  ledmatrixL.displayFrame(6);
                  ledmatrixC.displayFrame(6);
                  ledmatrixR.displayFrame(6);
                }
              }
            
            pinMode(light0,INPUT);
            digitalWrite(TestOut, LOW);
            return;
            }
//------------------------------------------
void pattern_left_right_vertical_db()

            {
            digitalWrite(TestOut, HIGH);

            pinMode(light0, OUTPUT);
            digitalWrite(light0, HIGH);

            if (left_right_flag == 0)
            {
              ledmatrixL.setFrame(6);
              for (uint8_t x = 0; x < 15; x++)
              {
                for (uint8_t y = 0; y < 9; y++)
                {
                  ledmatrixL.drawPixel(x, y, light_pattern_brightness);
                  ledmatrixL.drawPixel(x+1, y, light_pattern_brightness);
                }
                  
                ledmatrixL.displayFrame(6);
                delay(light_pattern_speed);
                
                for (uint8_t y = 0; y < 9; y++)
                {
                  ledmatrixL.drawPixel(x, y, 0);
                  ledmatrixL.drawPixel(x+1, y, 0);
                }
                ledmatrixL.displayFrame(6);

              }
            }
            else if (left_right_flag == 1)
            {
              ledmatrixR.setFrame(6);
              for (uint8_t x = 0; x < 15; x++)
              {
                for (uint8_t y = 0; y < 9; y++)
                {
                  ledmatrixR.drawPixel(x, y, light_pattern_brightness);
                  ledmatrixR.drawPixel(x+1, y, light_pattern_brightness);
                }
                  
                ledmatrixR.displayFrame(6);
                delay(light_pattern_speed);
                
                for (uint8_t y = 0; y < 9; y++)
                {
                  ledmatrixR.drawPixel(x, y, 0);
                  ledmatrixR.drawPixel(x+1, y, 0);
                }
                ledmatrixR.displayFrame(6);
              }
            }
            else if (left_right_flag == 2)
            {
              ledmatrixC.setFrame(6);
              for (uint8_t x = 0; x < 15; x++)
              {
                for (uint8_t y = 0; y < 9; y++)
                {
                  ledmatrixC.drawPixel(x, y, light_pattern_brightness);
                  ledmatrixC.drawPixel(x+1, y, light_pattern_brightness);
                }
                  
                ledmatrixC.displayFrame(6);
                delay(light_pattern_speed);
                
                for (uint8_t y = 0; y < 9; y++)
                {
                  ledmatrixC.drawPixel(x, y, 0);
                  ledmatrixC.drawPixel(x+1, y, 0);
                }
                ledmatrixC.displayFrame(6);
              }
            }
            else
            {
              ledmatrixL.setFrame(6);
              ledmatrixC.setFrame(6);
              ledmatrixR.setFrame(6);
              for (uint8_t x = 0; x < 15; x++)
              {
                for (uint8_t y = 0; y < 9; y++)
                {
                  ledmatrixL.drawPixel(x, y, light_pattern_brightness);
                  ledmatrixC.drawPixel(x+1, y, light_pattern_brightness);
                  ledmatrixR.drawPixel(x, y, light_pattern_brightness);
                  ledmatrixL.drawPixel(x+1, y, light_pattern_brightness);
                  ledmatrixC.drawPixel(x, y, light_pattern_brightness);
                  ledmatrixR.drawPixel(x+1, y, light_pattern_brightness);
                }
                  
                ledmatrixL.displayFrame(6);
                ledmatrixC.displayFrame(6);
                ledmatrixR.displayFrame(6);
                delay(light_pattern_speed);
                
                for (uint8_t y = 0; y < 9; y++)
                {
                  ledmatrixL.drawPixel(x, y, 0);
                  ledmatrixL.drawPixel(x+1, y, 0);
                  ledmatrixC.drawPixel(x, y, 0);
                  ledmatrixC.drawPixel(x+1, y, 0);
                  ledmatrixR.drawPixel(x, y, 0);
                  ledmatrixR.drawPixel(x+1, y, 0);
                }
                ledmatrixL.displayFrame(6);
                ledmatrixC.displayFrame(6);
                ledmatrixR.displayFrame(6);
              }
            }
            
            pinMode(light0,INPUT);
            digitalWrite(TestOut, LOW);
            return;
            }

//-----------------------------------------------
void pattern_right_left_vertical()

            {
            digitalWrite(TestOut, HIGH);

            pinMode(light1, OUTPUT);
            digitalWrite(light1, HIGH);
            
            if (left_right_flag == 0)
              {
                ledmatrixL.setFrame(6);
                for (uint8_t x = 0; x < 16; x++)
                {
                  for (uint8_t y = 0; y < 9; y++)
                  {
                    ledmatrixL.drawPixel(15-x, y, light_pattern_brightness);
                  }
                    
                  ledmatrixL.displayFrame(6);
                  delay(light_pattern_speed);
                  
                  for (uint8_t y = 0; y < 9; y++)
                  {
                    ledmatrixL.drawPixel(15-x, y, 0);
                  }
                  ledmatrixL.displayFrame(6);
                }
              }
            else if (left_right_flag == 1)
              {
                ledmatrixR.setFrame(6);
                for (uint8_t x = 0; x < 16; x++)
                {
                  for (uint8_t y = 0; y < 9; y++)
                  {
                    ledmatrixR.drawPixel(15-x, y, light_pattern_brightness);
                  }
                    
                  ledmatrixR.displayFrame(6);
                  delay(light_pattern_speed);
                  
                  for (uint8_t y = 0; y < 9; y++)
                  {
                    ledmatrixR.drawPixel(15-x, y, 0);
                  }
                  ledmatrixR.displayFrame(6);
                }
              }
              else if (left_right_flag == 2)
              {
                ledmatrixC.setFrame(6);
                for (uint8_t x = 0; x < 16; x++)
                {
                  for (uint8_t y = 0; y < 9; y++)
                  {
                    ledmatrixC.drawPixel(15-x, y, light_pattern_brightness);
                  }
                    
                  ledmatrixC.displayFrame(6);
                  delay(light_pattern_speed);
                  
                  for (uint8_t y = 0; y < 9; y++)
                  {
                    ledmatrixC.drawPixel(15-x, y, 0);
                  }
                  ledmatrixC.displayFrame(6);
                }
              }
              else
              {
                ledmatrixL.setFrame(6);
                ledmatrixC.setFrame(6);
                ledmatrixR.setFrame(6);
                for (uint8_t x = 0; x < 16; x++)
                {
                  for (uint8_t y = 0; y < 9; y++)
                  {
                    ledmatrixL.drawPixel(15-x, y, light_pattern_brightness);
                    ledmatrixC.drawPixel(15-x, y, light_pattern_brightness);
                    ledmatrixR.drawPixel(15-x, y, light_pattern_brightness);
                  }
                    
                  ledmatrixL.displayFrame(6);
                  ledmatrixC.displayFrame(6);
                  ledmatrixR.displayFrame(6);
                  delay(light_pattern_speed);
                  
                  for (uint8_t y = 0; y < 9; y++)
                  {
                    ledmatrixL.drawPixel(15-x, y, 0);
                    ledmatrixC.drawPixel(15-x, y, 0);
                    ledmatrixR.drawPixel(15-x, y, 0);
                  }
                  ledmatrixL.displayFrame(6);
                  ledmatrixC.displayFrame(6);
                  ledmatrixR.displayFrame(6);
                }
              }
              digitalWrite(TestOut, LOW);
              pinMode(light1,INPUT);
             return;
            }
//-----------------------------------------------

void        pattern_right_left_vertical_db()

            {
            digitalWrite(TestOut, HIGH);

            pinMode(light1, OUTPUT);
            digitalWrite(light1, HIGH);
            
            if (left_right_flag == 0)
              {
                ledmatrixL.setFrame(6);
                for (uint8_t x = 0; x < 15; x++)
                {
                  for (uint8_t y = 0; y < 9; y++)
                  {
                    ledmatrixL.drawPixel(15-x, y, light_pattern_brightness);
                    ledmatrixL.drawPixel(14-x, y, light_pattern_brightness);
                  }
                    
                  ledmatrixL.displayFrame(6);
                  delay(light_pattern_speed);
                  
                  for (uint8_t y = 0; y < 9; y++)
                  {
                    ledmatrixL.drawPixel(15-x, y, 0);
                    ledmatrixL.drawPixel(14-x, y, 0);
                  }
                  ledmatrixL.displayFrame(6);
                }
              }
            else if (left_right_flag == 1)
              {
                ledmatrixR.setFrame(6);
                for (uint8_t x = 0; x < 15; x++)
                {
                  for (uint8_t y = 0; y < 9; y++)
                  {
                    ledmatrixR.drawPixel(15-x, y, light_pattern_brightness);
                    ledmatrixR.drawPixel(14-x, y, light_pattern_brightness);
                  }
                    
                  ledmatrixR.displayFrame(6);
                  delay(light_pattern_speed);
                  
                  for (uint8_t y = 0; y < 9; y++)
                  {
                    ledmatrixR.drawPixel(15-x, y, 0);
                    ledmatrixR.drawPixel(14-x, y, 0);
                  }
                  ledmatrixR.displayFrame(6);
                }
              }
              else if (left_right_flag == 2)
              {
                ledmatrixC.setFrame(6);
                for (uint8_t x = 0; x < 15; x++)
                {
                  for (uint8_t y = 0; y < 9; y++)
                  {
                    ledmatrixC.drawPixel(15-x, y, light_pattern_brightness);
                    ledmatrixC.drawPixel(14-x, y, light_pattern_brightness);
                  }
                    
                  ledmatrixC.displayFrame(6);
                  delay(light_pattern_speed);
                  
                  for (uint8_t y = 0; y < 9; y++)
                  {
                    ledmatrixC.drawPixel(15-x, y, 0);
                    ledmatrixC.drawPixel(14-x, y, 0);
                  }
                  ledmatrixC.displayFrame(6);
                }
              }
              else
              {
                ledmatrixL.setFrame(6);
                ledmatrixC.setFrame(6);
                ledmatrixR.setFrame(6);
                for (uint8_t x = 0; x < 15; x++)
                {
                  for (uint8_t y = 0; y < 9; y++)
                  {
                    ledmatrixL.drawPixel(15-x, y, light_pattern_brightness);
                    ledmatrixC.drawPixel(14-x, y, light_pattern_brightness);
                    ledmatrixR.drawPixel(15-x, y, light_pattern_brightness);
                    ledmatrixL.drawPixel(14-x, y, light_pattern_brightness);
                    ledmatrixC.drawPixel(15-x, y, light_pattern_brightness);
                    ledmatrixR.drawPixel(14-x, y, light_pattern_brightness);
                  }
                    
                  ledmatrixL.displayFrame(6);
                  ledmatrixC.displayFrame(6);
                  ledmatrixR.displayFrame(6);
                  delay(light_pattern_speed);
                  
                  for (uint8_t y = 0; y < 9; y++)
                  {
                    ledmatrixL.drawPixel(15-x, y, 0);
                    ledmatrixC.drawPixel(14-x, y, 0);
                    ledmatrixR.drawPixel(15-x, y, 0);
                    ledmatrixL.drawPixel(14-x, y, 0);
                    ledmatrixC.drawPixel(15-x, y, 0);
                    ledmatrixR.drawPixel(14-x, y, 0);
                  }
                  ledmatrixL.displayFrame(6);
                  ledmatrixC.displayFrame(6);
                  ledmatrixR.displayFrame(6);
                }
              }
              digitalWrite(TestOut, LOW);
              pinMode(light1,INPUT);
             return;
            }

//-----------------------------------------------
// pattern top to bottom
void        pattern_top_to_bottom_horizontal()

            {
            digitalWrite(TestOut, HIGH);

            pinMode(light2, OUTPUT);
            digitalWrite(light2, HIGH);
            
            if (left_right_flag == 0)
              {            
                ledmatrixL.setFrame(6);
                for (uint8_t y = 0; y < 9; y++)
                {
                  for (uint8_t x = 0; x < 16; x++)
                  {
                    ledmatrixL.drawPixel(x, y, light_pattern_brightness);
                  }
            
                  ledmatrixL.displayFrame(6);
                  delay(light_pattern_speed);
            
                  for (uint8_t x = 0; x < 16; x++)
                  {
                    ledmatrixL.drawPixel(x, y, 0);
                  }
                  ledmatrixL.displayFrame(6);
                }
              }
            else if (left_right_flag == 1)
              {
                ledmatrixR.setFrame(6);
                for (uint8_t y = 0; y < 9; y++)
                {
                  for (uint8_t x = 0; x < 16; x++)
                  {
                    ledmatrixR.drawPixel(x, y, light_pattern_brightness);
                  }
            
                  ledmatrixR.displayFrame(6);
                  delay(light_pattern_speed);
            
                  for (uint8_t x = 0; x < 16; x++)
                  {
                    ledmatrixR.drawPixel(x, y, 0);
                  }
                  ledmatrixR.displayFrame(6);
                }
              }
              else if (left_right_flag == 2)
              {
                ledmatrixC.setFrame(6);
                for (uint8_t y = 0; y < 9; y++)
                {
                  for (uint8_t x = 0; x < 16; x++)
                  {
                    ledmatrixC.drawPixel(x, y, light_pattern_brightness);
                  }
            
                  ledmatrixC.displayFrame(6);
                  delay(light_pattern_speed);
            
                  for (uint8_t x = 0; x < 16; x++)
                  {
                    ledmatrixC.drawPixel(x, y, 0);
                  }
                  ledmatrixC.displayFrame(6);
                }
              }
              else
              {
                ledmatrixL.setFrame(6);
                ledmatrixC.setFrame(6);
                ledmatrixR.setFrame(6);
                for (uint8_t y = 0; y < 9; y++)
                {
                  for (uint8_t x = 0; x < 16; x++)
                  {
                    ledmatrixL.drawPixel(x, y, light_pattern_brightness);
                    ledmatrixC.drawPixel(x, y, light_pattern_brightness);
                    ledmatrixR.drawPixel(x, y, light_pattern_brightness);
                  }
            
                  ledmatrixL.displayFrame(6);
                  ledmatrixC.displayFrame(6);
                  ledmatrixR.displayFrame(6);
                  delay(light_pattern_speed);
            
                  for (uint8_t x = 0; x < 16; x++)
                  {
                    ledmatrixL.drawPixel(x, y, 0);
                    ledmatrixC.drawPixel(x, y, 0);
                    ledmatrixR.drawPixel(x, y, 0);
                  }
                  ledmatrixL.displayFrame(6);
                  ledmatrixC.displayFrame(6);
                  ledmatrixR.displayFrame(6);
                }
              }
            
             digitalWrite(TestOut, LOW);
             pinMode(light2, INPUT);
             return;
            }
//-----------------------------------------------
// pattern top to bottom double bars
void        pattern_top_to_bottom_horizontal_db()

            {
            digitalWrite(TestOut, HIGH);

            pinMode(light2, OUTPUT);
            digitalWrite(light2, HIGH);
            
            if (left_right_flag == 0)
              {            
                ledmatrixL.setFrame(6);
                for (uint8_t y = 0; y < 8; y++)
                {
                  for (uint8_t x = 0; x < 16; x++)
                  {
                    ledmatrixL.drawPixel(x, y, light_pattern_brightness);
                    ledmatrixL.drawPixel(x, y+1, light_pattern_brightness);
                  }
            
                  ledmatrixL.displayFrame(6);
                  delay(light_pattern_speed);
            
                  for (uint8_t x = 0; x < 16; x++)
                  {
                    ledmatrixL.drawPixel(x, y, 0);
                    ledmatrixL.drawPixel(x, y+1, 0);
                  }
                  ledmatrixL.displayFrame(6);
                }
              }
            else if (left_right_flag == 1)
              {
                ledmatrixR.setFrame(6);
                for (uint8_t y = 0; y < 8; y++)
                {
                  for (uint8_t x = 0; x < 16; x++)
                  {
                    ledmatrixR.drawPixel(x, y, light_pattern_brightness);
                    ledmatrixR.drawPixel(x, y+1, light_pattern_brightness);
                  }
            
                  ledmatrixR.displayFrame(6);
                  delay(light_pattern_speed);
            
                  for (uint8_t x = 0; x < 16; x++)
                  {
                    ledmatrixR.drawPixel(x, y, 0);
                    ledmatrixR.drawPixel(x, y+1, 0);
                  }
                  ledmatrixR.displayFrame(6);
                }
              }
              else if (left_right_flag == 2)
              {
                ledmatrixC.setFrame(6);
                for (uint8_t y = 0; y < 8; y++)
                {
                  for (uint8_t x = 0; x < 16; x++)
                  {
                    ledmatrixC.drawPixel(x, y, light_pattern_brightness);
                    ledmatrixC.drawPixel(x, y+1, light_pattern_brightness);
                  }
            
                  ledmatrixC.displayFrame(6);
                  delay(light_pattern_speed);
            
                  for (uint8_t x = 0; x < 16; x++)
                  {
                    ledmatrixC.drawPixel(x, y, 0);
                    ledmatrixC.drawPixel(x, y+1, 0);
                  }
                  ledmatrixC.displayFrame(6);
                }
              }
              else
              {
                ledmatrixL.setFrame(6);
                ledmatrixC.setFrame(6);
                ledmatrixR.setFrame(6);
                for (uint8_t y = 0; y < 8; y++)
                {
                  for (uint8_t x = 0; x < 16; x++)
                  {
                    ledmatrixL.drawPixel(x, y, light_pattern_brightness);
                    ledmatrixC.drawPixel(x, y+1, light_pattern_brightness);
                    ledmatrixR.drawPixel(x, y, light_pattern_brightness);
                    ledmatrixL.drawPixel(x, y+1, light_pattern_brightness);
                    ledmatrixC.drawPixel(x, y, light_pattern_brightness);
                    ledmatrixR.drawPixel(x, y+1, light_pattern_brightness);
                  }
            
                  ledmatrixL.displayFrame(6);
                  ledmatrixC.displayFrame(6);
                  ledmatrixR.displayFrame(6);
                  delay(light_pattern_speed);
            
                  for (uint8_t x = 0; x < 16; x++)
                  {
                    ledmatrixL.drawPixel(x, y, 0);
                    ledmatrixC.drawPixel(x, y+1, 0);
                    ledmatrixR.drawPixel(x, y, 0);
                    ledmatrixL.drawPixel(x, y+1, 0);
                    ledmatrixC.drawPixel(x, y, 0);
                    ledmatrixR.drawPixel(x, y+1, 0);
                  }
                  ledmatrixL.displayFrame(6);
                  ledmatrixC.displayFrame(6);
                  ledmatrixR.displayFrame(6);
                }
              }
            
             digitalWrite(TestOut, LOW);
             pinMode(light2, INPUT);
             return;
            }


//-----------------------------------------------
// pattern bottom to top 
void       pattern_bottom_to_top_horizontal()

            {
            digitalWrite(TestOut, HIGH);

            pinMode(light3, OUTPUT);
            digitalWrite(light3, HIGH);
            
            if (left_right_flag == 0)
            {
              ledmatrixL.setFrame(6);
              for (uint8_t y = 0; y < 9; y++)
              {
                for (uint8_t x = 0; x < 16; x++)
                {
                  ledmatrixL.drawPixel(x, 9-y, light_pattern_brightness);
                }
          
                ledmatrixL.displayFrame(6);
                delay(light_pattern_speed);
          
                for (uint8_t x = 0; x < 16; x++)
                {
                  ledmatrixL.drawPixel(x, 9-y, 0);
                }
                ledmatrixL.displayFrame(6);
              }
            }
            else if (left_right_flag == 1)
            {
              ledmatrixR.setFrame(6);
              for (uint8_t y = 0; y < 9; y++)
              {
                for (uint8_t x = 0; x < 16; x++)
                {
                  ledmatrixR.drawPixel(x, 9-y, light_pattern_brightness);
                }
          
                ledmatrixR.displayFrame(6);
                delay(light_pattern_speed);
          
                for (uint8_t x = 0; x < 16; x++)
                {
                  ledmatrixR.drawPixel(x, 9-y, 0);
                }
                ledmatrixR.displayFrame(6);
              }
            } 
            else if (left_right_flag == 2)
            {
              ledmatrixC.setFrame(6);
              for (uint8_t y = 0; y < 9; y++)
              {
                for (uint8_t x = 0; x < 16; x++)
                {
                  ledmatrixC.drawPixel(x, 9-y, light_pattern_brightness);
                }
          
                ledmatrixC.displayFrame(6);
                delay(light_pattern_speed);
          
                for (uint8_t x = 0; x < 16; x++)
                {
                  ledmatrixC.drawPixel(x, 9-y, 0);
                }
                ledmatrixC.displayFrame(6);
              }
            } 
            else
            {
              ledmatrixL.setFrame(6);
              ledmatrixC.setFrame(6);
              ledmatrixR.setFrame(6);
              for (uint8_t y = 0; y < 9; y++)
              {
                for (uint8_t x = 0; x < 16; x++)
                {
                  ledmatrixL.drawPixel(x, 9-y, light_pattern_brightness);
                  ledmatrixC.drawPixel(x, 9-y, light_pattern_brightness);
                  ledmatrixR.drawPixel(x, 9-y, light_pattern_brightness);
                }
          
                ledmatrixL.displayFrame(6);
                ledmatrixC.displayFrame(6);
                ledmatrixR.displayFrame(6);
                delay(light_pattern_speed);
          
                for (uint8_t x = 0; x < 16; x++)
                {
                  ledmatrixL.drawPixel(x, 9-y, 0);
                  ledmatrixC.drawPixel(x, 9-y, 0);
                  ledmatrixR.drawPixel(x, 9-y, 0);
                }
                ledmatrixL.displayFrame(6);
                ledmatrixC.displayFrame(6);
                ledmatrixR.displayFrame(6);
              }
            } 
             digitalWrite(TestOut, LOW);
             pinMode(light3, INPUT);
             return;
            }
//-----------------------------------------------
// pattern bottom to top double bars
void       pattern_bottom_to_top_horizontal_db()

            {
            digitalWrite(TestOut, HIGH);

            pinMode(light3, OUTPUT);
            digitalWrite(light3, HIGH);
            
            if (left_right_flag == 0)
            {
              ledmatrixL.setFrame(6);
              for (uint8_t y = 0; y < 8; y++)
              {
                for (uint8_t x = 0; x < 16; x++)
                {
                  ledmatrixL.drawPixel(x, 9-y, light_pattern_brightness);
                  ledmatrixL.drawPixel(x, 8-y, light_pattern_brightness);
                }
          
                ledmatrixL.displayFrame(6);
                delay(light_pattern_speed);
          
                for (uint8_t x = 0; x < 16; x++)
                {
                  ledmatrixL.drawPixel(x, 9-y, 0);
                  ledmatrixL.drawPixel(x, 8-y, 0);
                }
                ledmatrixL.displayFrame(6);
              }
            }
            else if (left_right_flag == 1)
            {
              ledmatrixR.setFrame(6);
              for (uint8_t y = 0; y < 8; y++)
              {
                for (uint8_t x = 0; x < 16; x++)
                {
                  ledmatrixR.drawPixel(x, 9-y, light_pattern_brightness);
                  ledmatrixR.drawPixel(x, 8-y, light_pattern_brightness);
                }
          
                ledmatrixR.displayFrame(6);
                delay(light_pattern_speed);
          
                for (uint8_t x = 0; x < 16; x++)
                {
                  ledmatrixR.drawPixel(x, 9-y, 0);
                  ledmatrixR.drawPixel(x, 8-y, 0);
                }
                ledmatrixR.displayFrame(6);
              }
            } 
            else if (left_right_flag == 2)
            {
              ledmatrixC.setFrame(6);
              for (uint8_t y = 0; y < 8; y++)
              {
                for (uint8_t x = 0; x < 16; x++)
                {
                  ledmatrixC.drawPixel(x, 9-y, light_pattern_brightness);
                  ledmatrixC.drawPixel(x, 8-y, light_pattern_brightness);
                }
          
                ledmatrixC.displayFrame(6);
                delay(light_pattern_speed);
          
                for (uint8_t x = 0; x < 16; x++)
                {
                  ledmatrixC.drawPixel(x, 9-y, 0);
                  ledmatrixC.drawPixel(x, 8-y, 0);
                }
                ledmatrixC.displayFrame(6);
              }
            } 
            else
            {
              ledmatrixL.setFrame(6);
              ledmatrixC.setFrame(6);
              ledmatrixR.setFrame(6);
              for (uint8_t y = 0; y < 8; y++)
              {
                for (uint8_t x = 0; x < 16; x++)
                {
                  ledmatrixL.drawPixel(x, 9-y, light_pattern_brightness);
                  ledmatrixC.drawPixel(x, 8-y, light_pattern_brightness);
                  ledmatrixR.drawPixel(x, 9-y, light_pattern_brightness);
                  ledmatrixL.drawPixel(x, 8-y, light_pattern_brightness);
                  ledmatrixC.drawPixel(x, 9-y, light_pattern_brightness);
                  ledmatrixR.drawPixel(x, 8-y, light_pattern_brightness);
                }
          
                ledmatrixL.displayFrame(6);
                ledmatrixC.displayFrame(6);
                ledmatrixR.displayFrame(6);
                delay(light_pattern_speed);
          
                for (uint8_t x = 0; x < 16; x++)
                {
                  ledmatrixL.drawPixel(x, 9-y, 0);
                  ledmatrixC.drawPixel(x, 8-y, 0);
                  ledmatrixR.drawPixel(x, 9-y, 0);
                  ledmatrixL.drawPixel(x, 8-y, 0);
                  ledmatrixC.drawPixel(x, 9-y, 0);
                  ledmatrixR.drawPixel(x, 8-y, 0);
                }
                ledmatrixL.displayFrame(6);
                ledmatrixC.displayFrame(6);
                ledmatrixR.displayFrame(6);
              }
            } 
             digitalWrite(TestOut, LOW);
             pinMode(light3, INPUT);
             return;
            }



void       tone_ON(uint8_t lrflag, uint16_t freq)

{
  if (lrflag == 0)
        tone(Tone0,freq);
          else
        tone(Tone1,freq);
}


void       tone_OFF(uint8_t lrflag)
{
  if (lrflag == 0)
        noTone(Tone0);
          else
        noTone(Tone1);
}




            
