#define WEBDUINO_FAIL_MESSAGE "NOT ok\n"
#define WEBDUINO_COMMANDS_COUNT 20

#include "SPI.h"
#include "avr/pgmspace.h"
//#include "WebServer.h"
#include "NeoMatrix_WS2801.h"
#include "Adafruit_WS2801.h"
#include "Adafruit_GFX.h"
#include "glcdfont.c"
#include "WiFiEsp.h"

//WiFi Setup
const char ssid[] = "Twim"; // your network SSID (name)
const char pass[] = "12345678"; // your network password
int status = WL_IDLE_STATUS; // the Wifi radio's status
int reqCount = 0; // number of requests received
WiFiEspServer server(80);

// LED Stuff
const uint8_t dataPin = 2; // Yellow wire on Adafruit Pixels
const uint8_t clockPin = 3; // Green wire on Adafruit Pixels

//LED Grid Stuff
const uint16_t max_x = 16;
const uint16_t max_y = 8;

#define STRIPLEN 128
int defaultPattern = 0;
NeoMatrix_WS2801 theMatrix = NeoMatrix_WS2801(max_x, max_y, dataPin, clockPin, \
NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG, WS2801_RGB);

//Matrix Scrolling
unsigned long prevFrameTime = 0L;             // For animation timing
#define FPS 10                                // Scrolling speed
uint8_t       msgLen        = 0;              // Empty message
int           msgX          = 16; // Start off right edge
String writeCharStr = "";

uint32_t aac;
int aayE, aaxE, aay, aax;

// 'graph' style x,y where 0,0 is bottom left

int grid[STRIPLEN] = {
7,8,23,24,39,40,55,56,71,72,87,88,103,104,119,120,
6,9,22,25,38,41,54,57,70,73,86,89,102,105,118,121,
5,10,21,26,37,42,53,58,69,74,85,90,101,106,117,122,
4,11,20,27,36,43,52,59,68,75,84,91,100,107,116,123,
3,12,19,28,35,44,51,60,67,76,83,92,99,108,115,124,
2,13,18,29,34,45,50,61,66,77,82,93,98,109,114,125,
1,14,17,30,33,46,49,62,65,78,81,94,97,110,113,126,
0,15,16,31,32,47,48,63,64,79,80,95,96,111,112,127
};

const uint32_t red[6] = { theMatrix.Color(255,0,0), theMatrix.Color(200,0,0), theMatrix.Color(150,0,0), theMatrix.Color(100,0,0), theMatrix.Color(50,0,0), theMatrix.Color(0,0,0) };
const uint32_t orange[6] = { theMatrix.Color(255,128,0), theMatrix.Color(200,100,0), theMatrix.Color(150,75,0), theMatrix.Color(100,50,0), theMatrix.Color(50,25,0), theMatrix.Color(0,0,0) };
const uint32_t yellow[6] = { theMatrix.Color(255,255,0), theMatrix.Color(200,200,0), theMatrix.Color(150,150,0), theMatrix.Color(100,100,0), theMatrix.Color(50,50,0), theMatrix.Color(0,0,0) };
const uint32_t green[6] = { theMatrix.Color(0,255,0), theMatrix.Color(0,200,0), theMatrix.Color(0,150,0), theMatrix.Color(0,100,0), theMatrix.Color(0,50,0), theMatrix.Color(0,0,0) };
const uint32_t blue[6] = { theMatrix.Color(0,0,255), theMatrix.Color(0,0,200), theMatrix.Color(0,0,150), theMatrix.Color(0,0,100), theMatrix.Color(0,0,50), theMatrix.Color(0,0,0) };
const uint32_t purple[6] = { theMatrix.Color(127,0,255), theMatrix.Color(100,0,200), theMatrix.Color(75,0,150), theMatrix.Color(50,0,100), theMatrix.Color(25,0,50), theMatrix.Color(0,0,0) };

/*** Things you might want to change ***/

/*WebServer webserver("", 80); // port to listen on

// ROM-based messages for webduino lib, maybe overkill here

void printNothing(WebServer &server) { //for snakemove, should respond faster not sending the entire page.
  server.println(F("good"));
}

void printOk(WebServer &server) {
  server.println(F("HTTP/1.1 200 OK"));
  server.println(F("Content-Type: text/html"));
  server.println(F("Connection: close"));
  server.print(F("\r\n"));
  server.println(F("<!DOCTYPE HTML PUBLIC -//W3C//DTD HTML 4.00 TRANSITIONAL//EN><html><head><title>")); //opening html
  server.println(F("DeskLights128</title>")); //title
  server.println(F("<meta name = 'viewport' content = 'width = device-width'> <meta name = 'viewport' content = 'initial-scale = 1.0'> </head>"));
  server.print(F("<script> function process() { var url='write?l=' + document.getElementById('url').value.length + '&c=' + document.getElementById('url').value; location.href=url; return false; } </script>")); //script for form submitting
  server.print(F("<script> function process2() { var urlalert='alert?h=' + document.getElementById('alert').value + '&d=1000'; location.href=urlalert; return false; } </script>")); //script for alert submitting
  server.print(F("<script> function process3() { var urlalert='default?id=' + document.getElementById('default').value; location.href=urlalert; return false; } </script>")); //script for default submitting
  server.print(F("<script> function colortable() { var url='color?h=' + document.getElementById('colortab').value; location.href=url; return false; } </script><script> function colorpixel() { var url='pixel?x=' + document.getElementById('pixx').value + '&y=' + document.getElementById('pixy').value + '&h=' + document.getElementById('pixcolor').value; location.href=url; return false; } </script><script> function colorwipe() { var url='wipe?h=' + document.getElementById('wipecolor').value + '&d=' + document.getElementById('wipedel').value; location.href=url; return false; } </script>"));
  server.println(F("<body>")); //links below here
  server.println(F("<form onSubmit='return process2();'>Send Alert <select id='alert'>")); //form select for Alerts
  server.println(F("<option value='FF0000'>Red</option>")); //red
  server.println(F("<option value='FF6600'>Orange</option>")); //orange
  server.println(F("<option value='FFFF00'>Yellow</option>")); //yellow
  server.println(F("<option value='336600'>Green</option>")); //green
  server.println(F("<option value='003333'>Blue</option>")); //blue
  server.println(F("<option value='660033'>Purple</option>")); //purple
  server.println(F("<input type='submit' value='go'> </select> </form>")); //end select
  server.println(F("<form onSubmit='return colortable();'>Color Table: <select id='colortab'> <option value='FF0000'>Red</option><option value='FF6600'>Orange</option> <option value='FFFF00'>Yellow</option><option value='336600'>Green</option> <option value='003333'>Blue</option><option value='660033'>Purple</option><input type='submit' value='go'> </select> </form>"));
  server.println(F("<form onSubmit='return colorpixel();'>Color Pixel: x=<input type='text' value='1' maxlength='3' size='3' name='pixx' id='pixx' type='number'> y=<input type='text' value='1' maxlength='3' size='3' name='pixy' id='pixy' type='number'><select id='pixcolor'> <option value='FF0000'>Red</option><option value='FF6600'>Orange</option> <option value='FFFF00'>Yellow</option><option value='336600'>Green</option> <option value='003333'>Blue</option><option value='660033'>Purple</option><input type='submit' value='go'> </select> </form>"));
  server.println(F("<form onSubmit='return colorwipe();'>Color Wipe: delay=<input type='text' value='100' maxlength='6' size='6' name='wipedel' id='wipedel' type='number'><select id='wipecolor'> <option value='FF0000'>Red</option><option value='FF6600'>Orange</option> <option value='FFFF00'>Yellow</option><option value='336600'>Green</option> <option value='003333'>Blue</option><option value='660033'>Purple</option><input type='submit' value='go'> </select> </form>"));  server.println(F("<form onSubmit='return process3();'>Run Command <select id='default'>")); //form select for Defaults
  server.println(F("<option value='1'>Rainbow</option>")); //red
  server.println(F("<option value='2'>Random</option>")); //orange
  server.println(F("<option value='3'>K.I.T.T.</option>")); //yellow
  server.println(F("<option value='8'>K.I.T.T. Multi</option>")); //yellow
  server.println(F("<input type='submit' value='go'> </select> </form>")); //end select
  server.println(F("<form onSubmit='return process();'> Write Character: <input type='text' name='url' id='url'> <input type='submit' value='go'> </form>")); //this writes a single character to the board
  server.print(F("<a href='default?id=4'>All Off</a><p></p>"));
  server.println(F("</body></html>")); //end html
}
P(noauth) = "User Denied\n";

// max length of param names and values
#define NAMELEN 2
#define VALUELEN 32
*/
/*** Below here shouldn't need to change ***/

// create a "Color" value from a hex string (no prefix)
// for example: ffffff
uint32_t hexColor(char * in) {
  return strtol(in, NULL, 16);
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return theMatrix.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if (WheelPos < 170) {
    WheelPos -= 85;
    return theMatrix.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return theMatrix.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

// set all pixels to a "Color" value
void colorAll(uint32_t c) {
  Serial.println("Matrix");
  for (int i=0; i < theMatrix.numPixels(); i++) {
    theMatrix.setPixelColor(i, c);
  }
  defaultPattern = 0;
  theMatrix.show();
}
//colorAllDef is just colorAll without the defaultPattern set
void colorAllDef(uint32_t c) {
  for (int i=0; i < theMatrix.numPixels(); i++) {
    theMatrix.setPixelColor(i, c);
  }
  theMatrix.show();
}
// set all pixels to a "Color" value, one at a time, with a delay
void colorWipe(uint32_t c, uint8_t wait) {
  for (int i=0; i < theMatrix.numPixels(); i++) {
    theMatrix.setPixelColor(i, c);
    defaultPattern = 0;
    theMatrix.show();
    delay(wait);
  }
}
// show the grid to verify
void gridTest(int wait) {
  int x;
  int y;
  uint32_t on = theMatrix.Color(255,255,255);
  uint32_t off = theMatrix.Color(0,0,0);

  if (!wait) {
    wait = 250;
  }

  for ( x = 0; x <= max_x; x++) {
    for ( y = 0; y <= max_y; y++) {
      defaultPattern = 0;
      theMatrix.setPixelColor(g2p(x,y), on);
      theMatrix.show();
      delay(wait);
      theMatrix.setPixelColor(g2p(x,y), off);
      theMatrix.show();
    }
  }
}
// random pixel, random color
// short pattern, very responsive
void p_random (int wait) {
  theMatrix.setPixelColor(
  random(0, theMatrix.numPixels()),
  theMatrix.Color(random(0,255), random(0,255), random(0,255))
    );
  theMatrix.show();
  delay(wait);
}

// If you were at maker faire, you know this pattern
// it takes about a second to run, so new requests will wait
void p_rainbow() {
  int i, j;
  for (j=0; j < 256; j++) {
    for (i=0; i < theMatrix.numPixels(); i++) {
      theMatrix.setPixelColor(i, Wheel( ((i * 256 / theMatrix.numPixels()) + j) % 256) );
    }
    theMatrix.show();
  }
}

// cylon or K.I.T.T. whichever 
void p_cylon(const uint32_t c[6]) {
  int x;
  int wait=75;

  for (x=0; x <= max_x; x++) {
    int mod = 0;
    while ((mod < 6) && (x - mod >= 0)) {
      int y = 0;
      while (y <= max_y) {
        theMatrix.setPixelColor(g2p(x-mod,y++), c[mod]);
      }
      mod++;
    }
    theMatrix.show();
    delay(wait);
  }

  for (x=max_x; x >= 0; x--) {
    int mod = 0;
    while ((mod < 6) && (x + mod <= max_x)) {
      int y = 0;
      while (y <= max_y) {
        theMatrix.setPixelColor(g2p(x+mod,y++), c[mod]);
      }
      mod++;
    }
    theMatrix.show();
    delay(wait);
  }

}

//visualizer, takes string of 16 numbers which are Y heights
void vu(String input) {
  uint32_t color = theMatrix.Color(255,0,0);
  for(int i = 0; i<input.length(); i++) {
    int y = input.charAt(i) - '0';
    if(y > max_y) {
      y = max_y;
    }
    if(y < 1) {
      for(y = 1; y<max_y+1; y++) {
        theMatrix.setPixelColor(g2p(i+1,y), theMatrix.Color(0,0,0));
      }
    }
    else {
      int y_orig = y;
      for(y; y>0; y--) {
        if(y>6) color = theMatrix.Color(255,0,0);
        else if(y>3) color = theMatrix.Color(255,128,0);
        else color = theMatrix.Color(255,255,0);
        theMatrix.setPixelColor(g2p(i+1,y), color);
      }
      y = y_orig+1;
      for(y; y<max_y+1; y++) {
        theMatrix.setPixelColor(g2p(i+1,y), theMatrix.Color(0,0,0));
      }
    }
  }
}
/*
void cmd_writechar(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete) {
  int theLength;
  writeCharStr = "";
  URLPARAM_RESULT rc;
  char name[NAMELEN];
  char value[VALUELEN];
  while(strlen(url_tail)) {
    rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
    if((rc != URLPARAM_EOS)) {
      switch(name[0]) {
        case 'l':
           theLength = atoi(value);
           break;
        case 'c':
        for(int i = 0; i<theLength; i++) {
          writeCharStr += (value[i]);
          Serial.write((value[i]));Serial.println();
        }
           break;
      }
    }
  }
  Serial.println(writeCharStr);
  defaultPattern = 7;
  printOk(server);
}

void cmd_show(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete) {
  defaultPattern = 0;
  theMatrix.show();
  printOk(server);
}
void cmd_vu(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete) {
  String inputData;
  
  URLPARAM_RESULT rc;
  char name[NAMELEN];
  char value[VALUELEN];
  while (strlen(url_tail)) {
    rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
    if ((rc != URLPARAM_EOS)) {
      switch(name[0]) {
        case 'v':
        for(int i = 0; i<16; i++) {
          inputData += String(value[i] - '0');
        }
        break;
      }
    }
  }
  vu(inputData);
  theMatrix.show();
  defaultPattern = 0;
  printOk(server);
}

void cmd_pixel(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete) {
  int id;
  int gid;
  int x;
  int y;
  int r;
  int g;
  int b;
  int s = 1;
  uint32_t c;
  int use_hex = 0;
  int use_id = 0;
  int use_gid = 0;

  URLPARAM_RESULT rc;
  char name[NAMELEN];
  char value[VALUELEN];
  while (strlen(url_tail)) {
    rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
    if ((rc != URLPARAM_EOS)) {
      switch(name[0]) {
      case 'i':
        gid = atoi(value);
        use_gid = 1;
        break;
      case 'n':
        id = atoi(value);
        use_id = 1;
        break;
      case 'x':
        x = atoi(value);
        break;
      case 'y':
        y = atoi(value);
        break;
      case 'h':
        c = hexColor(value);
        use_hex = 1;
        break;
      case 'r':
        r = atoi(value);
        break;
      case 'g':
        g = atoi(value);
        break;
      case 'b':
        b = atoi(value);
        break;
      case 's':
        s = atoi(value);
        break;
      }
    }
  }

  if (use_id == 0) {
    if (use_gid != 0) {
      id = grid[gid];
    } 
    else {
      id = g2p(x,y);
    }
  }

  if (use_hex == 0) {
    c = Color(r,g,b);
  }

  theMatrix.setPixelColor(id, c);

  if (s) {
    theMatrix.show();
  }

  defaultPattern = 0;
  printOk(server);
}
*/
// fade from one color to another: UNFINISHED
void fade(uint32_t c1, uint32_t c2, int wait) {
  if (c1 < c2) {
    while (c1 < c2) {
      colorAll(c1++);
      delay(wait);
    }
  } 
  else {
    while (c1 > c2) {
      colorAll(c1--);
      delay(wait);
    }
  }
}

// this takes x/y coordinates and maps it to a pixel offset
// your grid will need to be updated to match your pixel count and layout
uint16_t g2p(uint16_t x, uint16_t y) {
  if(x%2) { // if odd
  return (max_y * x) + y-1-max_y;
  }
  else { //else true, so
  return (max_y * x) + y -1 -max_y + ((max_y - 1)*-1) + 2 * (max_y - y);
  }
}

// flash color "c" for "wait" ms
void alert(uint32_t c, int wait) {
  colorAll(c);
  delay(wait);
  colorAll(theMatrix.Color(0,0,0));
}

// flash color "c" on x/y/c/u as a rectangle
void alertArea(uint32_t c, int x, int y, int xE, int yE) {
  int w = xE-x+1;
  int16_t h = yE-y+1;
  for (int16_t i=x; i<x+w; i++) {
    theMatrix.drawFastVLine(i, y, h, c);
  }
  theMatrix.show();
}



// wipe the major colors through all pixels
void lightTest(int wait) {
  colorWipe(theMatrix.Color(255, 0, 0), wait);
  colorWipe(theMatrix.Color(0, 255, 0), wait);
  colorWipe(theMatrix.Color(0, 0, 255), wait);
  colorWipe(theMatrix.Color(255, 255, 255), wait);
  colorWipe(theMatrix.Color(0, 0, 0), wait);
}
/*
void cmd_index(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete) {
  printOk(server);
}

void my_failCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete) {
  server.httpFail();
}

void cmd_off(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete) {
  colorAll(theMatrix.Color(0,0,0));
  cursor_x = cursor_x_orig;
  cursor_y = cursor_y_orig;
  printOk(server);
}

void cmd_color(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete) {
  int r;
  int g;
  int b;
  uint32_t c;
  int use_hex = 0;

  URLPARAM_RESULT rc;
  char name[NAMELEN];
  char value[VALUELEN];

  while (strlen(url_tail)) {
    rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
    if ((rc != URLPARAM_EOS)) {
      switch(name[0]) {
      case 'h':
        c = hexColor(value);
        use_hex = 1;
        break;
      case 'r':
        r = atoi(value);
        break;
      case 'g':
        g = atoi(value);
        break;
      case 'b':
        b = atoi(value);
        break;
      }
    }
  }

  if (!use_hex) {
    c = Color(r,g,b);
  }
  colorAll(c);
  printOk(server);
}

void cmd_wipe(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete) {
  int r;
  int g;
  int b;
  int delay;
  uint32_t c;
  int use_hex = 0;

  URLPARAM_RESULT rc;
  char name[NAMELEN];
  char value[VALUELEN];

  while (strlen(url_tail)) {
    rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
    if ((rc != URLPARAM_EOS)) {
      switch(name[0]) {
      case 'r':
        r = atoi(value);
        break;
      case 'g':
        g = atoi(value);
        break;
      case 'b':
        b = atoi(value);
        break;
      case 'h':
        c = hexColor(value);
        use_hex++;
        break;
      case 'd':
        delay = atoi(value);
        break;
      }
    }
  }

  if (!use_hex) {
    c = Color(r,g,b);
  }

  printOk(server);
  colorWipe(c, delay);
}

void cmd_default(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete) {
  URLPARAM_RESULT rc;
  char name[NAMELEN];
  char value[VALUELEN];
  while (strlen(url_tail)) {
    rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
    if ((rc != URLPARAM_EOS)) {
      if (name[0] == 'i') {
        defaultPattern = atoi(value);
        colorAllDef(theMatrix.Color(0,0,0));
      }
    }
  }

  printOk(server);
}

void cmd_snakemove(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete) {
  URLPARAM_RESULT rc;
  char name[NAMELEN];
  char value[VALUELEN];
  while (strlen(url_tail)) {
    rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
    if ((rc != URLPARAM_EOS)) {
      if (name[0] == 'i') {
        snakeButton = atoi(value);
        Serial.println(snakeButton);
      }
    }
  }

  printOk(server);
}

void cmd_alert(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete) {
  int r;
  int g;
  int b;
  int d;
  int use_hex = 0;
  uint32_t c;

  URLPARAM_RESULT rc;
  char name[NAMELEN];
  char value[VALUELEN];
  while (strlen(url_tail)) {
    rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
    if ((rc != URLPARAM_EOS)) {
      switch(name[0]) {
      case 'h':
        c = hexColor(value);
        use_hex = 1;
        break;
      case 'r':
        r = atoi(value);
        break;
      case 'g':
        g = atoi(value);
        break;
      case 'b':
        b = atoi(value);
        break;
      case 'd':
        d = atoi(value);
        break;
      }
    }
  }

  if (use_hex == 0) {
    c = Color(r,g,b);
  }

  if (!d) {
    d = 100;
  }

  alert(c, d);
  printOk(server);
}

void cmd_alertArea(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete) {
  int r;
  int g;
  int b;
  int use_hex = 0;
  uint32_t c;
  int x,xE,y,yE;

  URLPARAM_RESULT rc;
  char name[NAMELEN];
  char value[VALUELEN];
  while (strlen(url_tail)) {
    rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
    if ((rc != URLPARAM_EOS)) {
      switch(name[0]) {
      case 'h':
        aac = hexColor(value);
        use_hex = 1;
        break;
      case 'r':
        r = atoi(value);
        break;
      case 'g':
        g = atoi(value);
        break;
      case 'b':
        b = atoi(value);
        break;
      case 'x':
        aax = atoi(value);
        break;
      case 'y':
        aay = atoi(value);
        break;
      case 'c':
        aaxE = atoi(value);
        break;
      case 'u':
        aayE = atoi(value);
        break;
      }
    }
  }

  if (use_hex == 0) {
    aac = Color(r,g,b);
  }

  defaultPattern = 10;
  printOk(server);
}

void cmd_test(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete) {
  URLPARAM_RESULT rc;
  char name[NAMELEN];
  char value[VALUELEN];
  int id;
  int d;
  while (strlen(url_tail)) {
    rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
    if ((rc != URLPARAM_EOS)) {
      switch(name[0]) {
      case 'i':
        id = atoi(value);
        break;
      case 'd':
        d = atoi(value);
        break;
      }
    }
  }

  switch(id) {
  case 0:
    lightTest(d);
    break;
  case 1:
    gridTest(d);
    break;
  }

  printOk(server);
}
*/
// begin standard arduino setup and loop pattern

void setup() {
//  Serial1.begin(9600);
  /*
  webserver.setFailureCommand(&my_failCmd);
  webserver.setDefaultCommand(&cmd_index);
  webserver.addCommand("off", &cmd_off);
  webserver.addCommand("show", &cmd_show);
  webserver.addCommand("wipe", &cmd_wipe);
  webserver.addCommand("color", &cmd_color);
  webserver.addCommand("alert", &cmd_alert);
  webserver.addCommand("pixel", &cmd_pixel);
  webserver.addCommand("default", &cmd_default);
  webserver.addCommand("write", &cmd_writechar);
  webserver.addCommand("test", &cmd_test);
  webserver.addCommand("vu", &cmd_vu);
  webserver.addCommand("snake", &cmd_snakemove);
  webserver.addCommand("alertArea", &cmd_alertArea);
  webserver.begin();*/
  
  theMatrix.begin();
  theMatrix.setCursor(1,1);
  theMatrix.setTextWrap(false);
  
  // light blip of light to signal we are ready to listen
  colorAll(theMatrix.Color(0,0,11));
  delay(500);
  colorAll(theMatrix.Color(0,0,0));
  
  Serial.begin(9600);
}

void loop()
{
  unsigned long t = millis(); // Current elapsed time, milliseconds.
  // listen for connections
 /* char buff[64];
  int len = 64;
  webserver.processConnection(buff, &len);*/
  
  switch(defaultPattern) {
  case 1:
    p_rainbow();
    break;
  case 2:
    p_random(50);
    break;
  case 3:
    p_cylon(blue);
    break;
  case 4:
    colorAll(theMatrix.Color(0,0,0));
   // cursor_x = cursor_x_orig;
   // cursor_y = cursor_y_orig;
    break;
  case 5:
    p_cylon(red);
    break;
  case 6:
    if((t - prevFrameTime) >= (1000L / FPS)) { // Handle scrolling
      theMatrix.fillScreen(0);
      theMatrix.setCursor(msgX, 0);
      theMatrix.print("Hello"); //ip here
      msgLen = String("Hello").length(); //ip here
      if(--msgX < (msgLen * -6)) msgX = 15; // We must repeat!
      theMatrix.show();
      prevFrameTime = t;
    }
    break;
  case 7:
    if((t - prevFrameTime) >= (1000L / FPS)) { // Handle scrolling
      theMatrix.fillScreen(0);
      theMatrix.setCursor(msgX, 0);
      theMatrix.print(writeCharStr);
      msgLen = String(writeCharStr).length();
      if(--msgX < (msgLen * -6)) msgX = 15; // We must repeat!
      theMatrix.show();
      prevFrameTime = t;
    }
    break;
  case 8:
    p_cylon(red);
    p_cylon(orange);
    p_cylon(yellow);
    p_cylon(green);
    p_cylon(blue);
    p_cylon(purple);
    break;
    case 10:
      for(int p=0;p<200;p++) {
        alertArea(aac, aax, aay, aaxE, aayE);
        aac--;
        delay(5);
      }
      for(int p=0;p<200;p++) {
        alertArea(aac, aax, aay, aaxE, aayE);
        aac++;
        delay(5);
      }
    break;
  }
}

