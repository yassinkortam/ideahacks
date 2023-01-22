import json

####################################################################################################################################
# parseData parses data from bluetooth serial into a python dict for processing

def parseData(ser):
    raw = ser.readall().decode()
    print(raw)
    if raw and raw[0] == '{':
        try:
            deserialized = json.loads(raw)
        except:
            return {}
        data = deserialized["document"]
        return data
    return {}
       
        
        
