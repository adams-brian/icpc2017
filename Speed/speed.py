from __future__ import division
import glob, os

def solve(filename):
  speedToDistance = {}
  totalTime = 0
  with open(filename, "r") as file:
    for i, l in enumerate(file):
      line = l.strip().split()
      if i == 0:
        totalTime = int(line[1])
      else:
        distance = int(line[0])
        speed = int(line[1])
        if speed not in speedToDistance:
          speedToDistance[speed] = 0
        speedToDistance[speed] += distance

  # start with a large bounding range
  lower = -1000000000
  upper = 1000000000
  c = 0
  while True:
    computedTime = 0
    for speed in speedToDistance:
      distance = speedToDistance[speed]
      possibleSpeed = (speed + c)
      # protect against divide by 0
      if possibleSpeed < distance / totalTime:
        computedTime = totalTime + 1
        break
      computedTime += distance / possibleSpeed
    if computedTime > totalTime:
      lower = c
    else:
      upper = c
    newc = (lower + upper) / 2
    if abs(newc - c) < 0.00000001:
      break
    c = newc

  answerKey = 0
  with open(os.path.splitext(filename)[0] + ".ans", "r") as file:
    answerKey = float(file.read().strip())

  print c
  print answerKey

  if abs(answerKey - c) < 0.000001:
    print 'correct!'
  else:
    print '**************************************** INCORRECT ****************************************'


os.chdir("data")
for index, file in enumerate(glob.glob("*.in")):
  print file
  solve(file)
