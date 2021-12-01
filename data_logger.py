#!/usr/bin/env python3

import serial
import time
import csv
import sys
import select


ser = serial.Serial('/dev/ttyUSB0')
ser.baudrate = 115200
ser.flushInput()

def logging():
     try:
        ser_bytes = ser.readline()
        decoded_bytes = str(ser_bytes[0:len(ser_bytes)-2].decode("utf-8")).split(",")
        print(decoded_bytes)
        date = time.strftime("%m_%d_%Y")
        name = "data-" + date +".csv"
        with open(name,"a") as f:
            writer = csv.writer(f,delimiter=",")
            writer.writerow([time.strftime("%m/%d/%Y %H:%M:%S"),decoded_bytes[0],decoded_bytes[1],decoded_bytes[2],decoded_bytes[3],decoded_bytes[4],decoded_bytes[5]])
     except:
        print("Keyboard Interrupt")
    

while True:
    
    input = select.select([sys.stdin], [], [], 1)[0]
    if input:
        value = sys.stdin.readline().rstrip()
 
        if (value == "q"):
            print("Exiting")
            sys.exit(0)
        else:
            print("You entered: ", value)
            ser.write(value.encode())
    else:
        logging()
        
        
