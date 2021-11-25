from flask import Flask, jsonify
import pycountry
from countryinfo import CountryInfo
app = Flask(__name__)

# input: country
# output: population
@app.route('/<country>/') # returns the raw json format
def POST_country_to_population(country):
    nation = CountryInfo(country)
    try:
        if country == "Vatican City":
            pop = 825
            return jsonify({"input": "country",
                            "output": "country population",
                            "IMID": "IM5169",
                            "country population": pop}), 200
        
        data = nation.info()
        pop = data["population"]
        pop = "{:,}".format(pop)
        return jsonify({"input": "country",
                        "output": "country population",
                        "IMID": "IM5169",
                        "country population": pop}), 200

    except:
        return jsonify({"input": "country",
                        "output": "country population",
                        "IMID": "IM5169",
                        "country population": pop}), 400


   
  
