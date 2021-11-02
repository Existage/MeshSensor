//************************************************************
// This code uses the painlessMesh library and returns 
// 
//
//************************************************************
#include "painlessMesh.h"
#include <SD_ZH03B.h>       //needed for mesh
#include <Arduino.h>        //needed for mesh
#include "MHZ19.h"          //needed for mesh
#include <HardwareSerial.h> //needed for mesh

#define   MESH_PREFIX     "MeshNet"
#define   MESH_PASSWORD   "verysecure"
#define   MESH_PORT       5555

int8_t nodeNumber = 3; // Name needs to be unique

//For ZH03B
#define RXD1 18                                               //RX pin to which ZH03B TX pin connects to
#define TXD1 19                                               //TX pin to which ZH03B RX pin connects to

//For MHZ19
#define RXD2 16                                          // Rx pin which the MHZ19 Tx pin is attached to
#define TXD2 17                                          // Tx pin which the MHZ19 Rx pin is attached to
#define BAUDRATE 9600                                      // Device to MH-Z19 Serial baudrate (should not be changed)

HardwareSerial Serial_1(1);
HardwareSerial Serial_2(2);

MHZ19 myMHZ19;                                             // Constructor for library
SD_ZH03B ZH03B( Serial_1);          // Constructor for library ZH03B





// Prototypes
void receivedCallback( uint32_t from, String &msg );

painlessMesh  mesh;

String dataFetch(); //defined for global access

void setup() {
  Serial.begin(115200);
    
  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT );
  
  mesh.onReceive(&receivedCallback);
  setupSensors();
}

void loop() {
  mesh.update();
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("echoNode: Received from %u msg=%s\n", from, msg.c_str());
  if(from == 309512273){ //so that base station triggers data send. Change to base station ID if different
    Serial.println("Ack");
    String readings = dataFetch();
    Serial.println(readings);
    mesh.sendSingle(from, readings);
  }  
}

void setupSensors(){
  Serial_2.begin(BAUDRATE,SERIAL_8N1, RXD2, TXD2);                               // (Uno example) device to MH-Z19 serial start   
  myMHZ19.begin(Serial_2);                                // *Serial(Stream) refence must be passed to library begin(). 

  myMHZ19.autoCalibration();                              // Turn auto calibration ON (OFF autoCalibration(false))


  Serial_1.begin(BAUDRATE,SERIAL_8N1, RXD1, TXD1);
  delay(100);
  ZH03B.setMode( SD_ZH03B::QA_MODE );                   //sets to QA_MODE
}

String dataFetch(){
  int CO2;
  CO2 = myMHZ19.getCO2();                             // Request CO2 (as ppm)
  
  Serial.print("CO2 (ppm): ");                      
  Serial.println(CO2);     
                                

  int8_t Temp;
  Temp = myMHZ19.getTemperature();                     // Request Temperature (as Celsius)

  char printbuf1[90];
   
  if( ZH03B.readData() ) {
      
    sprintf(printbuf1, "%d,%d,%d,%d,%d,%d", nodeNumber, CO2, Temp,  ZH03B.getPM1_0(), ZH03B.getPM2_5(), ZH03B.getPM10_0() ); //returns nodenumber,co2,tempco2,pm1,pm2.5,pm10
    Serial.println(printbuf1);
  } else {   
    Serial.println( "ZH03B Error reading stream or Check Sum Error" );
    sprintf(printbuf1, "%d,%d,%d,x,x,x", nodeNumber, CO2, Temp ); //returns nodenumber,co2,tempco2,pm1,pm2.5,pm10
    Serial.println(printbuf1);
  } 
  
  
  return printbuf1;
  }
