import json
import serial
from parsedata import parseData
from api import insert, find, update


####################################################################################################################################
#______________________________________________________________SETUP________________________________________________________________

#Get setup info 
with open('setup.json') as setup_file:
    setup_contents = setup_file.read()
setup = json.loads(setup_contents)

#Open serial port
ser = serial.Serial('/dev/tty.'+setup["device_name"], baudrate=115200, timeout=1)

####################################################################################################################################
#_____________________________________________________________API LOOP______________________________________________________________

while(True):
    data = parseData(ser)
    if data:
        #Read the current state of the data
        current_state = find(setup)
        if not current_state:
            insert(setup, data)
            print("No history")
            continue
        #Increment the data
        update(setup, data, current_state)
    else:
        print("No data")
        