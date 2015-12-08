/*
 *	SineWave.h - Arduino library for creating a sine wave on the fly.
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
 
#ifndef SineWave_h
#define SineWave_h
 
#include <TimerOne.h>

#define OUTPIN 	9

class SineWave{
  private:
	const float pi = 3.14159 ;
    const float A = 490 ;  
    int _pin = OUTPIN ;
    float _T = 60/1000000.0 ;  
    float c1,  c1b, c0;
    volatile float a[3], b[3] ;

  public:
    void setInterval(float interval);
    void setPin(int pin);
    void playTone(float freq, int duration);
    void playTone(float freq);
    void playTone2(float freq1, float freq2, int duration);
    void playTone2(float freq1, float freq2);
    void playToneDecay(float freq, float tau);		// play a tone with decaying amplitude of time constant tau
    void stopTone(void);
    void compute(void);							// computes a sine tone
    void compute2(void);						// computes two tones simultaneously
    void compute_decay(void);					// computes a decaying sinewave
		
};

extern SineWave sw;
#endif