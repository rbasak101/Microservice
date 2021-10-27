    The code provided in the paper served to count the number of occurences of each word in a large collection of documents. The code is executed into two parts: map and reduce.
    The map function takes in each word and emits "1" for every occurence, and the reduce function sums all the "1" emmited from the map based on the key. 

    Ex: Suppose we have three computers and at least 6 documents, and we assign 2 documents per computer
    
    Stage 1: Map function
    After this stage, we realize the word "food" is emmited 5 times- so 5 "1"s emmited- , "plants" is emmited 3 times, and "cats" is emmited 4 times
    {
        "food": 1,
        "food": 1,
        "food": 1,
        "food": 1,
        "food": 1, (etc...)
    }

    Stage 2: Reduce Function
    Each computer works on a key. In other words, computer 1 takes in the word "food", computer 2 takes in the word "plants" and computer 3 takes in "cats". After this, each computer sums the number of ones for that key. 

    {
        "food": 5,
        "plants": 3,
        "cats": 4
    }