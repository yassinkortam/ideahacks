import serial
import json

ser = serial.Serial('/dev/tty.NathanESP', baudrate=115200, timeout=1)
while True:
    data = ser.readall().decode()
    print(data)
    if data and data[0] == '{':
        deserialized = json.loads(data)
