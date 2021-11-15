# Project MIX: API Documentation

# High-Level Overview of MIX design 
There is essentially one major constraint to the middleware. It must have the ability to add and remove other IMs. However, this could be challenging since not all IMs will take in the same input such as GPS coordinates. Some IMs might be dependent on the output of other IMs. 

To resolve this issue, a graph/hierarchy data structure of IMs is recommended. This would enables us to illustrate the dependency of one IM to another and resolve any issues if there is an IM that is removed that others rely on. For instance, removing IM that gives country name would create issues for other IMs that take country name as input. 

As such, we denote level 1, closest to the user input, as IMs that rely only on GPS coordinates. Level 2 that rely on the output on level 1, level 3 that rely on the output of level 2, and etc...



# Middleware and IM
The middleware organizes the dependencies of other IMs via graph. To do this, we should keep track of the input and output of each IM. This would enable us to create a hierarchy as described. 

To achieve this, we should not only return a JSON of the data, but also the input and output of what it takes and a unique identifier IMID. The IMID serves to identify the IM and port number it uses. This number is calculated as demonstrated in the next paragraph. (port number = IM id, excluding "IM")
Ex:  {
        "input": "GPS coordinates",
        "output": "City and country"
        "IMID": "IM5049",
        "city": "East London",
        "country": "South Africa,
     }
Note: I also return the name of the owner as requested but I do not display it

Each IM will also have its own port in order to run. Since many people would specify the same port number, there can be contradictions. To resolve a protocol is needed:
We could use the roster found in campuswire and let the row number of your name indicate the port number of your first IM and add 5000 to it. So if your name appears on the 10th row, port = 5010 for your first IM. For each other IMs, add 65 accordingly (since there are approximately 60 people in the class). Ex: Person X's first IM has port 5003 since his or her name appears on the third row, second IM has port 5068, third IM has port 5133, and etc...

Since the ID of the IM has the same port number it would use, we could add everyone's url in the env file. We then traverse through the graph and run the corresponding url accordingly. The url has the form: IMID_url = "host:port"

However, to extract the information from each IM, we would need to know its route since each IM would have different input. As such, we could create a textfile of each IM's url in addition to a number denoting its hierarchy. For instance: http://127.0.0.1:5049/40/-88 1 
This implies this IM has the following url and its first in line of the chain of IMs. We follow this pattern accordingly for all IMs and then order the urls in ascending order based on their hierarchy number. This would enable us to add IMs without the worry of checking if a higher ranked IM exists. 



# Frontend and Middleware
If successful, display output in JSON format. No need to display the metadata of the IM- input, output, and IMID- just the actual information it achieves to do.

If fails, we return an error depending on what the issue is: Bad request, dependencies missing, etc...


# Adding/Removing IM from MIX
If the input of an IM relies on the output of another IM, the input of the dependent IM and output of the independent IM should match word for word. This would enable us to create the graph like structure and illustrate their relationship. If the IM is completly independent, level 1, make it one of the "roots" of the graph. When adding, we also make sure to add the IM's url in the env file. 

The graph should also have an "upward" direction: IMs that rely on the input of another should point to that IM. For instance: IM3 relies only on IM1 for input. This would imply IM3 points to only IM1. In other words, IM3's neigbor is IM1. 
Due to the upward nature, this would make it easier to remove IMs since we keep track of what each IM is dependent on. 

Removal Algorithm: 
        We find node1, which we want to remove.
        Check to see if node1 is the ONLY element in the neighbors of other nodes. Suppose node2 fits this situation. We then remove node1 and search for nodes that only have node2. And repeat. 

Nodes with only one neighbor imply they are soley dependent on that node. Nodes that do not have any neighbors imply they are the root or one of the roots. 

