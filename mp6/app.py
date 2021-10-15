from flask import Flask
import os, sys
app = Flask(__name__)

# Route for "/" for a web-based interface to this micro-service:
@app.route('/')
def index():
  from flask import render_template
  return render_template("index.html")

# Extract a hidden "uiuc" GIF from a PNG image:
@app.route('/extract', methods=["POST"])
def extract_hidden_gif():
  from flask import send_file, request

  # ...your code here...

  # dir = os.path.join(app.instance_path, "temp")

  os.system("rm -r ./temp") 
  os.makedirs("./temp", mode=511, exist_ok=True)
  file = request.files["png"]
  # print(file)
  # print("type is.. ")
  type = file.filename.split('.')[1]
  # print(type)
  if file and type == "png": # succesfully obtained file and check for file type
    file.save("./temp/file.png")
    os.system("make") # essential!
    path = "./png-extractGIF ./temp/file.png ./temp/hidden.gif"
    exit_value = os.system(path)
    print(exit_value)
    if exit_value != 0:
      return "Gif not present", 500
    else:
      return send_file("./temp/hidden.gif", as_attachment = False)
  else:
    return "PNG not submitted", 500
    
#edge case: no file but submitted
if __name__ == '__main__':
   app.run(host='0.0.0.0',port=5000)