/*
 * mOwayduinoWifi example library
 */
 
//#define TWITTER_EXAMPLE
#define WEB_SERVER
/* IMPORTANT  */ /* COMMENT #define APP_SOCKAPP and UNCOMMENT #define APP_WISERVER 
in apps-conf.h in arduino/libraries/MowayduinoWifi folder */

//#define SOCKET 
/* IMPORTANT  */ /* UNCOMMENT #define APP_SOCKAPP and COMMENT #define APP_WISERVER 
in apps-conf.h in arduino/libraries/MowayduinoWifi folder 
UNCOMMENT #define socket in socketapp.c  */


#include <MowayduinoRobot.h>
#include <SPI.h>
#include <EEPROM.h>
#include "MowayduinoWifi.h"

#ifdef SOCKET
#include <WiShield.h>
#else
#include <WiServerMoway.h>
#endif

mowayduinorobot Mowayduino;
int move;
int red=0, green=0, blue=0,front=0,brake=0;

#ifdef TWITTER_EXAMPLE

// This function writes tweet when twittermowayduino.submit() is called 
void tweet_write() {
  WiServer.print("status=Mowayduino light sensor: ");
  WiServer.print(Mowayduino.readLight());
}


// Function that prints data from tweet return
void tweet_return(char* data, int len) {
  // Print the data returned by the server
  while (len-- > 0) {
    Serial.print(*(data++));
  } 
}

#endif

#ifdef WEB_SERVER

/******************  Redirect to / when command are sent  ******************/
void respond()
{
  WiServer.print("Access-Control-Allow-Origin: *\n");
  WiServer.print("\n");
  WiServer.print("{ \"Mowayduino\" : {"); //JSON
  Mowayduino.readAcc();
  WiServer.print("\"Accelerometer\" : {");
    WiServer.print(" \"x\" : ");
      WiServer.print(Mowayduino.Accelerometer[X_ACC]);
    WiServer.print(", \"y\" : ");
      WiServer.print(Mowayduino.Accelerometer[Y_ACC]);
    WiServer.print(", \"z\" : ");
      WiServer.print(Mowayduino.Accelerometer[Z_ACC]);
  WiServer.print("},");
  Mowayduino.readObstacle();
  WiServer.print("\"Obstacle\" : {");
    WiServer.print(" \"LL\" : ");
      WiServer.print(Mowayduino.Obstacle[IR_LL]);
    WiServer.print(", \"CL\" : ");
      WiServer.print(Mowayduino.Obstacle[IR_CL]);
    WiServer.print(", \"CR\" : ");
      WiServer.print(Mowayduino.Obstacle[IR_CR]);
    WiServer.print(", \"RR\" : ");
      WiServer.print(Mowayduino.Obstacle[IR_LR]);
  WiServer.print("},");
  Mowayduino.readLine();
  WiServer.print("\"Line\" : [");
      WiServer.print(Mowayduino.Line[0]);
    WiServer.print(",");
      WiServer.print(Mowayduino.Line[1]);
  WiServer.print("],");  
  WiServer.print("\"Mic\" :");
  WiServer.print(Mowayduino.readMic());
  WiServer.print(", \"Batt\" :");
  WiServer.print(Mowayduino.readBatt());
  WiServer.print(", \"Light\" :");
  WiServer.print(Mowayduino.readLight());  
  WiServer.print("}}");
}



/******************  HTML Page serving function  ******************/
// This is our page serving function that generates web pages
boolean mowayduinoPage(char* URL) 
{
  brake =1;
  // Check if the requested URL matches "/"
  if (strcmp(URL, "/r") == 0) {
    // Use WiServer's print and println functions to write out the page content
    red^=1;
    respond();
    return true;
  }
  else if (strcmp(URL, "/b") == 0) {
    // Use WiServer's print and println functions to write out the page content
    blue^=1;
    respond();
    return true;
  }
  else if (strcmp(URL, "/g") == 0) {
    // Use WiServer's print and println functions to write out the page content
    green^=1;
    respond();
    return true;
  }
  else if (strcmp(URL, "/f") == 0) {
    // Use WiServer's print and println functions to write out the page content
    front^=1;
    respond();
    return true;
  }
  else if (strcmp(URL, "/vp") == 0) {
    // Use WiServer's print and println functions to write out the page content
    speed++;
    respond();
    return true;
  }
  else if (strcmp(URL, "/vp") == 0) {
    // Use WiServer's print and println functions to write out the page content
    speed--;
    respond();
    return true;
  } 
  else if (strcmp(URL, "/up") == 0) {
    // Use WiServer's print and println functions to write out the page content
    move=MOVE_STRAIGHT;
    respond();
    return true;
  }

  else if (strcmp(URL, "/left") == 0) {
    // Use WiServer's print and println functions to write out the page content
    move=MOVE_LEFT;
    respond();
    return true;
  }
  else if (strcmp(URL, "/right") == 0) {
    // Use WiServer's print and println functions to write out the page content
    move=MOVE_RIGHT;
    respond();
    return true;
  }
  else if (strcmp(URL, "/down") == 0) {
    // Use WiServer's print and println functions to write out the page content
    move=MOVE_BACK;
    respond();
    return true;
  }
  else if (strcmp(URL, "/stop") == 0) {
    // Use WiServer's print and println functions to write out the page content
    move=STOP;
    brake=1;
    speed=100;
    respond();
    return true;
  }


  else if (strcmp(URL, "/") == 0)
  {
    WiServer.print(HEAD);
    WiServer.print("<meta http-equiv=\"Refresh\" content=\"0;url=");
    WiServer.print(EXT_SERVER);
    WiServer.print("?ip=");
    WiServer.print(LOCAL_IP);
    WiServer.print("\">");
    WiServer.print(END_HEAD);
    WiServer.print(END);  

    return true;
  }
  else
    return false;
}

#endif





void setup() {
  Mowayduino.beginMowayduino();
  Mowayduino.Blueon();
  beginmowayduinoIoT();
  Mowayduino.Blueoff();
  Mowayduino.Greenon();
  
  Serial.begin(57600);
  //while (!Serial);
}


void loop(){

#ifdef TWITTER_EXAMPLE
  // Check if it's time to get an update
  if (millis() >= updateTime) {
    twittermowayduino.submit();    
    // Get another update one minute from now
    updateTime += 60000;
  }
  Mowayduino.Stop();
#endif

#ifdef SOCKET
  WiFi.run();
   if (red==1)
   {
    Mowayduino.Redon();
    Mowayduino.Greenoff();
    Mowayduino.Blueoff();
   }
   else if (green==1)
   {
    Mowayduino.Redoff();
    Mowayduino.Greenon();
    Mowayduino.Blueoff();
   }
   else if (blue==1)
   {
    Mowayduino.Redoff();
    Mowayduino.Greenoff();
    Mowayduino.Blueon();
   }

#else
  // Run WiServer
  WiServer.server_task();
  delay(10);
#endif


#ifdef WEB_SERVER
  int speed = 100;
  if (blue==1)
    Mowayduino.Blueon();
  else
    Mowayduino.Blueoff();
  if (red==1)
    Mowayduino.Redon();
  else
    Mowayduino.Redoff();
  if (green==1)
    Mowayduino.Greenon();
  else
    Mowayduino.Greenoff();
  if (front==1)
    Mowayduino.Fronton();
  else
    Mowayduino.Frontoff();
  if (brake==1)
    Mowayduino.Brakeon();
  else
    Mowayduino.Brakeoff();
  if (move == STOP)
  {
    Mowayduino.Stop();
    move=0;
  }
  if (move == MOVE_STRAIGHT)
  {
    Mowayduino.Straight(speed);
    move=0;
  }
  if (move == MOVE_LEFT)
  {
    Mowayduino.TurnLeft(speed,30);
    move=0;
  }
  if (move == MOVE_RIGHT)
  {
    Mowayduino.TurnRight(speed,30);
    move=0;
  }
  if (move == MOVE_BACK)
  {
    Mowayduino.Back(speed);
    move=0;
  }
#endif
  
}
