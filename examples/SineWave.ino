#include <SineWave.h>
#include <TimerOne.h>

SineWave sw;

void setup()                 
{
  sw.begin();

}

void loop()
{
  for(int i=0; i<10; i++){
    sw.playTone(500+i*10, 500);    
  }
  for(int i=0; i<10; i++){
    sw.playTone2(500, 500+i*10, 500);    
  }
  sw.stopTone();
  sw.setInterval(50);
  sw.playTone2(500, 600);
  delay(1000);
  sw.stopTone();
}


