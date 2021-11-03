import serial
import time
import csv

ser = serial.Serial('/dev/ttyUSB0')
ser.baudrate = 115200
ser.flushInput()

while True:
    try:
        ser_bytes = ser.readline()
        decoded_bytes = str(ser_bytes[0:len(ser_bytes)-2].decode("utf-8")).split(",")
        print(decoded_bytes)
        with open("test_data.csv","a") as f:
            writer = csv.writer(f,delimiter=",")
            writer.writerow([time.strftime("%m/%d/%Y %H:%M:%S"),decoded_bytes[0],decoded_bytes[1],decoded_bytes[2],decoded_bytes[3],decoded_bytes[4],decoded_bytes[5]])
    except:
        print("Keyboard Interrupt")
        
