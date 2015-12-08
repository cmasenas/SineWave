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
 
// The minimum sampling delay is about 60 microseconds, this allows the "playToneDecay"
// function sufficient time to be calculated.  Below this time interval the results are erratic.  
 
#include <SineWave.h>
#include <TimerOne.h>

extern "C" {
void external_compute(void);
void external_compute2(void);
void external_compute_decay(void);
}


void SineWave::setInterval(float T){
	Timer1.initialize(T);
    _T = T/1000000.0 ;
}

void SineWave::setPin(int pin){
	_pin = pin ;
}

	
   void SineWave::playTone(float freq){
      float omega = 2.0*pi*freq ;  // angular frequency in radians/second
      float wTsq = _T*_T*omega*omega ;	// omega * sampling frequency squared
      c1 = (8.0 - 2.0*wTsq)/(4.0+wTsq);  // coefficient of first filter term 
      a[0] = 0.0 ;									// initialize filter coefficients	
      a[1] = A*sin(omega*_T);
      a[2] = 0.0 ; 
// "external_compute" is an external function that calls a member function
// this is a workaround because a member function cannot be called directly
// by an interrupt 	
      Timer1.attachInterrupt(external_compute);
      Timer1.start(); 
   }

     void SineWave::playTone2(float freq, float freq2){     
      float omega = 2*pi*freq ;
      float omega2 = 2*pi*freq2 ;
      float wTsq = _T*_T*omega*omega ;
      float wTsq2 = _T*_T*omega2*omega2 ;
      c1 = (8.0 - 2.0*wTsq)/(4.0+wTsq);   
      c1b = (8.0 - 2.0*wTsq2)/(4.0+wTsq2); 
      a[0] = 0.0 ;
      a[1] = A/2*sin(omega*_T);
      a[2] = 0.0 ;
      b[0] = 0.0 ;
      b[1] = A/2*sin(omega2*_T);
      b[2] = 0.0 ; 
      Timer1.attachInterrupt(external_compute2);
      Timer1.start();
   }
   
   void SineWave::playToneDecay(float freq, float tau){	// tau is in seconds
   	   float c = -( tau < .001 ? 1000 : 1.0/tau );
   	   float omega = 2.0*pi*freq ;
   	   float wTsq = _T*_T*omega*omega ;
   	   float cT = c*_T;		
   	   c1 = 2.0*(4.0 - cT*cT - wTsq)/((2.0-cT)*(2.0-cT) + wTsq) ;
   	   c0 = ((2.0+cT)*(2.0+cT) + wTsq)/((2.0-cT)*(2.0-cT) + wTsq);
   	   a[0] = 0.0 ;									// initialize filter coefficients	
       a[1] = A*sin(omega*_T) ;
       a[2] = 0.0 ;;
       Timer1.attachInterrupt(external_compute_decay);
       Timer1.start();
   	   
   }	   
void SineWave::playTone(float freq, int duration){     
      playTone(freq);
      delay(duration);
      Timer1.stop(); 
      Timer1.detachInterrupt(); 
   }

   void SineWave::playTone2(float freq, float freq2, int duration){     
      playTone2(freq, freq2);      
      delay(duration);
	  Timer1.stop();
      Timer1.detachInterrupt();
   }

   void SineWave::stopTone(void){
    Timer1.stop();
    Timer1.detachInterrupt();
   }
   
   void SineWave::compute(void){
     a[2] = c1*a[1] - a[0] ;
     a[0] = a[1] ;
     a[1] = a[2] ; 
     Timer1.pwm(_pin, a[2]+512);
   } 

   void SineWave::compute2(void){
     a[2] = c1*a[1] - a[0] ;
     a[0] = a[1] ;
     a[1] = a[2] ; 
     b[2] = c1b*b[1] - b[0] ;  
     b[0] = b[1] ;
     b[1] = b[2] ; 
     Timer1.pwm(_pin, a[2]+b[2]+512);
   }  
  
   void SineWave::compute_decay(void){
     a[2] = c1*a[1] - c0*a[0] ;		// compute the sample
     a[0] = a[1] ;					// shift the registers in preparation for the next cycle
     a[1] = a[2] ; 
     Timer1.pwm(_pin, a[2]+512);	// write to output
   } 
      
SineWave sw;

void external_compute(void){
  sw.compute();
}

void external_compute2(void){
  sw.compute2();
}  

void external_compute_decay(void){
  sw.compute_decay();
}  



