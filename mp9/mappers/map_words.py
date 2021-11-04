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
            if word not in dictionary:
                dictionary[word] = 1
            else:
                dictionary[word] += 1
  return dictionary