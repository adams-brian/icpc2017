import glob, os, sys

def getLeastCommonIntersection(intersections):
  lCounts = {}
  cCounts = {}
  for l,c in intersections:
    lCounts[l] = 1 if l not in lCounts else lCounts[l] + 1
    cCounts[c] = 1 if c not in cCounts else cCounts[c] + 1
  l = min(lCounts, key=lCounts.get)
  c = min(cCounts, key=cCounts.get)
  if lCounts[l] < cCounts[c]:
    return next(i for i in intersections if i[0] == l)
  else:
    return next(i for i in intersections if i[1] == c)

def solve(filename):
  with open(filename, 'r') as file:
    lines, columns = [int(n) for n in next(file).split()]
    grid = [[int(n) for n in line.split()] for line in file]
  
  viewDownLines = []
  for line in grid:
    viewDownLines.append(max(line))
  viewDownColumns = []
  for i in range(0, columns):
    v = []
    for line in grid:
      v.append(line[i])
    viewDownColumns.append(max(v))
  
  heights = sorted(set(viewDownLines) | set(viewDownColumns))

  result = []
  for line in grid:
    result.append([1 if v > 0 else 0 for v in line])
  
  linesCovered = set()
  columnsCovered = set()

  def updateResult(l, c, height):
    result[l][c] = height
    linesCovered.add(l)
    columnsCovered.add(c)
  
  while len(heights) > 0:
    height = heights.pop()
    linesNeedingHeight = [i for i,v in enumerate(viewDownLines) if v == height]
    columnsNeedingHeight = [i for i,v in enumerate(viewDownColumns) if v == height]

    intersections = []
    for l in linesNeedingHeight:
      for c in columnsNeedingHeight:
        if grid[l][c] > 0:
          intersections.append((l,c))
    
    while len(intersections) > 0:
      l,c = getLeastCommonIntersection(intersections)
      updateResult(l, c, height)
      linesNeedingHeight = [v for v in linesNeedingHeight if v is not l]
      columnsNeedingHeight = [v for v in columnsNeedingHeight if v is not c]
      intersections = [i for i in intersections if i[0] is not l and i[1] is not c]
    
    while len(linesNeedingHeight) > 0:
      l = linesNeedingHeight.pop()
      for c in columnsCovered:
        if result[l][c] > 0:
          updateResult(l, c, height)
          break
    while len(columnsNeedingHeight) > 0:
      c = columnsNeedingHeight.pop()
      for l in linesCovered:
        if result[l][c] > 0:
          updateResult(l, c, height)
          break
  
  answer = sum(map(sum, grid)) - sum(map(sum, result))

  answerKey = 0
  with open(os.path.splitext(filename)[0] + ".ans", "r") as file:
    answerKey = int(file.read().strip())

  print answer
  print answerKey

  if answer == answerKey:
    print 'correct!'
  else:
    print '**************************************** INCORRECT ****************************************'

os.chdir("data")
for index, file in enumerate(glob.glob("*.in")):
  print file
  solve(file)
