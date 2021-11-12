# Project MIX: API Documentation

High-Level Overview of MIX design
There are essentially one major constraint to the middleware. It must have the ability to add and remove other IMs. However, this could be challenging since not all IMs will take in the same input such as GPS coordinates. Some IMs might be dependent on the output of other IMs. 

To resolve this issue, a graph/hierarchy data structure of IMs is recommended. This would enables us to illustrate the dependency of one IM to another and resolve any issues if there is an IM that is removed that others rely on. For instance, removing IM that gives country name would create issues for other IMs that take country name as input. 

As such, we denote level 1, closest to the user input, as IMs that rely only on GPS coordinates. Level 2 that rely on the output on level 1, level 3 that rely on the output of level 2, and etc...




Middleware and IM
The middleware organizes the dependencies of other IMs via graph. To do this, we should keep track of the input and output of each IM. This would enable us to create a hierarchy as described. 

To achieve this, we should not only return a JSON of the data, but also the input and output of what it takes as well as it hierarchy (not sure if hierarchy is needed but it does not cost anything)
** Note: the ouput of the IM = the name of the IM **
Ex:  {
        "input": "GPS coordinates",
        "output": "City and country"
        "city": "East London",
        "country": "South Africa,
     }




Frontend and Middleware
If successful, display output in JSON format. No need to display the metadata of the IM, just the actual information it achieves to do.

If fails, we return an error depending on what the issue is: Bad request, depencies missing, etc...




Adding/Removing IM from MIX
If the input of an IM relies on the output of another IM, the input of the dependent IM and output of the independent IM should match word for word. This would enable us to create the graph like structure. If the IM is completly independent, level 1, make it one of the "roots" of the graph. 

We add an IM based on what it needs as its input to the graph. If the input is available, we display the information.

Regarding removal, we could either remove the IM and other IMs below it as well or create a boolean variable that indicates if we want to keep it. If the variable if false, in other words we want to remove it, we don't go down the hierachy and display information. 1st situation optimizes space and 2nd situation optimizes removal and adding procedure. For now, we shall remove the actual node from the graph. 