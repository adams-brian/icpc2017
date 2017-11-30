import glob, os

def canMapLetter(src, dest, letterMap):
  seenSet = set()
  srcSet = {src}
  while len(srcSet) > 0:
    pop = srcSet.pop()
    if pop == dest:
      return True
    if pop in seenSet:
      continue
    seenSet.add(pop)
    if pop in letterMap:
      for letter in letterMap[pop]:
        srcSet.add(letter)
  return False

def canMapWord(src, dest, letterMap):
  if(len(src) != len(dest)):
    return False
  for i, letter in enumerate(src):
    if not canMapLetter(letter, dest[i], letterMap):
      return False
  return True

def solve(filename):
  letterMap = {}
  wordPairs = []
  mc = 0
  mc = 0
  with open(filename, "r") as file:
    for i, l in enumerate(file):
      line = l.strip().split()
      if i == 0:
        mc = int(line[0])
        wc = int(line[1])
      elif i > 0 and i < mc + 1:
        src = line[0]
        dest = line[1]
        if src not in letterMap:
          letterMap[src] = set()
        letterMap[src].add(dest)
      else:
        wordPairs.append((line[0], line[1]))

  answer = []
  for wordPair in wordPairs:
    answer.append('yes' if canMapWord(wordPair[0], wordPair[1], letterMap) else 'no')

  answerKey = []
  with open(os.path.splitext(filename)[0] + ".ans", "r") as file:
    for l in file:
      answerKey.append(l.strip())
  
  correct = True
  if len(answer) != len(answerKey):
    print 'different number of answers!'
    correct = False
  else:
    for i, a in enumerate(answerKey):
      if (answer[i] != a):
        correct = False
  if correct:
    print 'correct!'
  else:
    print '**************************************** INCORRECT ****************************************'


os.chdir("data")
for index, file in enumerate(glob.glob("*.in")):
  print file
  solve(file)
