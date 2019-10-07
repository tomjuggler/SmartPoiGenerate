Basic c code to send pixel data to 36px Smart Poi Emulator. 

Why c? The wireless transmission speed increase from using c over Java is significant. 
So much so that this code has to have a delay in order to work. 

Usage: (Linux)
cd to generate folder 
gcc main.c
./main

I am using Code::Blocks if you want a simple IDE to edit and run this easily just open the .cbp project file

For Windows and Mac, I can't help sorry. On Windows I believe cygwin is a great way to run this sort of thing


About the code: 

I have implemented some c code to send image for triangle, rectangle, circle etc 
Different colours are supported. 

A 36x36 px array is generated, then sent line by line over WiFi

You need to connect to the poi emulator first:
see https://www.circusscientist.com/pov-wifi-streaming-emulation/ 

License tbc but basically I am giving this away because someone gave it to me, probably on Stackoverflow...
