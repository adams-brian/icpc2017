import glob, os
from itertools import combinations

people = {'A','B','C','D','E','F'}
weapons = {'G','H','I','J','K','L'}
rooms = {'M','N','O','P','Q','R','S','T','U'}
allcards = people | weapons | rooms


def solve(filename):
  state = {}
  for card in allcards:
    state[card] = {0,1,2,3,'E'}
  stars = []
  with open(filename, "r") as file:
    for index, line in enumerate(file):
      if index == 0:
        continue
      if index == 1:
        player0cards = set(line.strip().split())
        for card in player0cards:
          state[card] = {0}
        for card in (allcards - player0cards):
          state[card].discard(0)
      else:
        theRound = line.strip().split()
        player = (index-2) % 4
        guess = {theRound[0], theRound[1], theRound[2]}
        for i, result in enumerate(theRound[3:], start=player+1):
          responder = i % 4
          if(result == '-'):
            for card in guess:
              state[card].discard(responder)
          elif(result == '*'):
            stars.append((responder, guess))
          else:
            state[result] = {responder}
  updated = True
  while(updated):
    updated = False
    newStars = []
    for star in stars:
      player = star[0]
      oneOf = star[1]
      # discard any stars where the player is already known to have one of the cards
      skip = False
      for card in oneOf:
        if {player} == state[card]:
          updated = True
          skip = True
          break
      if(skip):
        continue
      newOneOf = oneOf.copy()
      # remove cards that the player is known to not have
      for card in oneOf:
        if(player not in state[card]):
          newOneOf.remove(card)
          updated = True
      if(len(newOneOf) == 1):
        state[newOneOf.pop()] = {player}
      else:
        newStars.append((player, newOneOf))
    stars = newStars
    # look for matching card combinations
    matchingCombos = []
    for index, star in enumerate(stars):
      playerSet = {star[0]}
      for star2 in stars[index+1:]:
        if star[1] == star2[1]:
          playerSet.add(star2[0])
      if(len(playerSet) == len(star[1])):
        matchingCombos.append(star[1])
        for card in star[1]:
          if state[card] != playerSet:
            state[card] = playerSet
            updated = True
    newStars = [star for star in stars if star[1] not in matchingCombos]
    if(len(newStars) < len(stars)):
      updated = True
    stars = newStars
    # envelope processing
    for cardSet in [people, weapons, rooms]:
      inEnvelope = [card for card in cardSet if state[card] == {'E'}]
      possible = [card for card in cardSet if 'E' in state[card] and len(state[card]) > 1]
      if len(inEnvelope) == 1:
        for card in possible:
          state[card].remove('E')
          updated = True
      if len(inEnvelope) == 0 and len(possible) == 1:
        state[possible[0]] = {'E'}
        updated = True
    # fill out player hands
    for player in range(0, 4):
      cards = [card for card in allcards if state[card] == {player}]
      possible = [card for card in allcards if player in state[card] and len(state[card]) > 1]
      num = 5 if player < 2 else 4
      if len(cards) == num:
        for card in allcards:
          if len(state[card]) > 1 and player in state[card]:
            state[card].remove(player)
            updated = True
      if len(cards) < num and len(cards) + len(possible) == num:
        for card in possible:
          state[card] = {player}
          updated = True

  # for card in state:
  #   print '{}: {}'.format(card, state[card])
  # print '---------'
  # print stars
  # print '---------'

  person = [card for card in people if 'E' in state[card]]
  weapon = [card for card in weapons if 'E' in state[card]]
  room = [card for card in rooms if 'E' in state[card]]

  print '{}{}{}'.format(person, weapon, room)
  answer = '{}{}{}'.format('?' if len(person) > 1 else person.pop(),
    '?' if len(weapon) > 1 else weapon.pop(),
    '?' if len(room) > 1 else room.pop())
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
