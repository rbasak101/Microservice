# # # Implement This
# # #Builds an image starting with python 3 versions
# FROM python:3 

# # specify who is going to maintain this image
# MAINTAINER basak4@illinois.edu

# WORKDIR /app

# #Copying files from mp2 and mp6 into the docker image
# COPY ./mp2 mp2
# COPY ./mp6 mp6

# # Set the working directory to /mp6

# # COPY command takes two parameters. The first parameter tells Docker what file(s) you would like to copy into the image. The second parameter tells Docker where you want that file(s) to be copied to. 
# COPY mp6/requirements.txt requirements.txt

# # executes the command pip3 install
# RUN pip3 install -r requirements.txt

# #Set environment variables used by the flask command
# ENV FLASK_APP=mp6/app.py
# ENV FLASK_RUN_HOST=0.0.0.0



# #CMD command​ specifies the instruction that is to be executed when a Docker container starts
# CMD [ "python3", "-m" , "flask", "run", "--host=0.0.0.0"]



FROM python:3
MAINTAINER basak4@illinois.edu
COPY ./mp2 /mp2
COPY ./mp6 /mp6
WORKDIR /mp6

RUN pip3 install -r requirements.txt

ENV FLASK_RUN_HOST=0.0.0.0
EXPOSE 5000


CMD ["python3", "-m" , "flask", "run", "--host=0.0.0.0"]

