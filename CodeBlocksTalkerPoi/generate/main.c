typedef unsigned char byte;
/*
** adapted from talker.c -- a datagram "client" demo
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
#include <math.h>

#define SERVERPORT "2390"    // the port users will be connecting to
#define CLIENTPORT "2390"

volatile unsigned long millisecs = 0;
unsigned long seconds = 0;
volatile int variable = 0;

//rgbArray is one line of image sent via WiFi
unsigned char rgbArray[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                            1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                            1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                            1, 2, 3, 4, 5, 6
                           };

int counter = 2;
int rows = 36;

// x_array is for image pixels 36x36 compressed RGB, one byte per pixel
byte x_array[36][36]; //using first byte in array to denote position in array
byte col = 0;

byte encoded = 0;

int main(int argc, char *argv[])
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;

    if (argc != 2)
    {
        fprintf(stderr,"usage: ./program ipAddress \n");
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
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            perror("talker: socket");
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "talker: failed to create socket\n");
        return 2;
    }

    while(1)
    {
//set all pixels to 0:
        for(int i = 0; i < 36; i++)
        {
            for(int j = 0; j < 36; j++)
            {
                // drawPixel(i, j, 0, 0, 0)
                x_array[i][j] = 0;
            }
        }

// Switch Case example patterns below:
        switch(counter)
        {
        case 0:
            //Right angle triangle from: http://www.techcrashcourse.com/2016/01/print-right-triangle-star-pattern-in-c.html
            rows = 36;
            for(int i = 0; i < rows; i++)
            {
//       Prints one row of triangle
                for(int j = 0; j < i; ++j)
                {
                    DrawPixel(j, i, i*7, 0, 255-i*7);
                }
//       move to next row
            }
            counter++;
            break;
        case 1:
            for(int i = 17; i < 19; i+=2)
            {
                DrawColouredSquare(i, i, i, 255-i*7, 0, i*7);
                DrawColouredCircle(i, i, i, i*7, 0, 255-i*7);
            }
            counter++;
            break;
//        case 2:
//            for(int i = 14; i < 16; i+=2)
//            {
////      DrawColouredSquare(i+2, i+2, i+2, 255, 255, 0);
//                DrawColouredCircle(i, i, i, 0, 0, 255);
//            }
//            counter++;
//            break;
//        case 3:
////    exponential from: http://www.techcrashcourse.com/2016/01/exponentially-increasing-star-pattern-in-c.html
//            rows = 6;
//            for(int i = 0; i < rows; i++)
//            {
////   Prints one row of pattern
//                for(int j = 0; j < pow(2,i); j++)
//                {
//                    DrawPixel(j, i, 255, 255, 255);
//                }
////     move to next row
//            }
//            counter++;
//            break;
//        case 3:
////    rhombus from : http://www.techcrashcourse.com/2016/01/print-rhombus-star-pattern-in-c.html
//            rows = 20;
//            for(int i = 1; i <= rows; i++)
//            {
////    Print spaces before stars in a row
//                for(int j=i; j<rows; j++)
//                {
//                    DrawPixel(j, i, 0, 0, 0);
//                }
////    Print rows stars after spaces in a row
//                for(int j = 1; j <= rows; j++)
//                {
//                    DrawPixel(j, i, 255, 0, 255);
//                }
////    jump to next row
//            }
//            counter=0;
//            break;
        case 2: //Right andgle Triangle other way
            rows = 36;
            for(int i = 0; i < rows; i++)
            {
//       Prints one row of triangle
                for(int j = 0; j < rows; j++)
                {
                    if(j>rows-i)
                    {
                        DrawPixel(j, i, 0, 0, 0);
                    }
                    else
                    {
                        DrawPixel(j, i, i*7, 0, 255-i*7);
                    }
                }
//       move to next row
            }
            counter=0;
            break;
        default:
            counter = 0;
            break;
        }






////////////////////////////////////send rgb///////////////////////////////////////////////////////

//for(int z = 0; z < 5000; z++){ //send multiple times
        for (int i = 0; i < 36; i++ )
        {
            for(int j = 0; j < 36; j++)
            {
                rgbArray[j] = x_array[i][j]+127;
            }
            sendto(sockfd, rgbArray, 36, 0, p->ai_addr, p->ai_addrlen);

//simulated delay:
            int c = 1, d = 1;

            for ( c = 1 ; c <= 4000 ; c++ )
                for ( d = 1 ; d <= 1500 ; d++ )
                {}
        }
//without the delay, the pixels are sent too fast for poi ESP8266
//send speed varies with the system! How to guarantee send speed?
    }//end while(1)
//}//end send multiple times

    freeaddrinfo(servinfo);
    close(sockfd);
    return 0;
/////////////////////////////////////end send RGB///////////////////////////////////////////////////

}//end main()

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

    if(radius > 6)
    {
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


//DrawPixel: encode pixels into x_array compressed pixel array
void DrawPixel(byte xx, byte yy, byte red, byte green, byte blue)
{
// encode rgb to 1 byte
    encoded = (red & 0xE0) | ((green & 0xE0)>>3) | (blue >> 6);
    //encoded = byteEncode(red, green, blue);
    x_array[yy][xx] = encoded;
//  printf("red value is: %d", red);
}
