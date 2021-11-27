# Project MIX: API Documentation

# High-Level Overview of MIX design 
The middleware handles all incoming IMs and the addition or removal of IMs. However, removing an IM could be challenging since not all IMs will take in the same input such as GPS coordinates. Some IMs might be dependent on the output of other IMs. 

To resolve this issue, a graph/hierarchy data structure of IMs is recommended. This would enables us to illustrate the dependency of one IM to another and resolve any issues if there is an IM that is removed that others rely on. To construct the graph accordingly, a url.txt file will be used, which has a specific format that inludes the server link, input of the IM, output of the IM, hierarchy, and number of parameters the IM requires. 

The hierarchy denotes how far the IM is from the root. We denote level 1, closest to the user input, as IMs that rely only on GPS coordinates. Level 2 that rely on the output on level 1, level 3 that rely on the output of level 2, and etc...



# Middleware and IM
The middleware organizes the dependencies of other IMs via graph. To do this, we should keep track of the input and output of each IM. This would enable us to create a hierarchy as described. 

To achieve this, we should not only return a JSON of the data, but also the input and output of what it takes and a unique identifier IMID. The IMID serves to identify the IM and port number it uses. This number is calculated as demonstrated in the next paragraph. (port number = IM's ID)
Ex:  {
        "input": "GPS",
        "output": "city,country"
        "IMID": "IM5049",
        "city": "East London",
        "country": "South Africa,
     }
Note: I also return the name of the owner as requested but I do not display it

Each IM will also have its own port in order to run. Since many people could specify the same port number, there can be contradictions. To resolve a protocol is needed:

We could use the roster found in campuswire and let the row number of your name indicate the port number of your first IM and add 5000 to it. So if your name appears on the 10th row, port = 5010 for your first IM. For each other IMs, add 65 accordingly (since there are approximately 60 people in the class). Ex: Person X's first IM has port 5003 since his or her name appears on the third row, second IM has port 5068, third IM has port 5133, and etc...

However, to extract the information from each IM, we would need to know its route since each IM would have different input. As such, we could create a textfile of each IM's url in addition to a number denoting its hierarchy as mentioned earlier. For instance: http://127.0.0.1:5049/|GPS|city,country|1|2 
This implies this IM has the following url: http://127.0.0.1:5049, it's first in line of the chain of IMs, and requires 2 parameters. We follow this pattern accordingly for all IMs and then order the urls in ascending order based on their hierarchy number. This would enable us to add IMs without the worry of checking if a higher ranked IM exists or adding an IM that requires a certain input that is not given.

Regarding caching, the current implementation caches the result of each IM using a dictionary and datetime library. The key would be the url (Ex: http://127.0.0.1:5049/) and the key is a list of the form [time of storage, data]



# Frontend and Middleware
If successful, display output in JSON format. No need to display the metadata of the IM- input, output, and IMID- just the actual information it achieves to do.

If fails, we return an error depending on what the issue is: Bad request, dependencies missing, etc...


# Adding/Removing IM from MIX
If the input of an IM relies on the output of another IM, the input of the dependent IM and output of the independent IM should match word for word. This would enable us to create the graph like structure and illustrate their relationship. If the IM is completly independent, level 1, make it one of the "roots" of the graph. When adding, we also make sure to add the IM's url in the url textfile. 

The graph should also have an "upward" direction: IMs that rely on the input of another should point to that IM. For instance: IM3 relies only on IM1 for input. This would imply IM3 points to only IM1. In other words, IM3's neigbor is IM1. 
Due to the upward nature, this would make it easier to remove IMs since we keep track of what each IM is dependent on. 

Nodes with only one neighbor imply they are soley dependent on that node. Nodes that do not have any neighbors imply they are the root or one of the roots. 

