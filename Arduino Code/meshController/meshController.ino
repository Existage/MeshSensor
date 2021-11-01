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

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 5 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg = "send";
  
  mesh.sendBroadcast( msg );
  
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
  taskSendMessage.enable();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
}
