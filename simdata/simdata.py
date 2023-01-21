import requests
import json
import os
from dotenv import load_dotenv
load_dotenv()

import random

URL_ENDPOINT = os.getenv('URL_ENDPOINT')
API_KEY = os.getenv('API_KEY')

ACTIONS = {
    "insert":"/action/insertOne",
    "find":"/action/findOne",
    "delete":"/action/deleteOne"
}

PAYLOAD = json.dumps({
    "collection": "records",
    "database": "events",
    "dataSource": "AtlasCluster",
    "document": {
        "_id": 1,
        "name" : "Yassin Kortam",
        "dishwashing" : 100,
        "steps" : 6000
    }
})

HEADERS = {
  'Content-Type': 'application/json',
  'Access-Control-Request-Headers': '*',
  'api-key': API_KEY, 
}

url = URL_ENDPOINT + ACTIONS["insert"]
response = requests.request("POST", url, headers=HEADERS, data=PAYLOAD)
print(response.text)