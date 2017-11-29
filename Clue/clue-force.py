import glob, os
from itertools import combinations

people = {'A','B','C','D','E','F'}
weapons = {'G','H','I','J','K','L'}
rooms = {'M','N','O','P','Q','R','S','T','U'}
allcards = people | weapons | rooms

def solve(filename):
  stars = []
  players = []
  for i in range(0,4):
    players.append(set())
  exclusions = []
  for i in range(0,4):
    exclusions.append(set())
  with open(filename, "r") as file:
    for index, line in enumerate(file):
      if index == 0:
        continue
      if index == 1:
        players[0] = set(line.strip().split())
        exclusions[0] = allcards - players[0]
      else:
        theRound = line.strip().split()
        player = (index-2) % 4
        guess = {theRound[0], theRound[1], theRound[2]}
        for i, result in enumerate(theRound[3:], start=player+1):
          responder = i % 4
          if(result == '-'):
            exclusions[responder].update(guess)
          elif(result == '*'):
            stars.append((responder, guess))
          else:
            players[responder].add(result)
  
  updated = True
  while(updated):
    updated = False
    # remove redundant stars
    newStars = []
    for star in stars:
      responder = star[0]
      redundant = False
      for card in star[1]:
        if card in players[responder]:
          redundant = True
          updated = True
          break
      if not redundant:
        newStars.append(star)
    stars = newStars

    # simplify stars
    newStars = []
    for star in stars:
      responder = star[0]
      othersHave = (players[0] | players[1] | players[2] | players[3]) - players[responder]
      updatedSet = star[1] - set([card for card in star[1] if card in othersHave])
      if len(updatedSet) == 1:
        players[responder].add(updatedSet.pop())
        updated = True
      else:
        if updatedSet < star[1]:
          updated = True
        newStars.append((responder, updatedSet))
    stars = newStars

  # envelope processing
  knownCards = players[0] | players[1] | players[2] | players[3]
  notInEnvelope = knownCards.copy()
  for index, star in enumerate(stars):
    playerSet = {star[0]}
    for star2 in stars[index+1:]:
      if star[1] == star2[1]:
        playerSet.add(star2[0])
    if(len(playerSet) == len(star[1])):
      notInEnvelope.update(star[1])
  possiblePeople = people - notInEnvelope
  possibleWeapons = weapons - notInEnvelope
  possibleRooms = rooms - notInEnvelope
  for card in exclusions[0] & exclusions[1] & exclusions[2] & exclusions[3]:
    if card in people:
      possiblePeople = card
    if card in weapons:
      possibleWeapons = card
    if card in rooms:
      possibleRooms = card

  # print 'players:'
  # for index, player in enumerate(players):
  #   print '{}: {}'.format(index, player)
  # print 'exclusions:'
  # for index, exclusion in enumerate(exclusions):
  #   print '{}: {}'.format(index, exclusion)
  # print 'stars:'
  # for star in stars:
  #   print star
  # print 'notInEnvelope {}'.format(notInEnvelope)
  # print 'possiblePeople: {}'.format(possiblePeople)
  # print 'possibleWeapons: {}'.format(possibleWeapons)
  # print 'possibleRooms: {}'.format(possibleRooms)
  
  envelopePeople = set()
  envelopeWeapons = set()
  envelopeRooms = set()
  for person in possiblePeople:
    for weapon in possibleWeapons:
      for room in possibleRooms:
        envelope = {person, weapon, room}
        remaining = allcards - knownCards - envelope
        
        testPlayers = {}
        testPlayers[0] = players[0].copy()
        valid = False
        for t1 in combinations(remaining - exclusions[1], 5 - len(players[1])):
          s1 = set(t1)
          testPlayers[1] = players[1] | s1
          for t2 in combinations(remaining - s1 - exclusions[2], 4 - len(players[2])):
            s2 = set(t2)
            testPlayers[2] = players[2] | s2
            s3 = remaining - s1 - s2 - exclusions[3]
            testPlayers[3] = players[3] | s3

            # if there are not enough valid cards to fill out the hand just continue
            if len(testPlayers[3]) < 4:
              continue

            valid = True
            for star in stars:
              if len(star[1] & testPlayers[star[0]]) == 0:
                valid = False
                break
            if valid:
              break
          if valid:
            break
        if valid:
          envelopePeople.update(people & envelope)
          envelopeWeapons.update(weapons & envelope)
          envelopeRooms.update(rooms & envelope)
  
  print '{} {} {}'.format(envelopePeople, envelopeWeapons, envelopeRooms)

  answer = '{}{}{}'.format('?' if len(envelopePeople) > 1 else envelopePeople.pop(),
    '?' if len(envelopeWeapons) > 1 else envelopeWeapons.pop(),
    '?' if len(envelopeRooms) > 1 else envelopeRooms.pop())
  print answer
  with open(os.path.splitext(filename)[0] + ".ans", "r") as file:
    answerKey = file.read().strip()
    print answerKey
    if(answer == answerKey):
      print 'correct'
    else:
      print '**************************************** INCORRECT ****************************************'

  

os.chdir("data")
for index, file in enumerate(glob.glob("*.in")):
  print file
  solve(file)
