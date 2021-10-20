from flask import Flask, render_template, request, jsonify
import requests
app = Flask(__name__)

from pymongo import MongoClient
mongo = MongoClient('localhost', 27017)
db = mongo["mp7-state-server"]

# TODO:
# PUT /<key> – Adds a versioned object
@app.route("/<key>", methods=["PUT"])
def add(key):
    collection = db[key]
    value = request.data.decode("utf-8")
    document =  { 
                 "value": value,
                 "version": collection.count() + 1
                }
    collection.insert_one(document)
    #entries += 1
    cursor = collection.find()
    for record in cursor:
        print(record)

    print(db.list_collection_names())
    return "Added\n", 200

# GET /<key> – Retrieves the latest version of a key
@app.route("/<key>", methods=["GET"])
def retrieve_latest(key): 
    print("finding key")
    collection = db[key]
    result = collection.find_one({"version": collection.count()})
    print(result)
    print(type(result))
    return "sucessfully retrieved\n", 200    # fix conclusion + edge?

    
# GET /<key>/<version> – Retrieves a specific version of a key
@app.route("/<key>/<version>", methods=["GET"])
def specific_retrieve(key, version):
    print("finding key")
    collection = db[key]
    result = collection.find_one({"version": int(version)})
    print(result)
    print(type(result))
    return "sucessfully retrieved\n", 200    # fix conclusion + edge?

# DELETE /<key> – Completely deletes a key
@app.route("/<key>", methods=["DELETE"])
def delete(key):
    collection = db[key]
    collection.drop() #takes care of edge cases
    print(db.list_collection_names())

    return "Deleted\n", 200

    # if deleted:
    #     return "Deleted all keys\n", 200
    # else:
    #     return "Key not present\n"