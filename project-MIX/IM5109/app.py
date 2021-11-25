from flask import Flask, jsonify
import pycountry
app = Flask(__name__)

# input: country name
# output: alpha
@app.route('/<country>/') # returns the raw json format
def POST_country_to_alpha3(country):
    
    for finding_country in pycountry.countries:
        #print(finding_country.numeric, finding_country.name, finding_country.alpha_3)
        if str(finding_country.name) == str(country):
            alpha_3 = finding_country.alpha_3
            return jsonify({"input": "country",
                            "output": "alpha_3",
                            "IMID": "IM5109",
                            "alpha_3": alpha_3}), 200

    return jsonify({"input": "country",
                    "output": "alpha_3",
                    "IMID": "IM5109",
                    "alpha_3": "N/A"}), 200  # for some reason alpha does not exist for this country bc of API or invalid input
  
