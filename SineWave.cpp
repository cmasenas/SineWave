/*
 *	SineWave.cpp - Arduino library for creating a sine wave on the fly.
 *	Created by C. Masenas, November 4, 2015.  cmasenas@alum.rpi.edu
 *
 *  This is free software. You can redistribute it and/or modify it under
 *  the terms of Creative Commons Attribution 3.0 United States License. 
 *  To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/us/ 
 *  or send a letter to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 * 
 *	Attribution must be given to the author for use of the sine generation algorithm.
 *	That's all I ask.
 */
 
#include <TimerOne.h>
#include <SineWave.h>

extern "C" {
void external_compute(void);
void external_compute2(void);
}

void SineWave::begin(){
    Timer1.initialize(_T);  // set sample time for discrete tone signal
    Timer1.pwm(_pin, 0,_T);
       // "external_compute" is an external function that calls a member function
       // this is a workaround because a member function cannot be called directly
       // by an interrupt
    Timer1.attachInterrupt(external_compute);
    Timer1.stop();   // necessary?
}

void SineWave::setInterval(int T){
    _T = T ;
    this->begin();
}

void SineWave::setPin(int pin){
	_pin = pin ;
    this->begin();
}
   
void SineWave::playTone(int freq, int duration){     
    omega = 2*pi*freq ;
    c1 = (8.0 - 2.0*pow(omega*_T/1000000.0,2))/(4.0+pow(omega*_T/1000000.0,2));   
    a[0] = 0.0 ;
    a[1] = A*sin(omega*_T/1000000.0);
    a[2] = 0.0 ;
    Timer1.attachInterrupt(external_compute);
      Timer1.restart();
      delay(duration);
      Timer1.stop(); 
      Timer1.detachInterrupt();
   }

   void SineWave::playTone2(int freq, int freq2, int duration){     
      omega = 2*pi*freq ;
      omega2 = 2*pi*freq2 ;
      c1 = (8.0 - 2.0*pow(omega*_T/1000000.0,2))/(4.0+pow(omega*_T/1000000.0,2));   
      c1b = (8.0 - 2.0*pow(omega2*_T/1000000.0,2))/(4.0+pow(omega2*_T/1000000.0,2)); 
      a[0] = 0.0 ;
      a[1] = A/2*sin(omega*_T/1000000.0);
      a[2] = 0.0 ;
      b[0] = 0.0 ;
      b[1] = A/2*sin(omega*_T/1000000.0);
      b[2] = 0.0 ;
      Timer1.attachInterrupt(external_compute2);
      Timer1.restart();
      delay(duration);
      Timer1.stop(); 
      Timer1.detachInterrupt();
   }

   void SineWave::playTone(int freq){
      omega = 2*pi*freq ;
      c1 = (8.0 - 2.0*pow(omega*_T/1000000.0,2))/(4.0+pow(omega*_T/1000000.0,2));   
      a[0] = 0.0 ;
      a[1] = A*sin(omega*_T/1000000.0);
      a[2] = 0.0 ;
      Timer1.attachInterrupt(external_compute);
      Timer1.restart();
   }

     void SineWave::playTone2(int freq, int freq2){     
      omega = 2*pi*freq ;
      omega2 = 2*pi*freq2 ;
      c1 = (8.0 - 2.0*pow(omega*_T/1000000.0,2))/(4.0+pow(omega*_T/1000000.0,2));   
      c1b = (8.0 - 2.0*pow(omega2*_T/1000000.0,2))/(4.0+pow(omega2*_T/1000000.0,2)); 
      a[0] = 0.0 ;
      a[1] = A/2*sin(omega*_T/1000000.0);
      a[2] = 0.0 ;
      b[0] = 0.0 ;
      b[1] = A/2*sin(omega*_T/1000000.0);
      b[2] = 0.0 ;
      Timer1.attachInterrupt(external_compute2);
      Timer1.restart();
   }

   void SineWave::stopTone(void){
    Timer1.stop();
    Timer1.detachInterrupt();
   }
   
   void SineWave::compute(void){
     a[2] = c1*a[1] - a[0] ;
     a[0] = a[1] ;
     a[1] = a[2] ; 
     Timer1.setPwmDuty(_pin, a[2]+512);
   } 

   void SineWave::compute2(void){
     a[2] = c1*a[1] - a[0] ;
     a[0] = a[1] ;
     a[1] = a[2] ; 
     b[2] = c1b*b[1] - b[0] ;  
     b[0] = b[1] ;
     b[1] = b[2] ; 
     Timer1.setPwmDuty(_pin, a[2]+b[2]+512);
   }  
   
extern SineWave sw;

void external_compute(void){
  sw.compute();
}

void external_compute2(void){
  sw.compute2();
}  



