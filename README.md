# SineWave
Sine wave library for Arduino.  Produces a sine signal and passes the output to a PWM pin.

Features:
<ul>
<li>Produces a sine wave using a high sampling frequency. 
<li>Uses a 50 microsecond sample time as the default.
<li>Uses pin 9 as the default output pin.
<li>Can produce two tones at once.
<li>Can produce very low frequencies (below 1 Hz, low enough to blink an led) by increasing the sample time.
<li>Can produce up to 10 KHz with the default sampling time.
</ul>
