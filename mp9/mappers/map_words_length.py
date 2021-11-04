import re
# map function:
def map(filename):
  dictionary = {}
  with open(filename, "r", encoding="utf-8") as f:
    for line in f:
        line = line.lower()
        for word in re.split('[^a-zA-Z]', line):

            word = word.lower()
            is_word = word.isalpha()
            if is_word == False:
                continue
            if len(word) not in dictionary:
                dictionary[len(word)] = 1
            else:
                dictionary[len(word)] += 1
  return dictionary