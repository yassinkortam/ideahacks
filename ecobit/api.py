import requests
import json
import os
from dotenv import load_dotenv
load_dotenv()

####################################################################################################################################
#___________________________________________________________API CONSTANTS___________________________________________________________

URL_ENDPOINT = os.getenv('URL_ENDPOINT')
API_KEY = os.getenv('API_KEY')

ACTIONS = {
    "insert":"/action/insertOne",
    "find":"/action/findOne",
    "delete":"/action/deleteOne",
    "update":"/action/updateOne"
}

####################################################################################################################################
#___________________________________________________________API FUNCTIONS___________________________________________________________

#Find collections
def find(setup):
    FIND_HEADERS = {
        'Content-Type': 'application/json',
        'Accept': 'application/json',
        'api-key': API_KEY, 
    }
    FIND_PAYLOAD = json.dumps({
        "collection": "user-stats",
        "database": "ideahacks",
        "dataSource": "AtlasCluster",
        "filter": { "email": setup["email"]},
        "projection": {
            "reuse" : 1,
            "steps" : 1
        }
    })
    url = URL_ENDPOINT + ACTIONS["find"]
    response = requests.request("POST", url, headers=FIND_HEADERS, data=FIND_PAYLOAD)
    if response.ok:
        if "document" in response.json().keys():
            return response.json()["document"]
        else:
            return {}
    else:
        return {}

#Insert new collections
def insert(setup, data):
    INSERT_HEADERS = {
        'Content-Type': 'application/json',
        'Access-Control-Request-Headers': '*',
        'api-key': API_KEY, 
    }
    INSERT_PAYLOAD = json.dumps({
        "collection": "user-stats",
        "database": "ideahacks",
        "dataSource": "AtlasCluster",
        "document": {
            "email": setup["email"],
            "name" : setup["name"],
            "reuse" : data["reuse"],
            "steps" : data["steps"]
        }
    })
    url = URL_ENDPOINT + ACTIONS["insert"]
    response = requests.request("POST", url, headers=INSERT_HEADERS, data=INSERT_PAYLOAD)
    return response.ok

#Update collections
def update(setup, data, current_state):
    UPDATE_HEADERS = {
        'Content-Type': 'application/json',
        'api-key': API_KEY, 
    }
    UPDATE_PAYLOAD = json.dumps({
        "collection": "user-stats",
        "database": "ideahacks",
        "dataSource": "AtlasCluster",
        "filter": { "email": setup["email"] },
        "update": {
            "$set": {
              "steps": data["steps"] + current_state["steps"],
              "reuse": data["steps"] + current_state["steps"]
            }
        }
    })
    url = URL_ENDPOINT + ACTIONS["update"]
    response = requests.request("POST", url, headers=UPDATE_HEADERS, data=UPDATE_PAYLOAD)
    return response.ok
