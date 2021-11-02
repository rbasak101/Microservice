# map function:
def map(filename):
  dictionary = {}
  with open(filename, "r", encoding="utf-8") as f:
    lines = f.read().lower() # reads entire file
    for char in lines:
      letter = char.isalpha()
      if letter == False:
        continue
      if char not in dictionary:
          dictionary[char] = 1
      else:
          dictionary[char] = dictionary[char] + 1
    
  print(dictionary)
  return dictionary