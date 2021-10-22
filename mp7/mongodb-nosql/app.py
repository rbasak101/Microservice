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
    # cursor = collection.find()
    # for record in cursor:
    #     print(record)
    # print(db.list_collection_names())
    return "Added\n", 200

# GET /<key> – Retrieves the latest version of a key
@app.route("/<key>", methods=["GET"])
def retrieve_latest(key): 
    collection = db[key]
    result = collection.find_one({"version": collection.count()})
    # print(result) 
    parsed_result = collection.find_one({"version": collection.count()}, {'_id': 0, 'value': 1})
    # print(parsed_result)
    # print(type(parsed_result))
    if type(result) is not dict:
        return "Key is not present\n", 404
    #return "sucessfully retrieved\n", 200    # fix conclusion 
    return jsonify({"value" : parsed_result['value'], "version" : collection.count()}), 200
    
# GET /<key>/<version> – Retrieves a specific version of a key
@app.route("/<key>/<version>", methods=["GET"])
def specific_retrieve(key, version):
    collection = db[key]
    result = collection.find_one({"version": int(version)})
    # print(result)
    parsed_result = collection.find_one({"version": int(version)}, {'_id': 0, 'value': 1})
    # print(parsed_result)
    if type(result) is not dict:
        return "Key and/or version is not present\n", 404
    #return "sucessfully retrieved\n", 200    # fix conclusion
    return jsonify({"value" : parsed_result['value'], "version" : int(version)}), 200

# DELETE /<key> – Completely deletes a key
@app.route("/<key>", methods=["DELETE"])
def delete(key):
    orig_total = 0
    for database in db.list_collection_names():
        orig_total += 1
    
    # print(orig_total)
    collection = db[key]
    collection.drop() #takes care of edge cases
    # print(db.list_collection_names())

    # print(len(db.list_collection_names()))
    # print
    if orig_total - 1 == len(db.list_collection_names()):
        return "Deleted\n", 404
    return "Key is not present\n", 200
