//************************************************************
// this is a simple mesh controller program to collect readings from the mesh network
//
// 1. sends a message to initiate readings on all the nodes
// 2. prints readings it receives to Serial.print
//adapted from painlessmesh broadcast example program 
//
//************************************************************
#include "painlessMesh.h"

#define   MESH_PREFIX     "MeshNet"
#define   MESH_PASSWORD   "verysecure"
#define   MESH_PORT       5555

int sampleTime = 10; // default to sample every 10 seconds
int incomingVal = 10; // for incoming serial data

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * sampleTime , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg = "send";
  
  mesh.sendBroadcast( msg );
  
}

Task taskSetSampleRate(TASK_SECOND , TASK_FOREVER, &setSampleTime );

void setSampleTime(){
  //Serial.println(Serial.available());
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingVal = Serial.parseInt();
    if (incomingVal > 0 && incomingVal < 601){ //expecting int time in seconds greater than 0 and less that 601= 10 min
      taskSendMessage.setInterval(incomingVal * TASK_SECOND);
      //Serial.printf("Sample rate set to %d seconds \n", sampleTime);
      }
    }
    
  }

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.println(msg.c_str()); //prints msg direct to serial                    
}

void newConnectionCallback(uint32_t nodeId) {
   // Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
 // Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
   // Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  userScheduler.addTask( taskSetSampleRate );
  taskSendMessage.enable();
  taskSetSampleRate.enable();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
}
