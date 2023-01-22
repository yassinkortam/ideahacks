import serial
import json

ser = serial.Serial('COM7', baudrate=115200, timeout=1)
while True:
    data = ser.readall().decode()
    print(data)
    if data and data[0] == '{':
        deserialized = json.loads(data)
