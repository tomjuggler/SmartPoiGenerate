typedef unsigned char byte;
/*
** talker.c -- a datagram "client" demo
*/
//OK THIS WORKS!
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include<math.h>

#define SERVERPORT "2390"    // the port users will be connecting to
#define CLIENTPORT "2390"

volatile unsigned long millisecs = 0;
unsigned long seconds = 0;
volatile int variable = 0;

unsigned char rgbArray[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
1, 2, 3, 4, 5, 6};
int counter = 0;

struct dataRGB { byte r;
                 byte g;
                 byte b; };

struct pattern { byte numLeds;
                 byte numFrames;
                 unsigned char *patData; };
  /////////// so need to load this array below from sd card. /////////////////////////////
unsigned char message1Data[] =
{63,	63,	31,	31,	22,	9,	0,	0,	-112,	-3,	-4,	-4,	-40,	68,	64,	-92,	-64,	-32,	-64,	-60,	-92,	96,	100,	-8,	-3,	-4,	-3,	-111,	0,	0,	5,	22,	31,	31,	31,	63,
18,	63,	63,	63,	63,	18,	4,	0,	0,	-112,	-3,	-3,	-7,	-43,	100,	-128,	-60,	-64,	-96,	-92,	-128,	100,	-11,	-3,	-3,	-3,	112,	0,	0,	4,	18,	95,	63,	63,	59,	22,
4,	18,	59,	63,	63,	63,	18,	4,	0,	32,	-112,	-3,	-3,	-3,	-44,	96,	-128,	-60,	-96,	96,	96,	-44,	-3,	-3,	-3,	112,	4,	0,	4,	17,	63,	63,	91,	91,	18,	5,
0,	8,	18,	59,	31,	63,	63,	18,	4,	0,	36,	-112,	-3,	-3,	-7,	-44,	96,	96,	96,	100,	-44,	-3,	-3,	-35,	112,	0,	0,	4,	49,	95,	63,	63,	91,	50,	5,	0,
0,	0,	5,	18,	63,	63,	63,	63,	14,	4,	0,	4,	-112,	-3,	-3,	-3,	-48,	68,	100,	-44,	-3,	-4,	-3,	-112,	0,	0,	4,	17,	63,	63,	63,	63,	46,	5,	0,	0,
1,	0,	0,	5,	18,	59,	63,	31,	95,	18,	4,	0,	0,	-112,	-3,	-3,	-3,	-40,	-76,	-3,	-3,	-3,	112,	0,	0,	4,	18,	95,	63,	27,	63,	18,	5,	0,	0,	1,
2,	2,	1,	0,	0,	50,	95,	63,	31,	63,	18,	4,	0,	0,	-112,	-3,	-4,	-4,	-4,	-4,	-3,	-112,	0,	0,	4,	50,	63,	31,	63,	63,	50,	5,	1,	1,	1,	2,
3,	2,	1,	1,	0,	0,	18,	95,	31,	31,	63,	18,	0,	0,	0,	-80,	-3,	-4,	-4,	-3,	-112,	0,	0,	4,	50,	95,	63,	63,	59,	18,	5,	1,	1,	1,	2,	2,
3,	3,	2,	2,	1,	0,	4,	50,	95,	31,	63,	63,	18,	4,	0,	0,	-80,	-3,	-3,	-112,	0,	0,	4,	50,	95,	63,	63,	59,	50,	5,	0,	1,	2,	2,	3,	3,
3,	3,	3,	2,	2,	1,	0,	4,	18,	95,	63,	31,	63,	18,	4,	0,	36,	-112,	108,	4,	0,	4,	18,	95,	31,	63,	91,	50,	4,	0,	1,	1,	2,	3,	3,	3,
3,	3,	3,	3,	2,	2,	1,	0,	4,	50,	59,	31,	63,	63,	18,	4,	0,	0,	0,	0,	4,	18,	63,	31,	63,	59,	50,	4,	0,	1,	2,	2,	3,	3,	3,	3,
2,	2,	3,	3,	3,	2,	2,	1,	0,	4,	18,	63,	31,	63,	63,	18,	4,	0,	0,	4,	54,	63,	31,	31,	63,	18,	4,	0,	1,	2,	2,	3,	3,	3,	3,	3,
34,	2,	2,	3,	3,	3,	2,	2,	1,	0,	5,	50,	59,	63,	63,	63,	17,	8,	9,	18,	63,	63,	31,	63,	18,	5,	0,	1,	2,	2,	3,	3,	3,	2,	2,	34,
-121,	66,	34,	2,	3,	3,	3,	2,	2,	1,	0,	5,	18,	59,	63,	63,	95,	18,	22,	95,	63,	63,	63,	18,	4,	0,	1,	2,	2,	3,	3,	3,	3,	34,	66,	-121,
-57,	-93,	66,	34,	2,	3,	3,	3,	2,	2,	1,	0,	4,	50,	91,	63,	63,	63,	31,	31,	63,	95,	50,	4,	0,	1,	2,	3,	3,	3,	3,	2,	34,	66,	-125,	-89,
-29,	-61,	-89,	66,	2,	2,	3,	3,	3,	2,	1,	0,	0,	4,	18,	63,	31,	31,	31,	31,	63,	18,	0,	0,	0,	1,	2,	3,	3,	3,	3,	2,	66,	-122,	-57,	-29,
-29,	-29,	-89,	98,	34,	2,	3,	3,	3,	3,	1,	1,	0,	0,	9,	59,	31,	31,	31,	31,	59,	9,	0,	0,	1,	2,	3,	3,	3,	3,	2,	2,	103,	-89,	-61,	-29,
-29,	-61,	-93,	66,	2,	3,	3,	3,	3,	2,	1,	0,	0,	4,	18,	63,	31,	31,	31,	31,	95,	18,	0,	0,	1,	2,	2,	3,	3,	3,	3,	2,	66,	-125,	-61,	-29,
-57,	-93,	98,	34,	2,	3,	3,	3,	2,	2,	1,	0,	4,	18,	63,	63,	63,	63,	63,	63,	27,	95,	18,	4,	0,	0,	1,	2,	3,	3,	3,	3,	34,	98,	-93,	-57,
-89,	98,	34,	2,	3,	3,	3,	2,	2,	1,	0,	5,	50,	91,	63,	63,	59,	18,	22,	59,	63,	63,	95,	18,	4,	0,	0,	1,	3,	3,	3,	3,	2,	34,	98,	-121,
34,	34,	3,	3,	3,	3,	2,	2,	1,	0,	5,	18,	63,	63,	59,	95,	17,	4,	5,	18,	95,	59,	63,	63,	18,	9,	0,	1,	2,	2,	3,	3,	3,	2,	2,	34,
2,	3,	3,	3,	3,	2,	1,	1,	0,	5,	18,	63,	63,	63,	59,	50,	4,	0,	0,	4,	13,	95,	63,	63,	63,	18,	5,	0,	1,	2,	2,	3,	3,	3,	2,	2,
3,	3,	3,	3,	2,	1,	1,	0,	5,	18,	63,	63,	63,	59,	18,	0,	0,	0,	4,	0,	0,	50,	59,	63,	63,	27,	18,	5,	0,	1,	2,	2,	3,	3,	3,	3,
3,	3,	3,	2,	1,	1,	0,	5,	50,	59,	63,	31,	63,	18,	4,	0,	36,	-112,	-112,	0,	0,	4,	50,	63,	31,	63,	63,	18,	4,	0,	1,	2,	3,	3,	3,	3,
3,	3,	2,	1,	0,	0,	5,	50,	63,	63,	63,	95,	17,	4,	0,	4,	-112,	-3,	-3,	-112,	0,	0,	5,	18,	63,	31,	63,	63,	50,	4,	0,	1,	2,	2,	3,	3,
3,	2,	1,	1,	0,	4,	18,	59,	63,	27,	95,	18,	4,	0,	4,	-75,	-3,	-3,	-3,	-3,	-108,	0,	0,	4,	18,	59,	63,	31,	59,	18,	5,	0,	1,	2,	2,	2,
2,	2,	1,	0,	5,	18,	59,	63,	59,	95,	13,	4,	0,	0,	-112,	-3,	-4,	-4,	-4,	-3,	-3,	-108,	0,	0,	5,	18,	63,	63,	63,	59,	50,	5,	0,	1,	1,	2,
1,	1,	0,	4,	18,	59,	63,	63,	91,	18,	4,	0,	36,	-112,	-3,	-3,	-3,	-44,	-40,	-4,	-4,	-3,	-112,	0,	0,	4,	18,	59,	63,	63,	59,	50,	4,	0,	1,	1,
0,	0,	5,	50,	59,	63,	63,	91,	49,	0,	0,	36,	-112,	-3,	-3,	-3,	-48,	64,	100,	-80,	-4,	-4,	-3,	-112,	0,	0,	4,	17,	59,	63,	63,	91,	50,	4,	0,	0,
0,	4,	50,	91,	63,	63,	95,	18,	0,	0,	4,	-112,	-3,	-3,	-7,	-48,	96,	96,	64,	100,	-48,	-4,	-4,	-3,	-112,	36,	0,	4,	18,	59,	63,	63,	91,	18,	4,	0,
9,	18,	63,	63,	63,	91,	50,	4,	0,	4,	-112,	-3,	-4,	-3,	-48,	96,	-128,	-96,	-92,	96,	96,	-43,	-3,	-3,	-4,	-112,	36,	0,	4,	18,	63,	31,	63,	27,	18,	9,
22,	63,	63,	31,	63,	18,	4,	0,	0,	-112,	-3,	-4,	-4,	-44,	100,	-128,	-64,	-64,	-64,	-96,	-128,	96,	-44,	-3,	-4,	-3,	-112,	0,	0,	5,	18,	63,	31,	31,	63,	22,
63,	31,	31,	63,	18,	4,	0,	0,	-112,	-3,	-4,	-4,	-44,	68,	96,	-92,	-64,	-32,	-32,	-64,	-96,	96,	68,	-40,	-4,	-4,	-3,	-111,	0,	0,	4,	22,	31,	31,	31,	63 };
struct pattern message1 = {36, 33,message1Data};

byte x_array[36][36]; //using first byte in array to denote position in array
//3 arrays 1 for each colour?
byte col = 0;

byte encoded = 0;

void setTimeout(int milliseconds)
{
    // If milliseconds is less or equal to 0
    // will be simple return from function without throw error
    if (milliseconds <= 0) {
        fprintf(stderr, "Count milliseconds for timeout is less or equal to 0\n");
        return;
    }

    // a current time of milliseconds
    int milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;

    // needed count milliseconds of return from this timeout
    int end = milliseconds_since + milliseconds;

    // wait while until needed time comes
    do {
        milliseconds_since = clock() * 1000 /
CLOCKS_PER_SEC;
    } while (milliseconds_since <= end);
}


int main(int argc, char *argv[])
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;

    if (argc != 3) {
        fprintf(stderr,"usage: talker hostname message\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0)
{
	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }


    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");
        return 2;
    }
//set all pixels to 0: 
for(int i = 0; i < 36; i++){
  for(int j = 0; j < 36; j++){
    // drawPixel(i, j, 0, 0, 0)
     x_array[i][j] = 0;
   }
  }

// for(int i = 2; i < 16; i+=2){
//    DrawColouredSquare(i+2, i+2, i+2, 255, 255, 0);
//    DrawColouredCircle(i, i, i, 0, 0, 255);
// }

//triangle from: http://www.techcrashcourse.com/2016/01/print-right-triangle-star-pattern-in-c.html
int rows = 36;
for(int i = 1; i <= rows; i++) {
  /* Prints one row of triangle */
  for(int j = 1; j <= i; ++j) {
     DrawPixel(j, i, 255, 0, 0);
  }
  /* move to next row */
 //
}

//rhombus from : http://www.techcrashcourse.com/2016/01/print-rhombus-star-pattern-in-c.html
// int rows = 20;
// for(int i = 1; i <= rows; i++) {  
//   /* Print spaces before stars in a row */ 
//   for(int j=i; j<rows; j++) {  
//     DrawPixel(j, i, 0, 0, 0); 
//   }  

//   /* Print rows stars after spaces in a row */ 
//   for(int j = 1; j <= rows; j++) {  
//     DrawPixel(j, i, 255, 0, 255);  
//   }  

//   /* jump to next row */ 
//   //  
// }  

//exponential from: http://www.techcrashcourse.com/2016/01/exponentially-increasing-star-pattern-in-c.html
// int i,j;
// int rows = 6; 
// for(i = 0; i < rows; i++){
//  /* Prints one row of pattern */
//    for(j = 0; j < pow(2,i); j++){
//     DrawPixel(j, i, 0, 255, 255);
//    }
//    /* move to next row */
// }

for(int z = 0; z < 5000; z++){ //send multiple times


counter = 0;
////////////////////////////////////send rgb/////////////////
//unsigned char  *rgbx;
struct pattern *pat;
pat = &message1;
  //rgbx = pat->patData;
//  for (int j = 0; j < pat->numFrames; j++ ) {
//   for(int i = 0; i < pat->numLeds; i++){
 for (int i = 0; i < 36; i++ ) {
   for(int j = 0; j < 36; j++){
 //      rgbArray[i] = message1Data[counter++]+127;
//rgbArray[i] = 127;
rgbArray[j] = x_array[i][j]+127;
      // printf("%i\n", counter);
 }
  sendto(sockfd, rgbArray, 36, 0, p->ai_addr, p->ai_addrlen);
// for(int j = 0; j < 36; j++) {
        // printf("%d ", rgbArray[j]);
    // }
// printf("\n");
//delay
// setTimeout(4);
//simulated delay:
int c = 1, d = 1;

   for ( c = 1 ; c <= 1000 ; c++ )
       for ( d = 1 ; d <= 1500 ; d++ )
       {}
   }


/////////////////////////////////////////////////////////////
}//end send multiple times
freeaddrinfo(servinfo);
    //printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
    close(sockfd);

    return 0;
}

void DrawColouredSquare(int x0, int y0, int radius, byte r, byte g, byte b)
{
  int x = radius, y = 0;
  int radiusError = 1-x;
 
  while(x >= y)
  {
    DrawPixel(x + x0, y + y0, r, g, b);
    DrawPixel(y + x0, x + y0, r, g, b);
    DrawPixel(-x + x0, y + y0, r, g, b);
    DrawPixel(-y + x0, x + y0, r, g, b);
    DrawPixel(-x + x0, -y + y0, r, g, b);
    DrawPixel(-y + x0, -x + y0, r, g, b);
    DrawPixel(x + x0, -y + y0, r, g, b);
    DrawPixel(y + x0, -x + y0, r, g, b);
    y++;
  //   if (radiusError<0)
  //   {
  //     radiusError += 2 * y + 1;
  //   }
  //   else
  //   {
  //     x--;
  //     radiusError += 2 * (y - x + 1);
  //   }
   }
  //recursion test: from http://natureofcode.com/book/chapter-8-fractals/

  // if(radius > 6){
  //   //x0 *= 0.75f;
  //   //DrawCircle(x0, y0, radius);
    
  //   //DrawCircle(x0 + radius/2, y0, radius/2);
  //   //DrawCircle(x0 - radius/2, y0, radius/2);
  //   //mix it up
  //   DrawColouredSquare(x0 + radius/2, y0, radius/2, g, b, r);
  //   DrawColouredSquare(x0 - radius/2, y0, radius/2, b, r, g);
  //   DrawColouredSquare(x0, y0 + radius/2, radius/2, r, b, g);
  //   DrawColouredSquare(x0, y0 - radius/2, radius/2, g, r, b);
  // }

  //end recursion test
}




void DrawCircle(int x0, int y0, int radius)
{
  int x = radius, y = 0;
  int radiusError = 1-x;
 
  while(x >= y)
  {
    DrawPlainPixel(x + x0, y + y0);
    DrawPlainPixel(y + x0, x + y0);
    DrawPlainPixel(-x + x0, y + y0);
    DrawPlainPixel(-y + x0, x + y0);
    DrawPlainPixel(-x + x0, -y + y0);
    DrawPlainPixel(-y + x0, -x + y0);
    DrawPlainPixel(x + x0, -y + y0);
    DrawPlainPixel(y + x0, -x + y0);
    y++;
    if (radiusError<0)
    {
      radiusError += 2 * y + 1;
    }
    else
    {
      x--;
      radiusError += 2 * (y - x + 1);
    }
  }
  //recursion test: from http://natureofcode.com/book/chapter-8-fractals/

  if(radius > 6){
    //x0 *= 0.75f;
    //DrawCircle(x0, y0, radius);
    
    //DrawCircle(x0 + radius/2, y0, radius/2);
    //DrawCircle(x0 - radius/2, y0, radius/2);
    
    DrawCircle(x0 + radius/2, y0, radius/2);
    DrawCircle(x0 - radius/2, y0, radius/2);
    DrawCircle(x0, y0 + radius/2, radius/2);
    DrawCircle(x0, y0 - radius/2, radius/2);
  }

  //end recursion test
}



void DrawPlainPixel(byte xx, byte yy)
{
  x_array[yy][xx] = 128;
}

void DrawColouredCircle(int x0, int y0, int radius, byte r, byte g, byte b)
{
  int x = radius, y = 0;
  int radiusError = 1-x;
 
  while(x >= y)
  {
    DrawPixel(x + x0, y + y0, r, g, b);
    DrawPixel(y + x0, x + y0, r, g, b);
    DrawPixel(-x + x0, y + y0, r, g, b);
    DrawPixel(-y + x0, x + y0, r, g, b);
    DrawPixel(-x + x0, -y + y0, r, g, b);
    DrawPixel(-y + x0, -x + y0, r, g, b);
    DrawPixel(x + x0, -y + y0, r, g, b);
    DrawPixel(y + x0, -x + y0, r, g, b);
    y++;
    if (radiusError<0)
    {
      radiusError += 2 * y + 1;
    }
    else
    {
      x--;
      radiusError += 2 * (y - x + 1);
    }
  }
  //recursion test: from http://natureofcode.com/book/chapter-8-fractals/
/*
  if(radius > 6){
    //x0 *= 0.75f;
    //DrawCircle(x0, y0, radius);
    
    //DrawCircle(x0 + radius/2, y0, radius/2);
    //DrawCircle(x0 - radius/2, y0, radius/2);
    //mix it up
    DrawColouredCircle(x0 + radius/2, y0, radius/2, g, b, r);
    DrawColouredCircle(x0 - radius/2, y0, radius/2, b, r, g);
    DrawColouredCircle(x0, y0 + radius/2, radius/2, r, b, g);
    DrawColouredCircle(x0, y0 - radius/2, radius/2, g, r, b);
  }
*/
  //end recursion test
}


void DrawColouredPixel(byte xx, byte yy, byte col)
{
  x_array[yy][xx] = col;
}

void DrawBlueCircle(int x0, int y0, int radius)
{
  int x = radius, y = 0;
  int radiusError = 1-x;
 
  while(x >= y)
  {
    DrawPixel(x + x0, y + y0, 0, 0, 255);
    DrawPixel(y + x0, x + y0, 0, 0, 255);
    DrawPixel(-x + x0, y + y0, 0, 0, 255);
    DrawPixel(-y + x0, x + y0, 0, 0, 255);
    DrawPixel(-x + x0, -y + y0, 0, 0, 255);
    DrawPixel(-y + x0, -x + y0, 0, 0, 255);
    DrawPixel(x + x0, -y + y0, 0, 0, 255);
    DrawPixel(y + x0, -x + y0, 0, 0, 255);
    y++;
    if (radiusError<0)
    {
      radiusError += 2 * y + 1;
    }
    else
    {
      x--;
      radiusError += 2 * (y - x + 1);
    }
  }
  //recursion test: from http://natureofcode.com/book/chapter-8-fractals/
/*
  if(radius > 6){
    //x0 *= 0.75f;
    //DrawCircle(x0, y0, radius);
    
    //DrawCircle(x0 + radius/2, y0, radius/2);
    //DrawCircle(x0 - radius/2, y0, radius/2);
    
    DrawCircle(x0 + radius/2, y0, radius/2);
    DrawCircle(x0 - radius/2, y0, radius/2);
    DrawCircle(x0, y0 + radius/2, radius/2);
    DrawCircle(x0, y0 - radius/2, radius/2);
  }
*/
  //end recursion test
}



void DrawPixel(byte xx, byte yy, byte red, byte green, byte blue) //var for colour here?
{
// encode rgb to 1 byte
   encoded = (red & 0xE0) | ((green & 0xE0)>>3) | (blue >> 6);
  //encoded = byteEncode(red, green, blue);
  x_array[yy][xx] = encoded;

}