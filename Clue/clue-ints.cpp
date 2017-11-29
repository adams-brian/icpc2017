#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <tuple>
#include <unordered_set>
#include <exception>
#include <dirent.h>

using namespace std;

string people = "ABCDEF";
string weapons = "GHIJKL";
string rooms = "MNOPQRSTU";
string allCards = people + weapons + rooms;

int peopleNumber;
int weaponsNumber;
int roomsNumber;
int allCardsNumber;

map<char, int> letterToNumber;
map<int, char> numberToLetter;

vector<string> readFile(string filename) {
  ifstream inFile;

  inFile.open(filename);
  if (!inFile) {
      cout << "Unable to open file: " << filename << endl;
      throw exception();
  }
  
  vector<string> lines;
  string line;
  while (getline(inFile, line))
  {
    lines.push_back(line);
  }

  inFile.close();

  return lines;
}

string readLine(string line) {
  string entries = "";
  for (int index = 0; index < line.length(); index++) {
    if (index % 2 == 0) {
      entries += line[index];
    }
  }
  return entries;
}

template <typename Iterator>
inline bool next_combination(const Iterator first, Iterator k, const Iterator last)
{
   /* Credits: Thomas Draper */
   if ((first == last) || (first == k) || (last == k))
      return false;
   Iterator itr1 = first;
   Iterator itr2 = last;
   ++itr1;
   if (last == itr1)
      return false;
   itr1 = last;
   --itr1;
   itr1 = k;
   --itr2;
   while (first != itr1)
   {
      if (*--itr1 < *itr2)
      {
         Iterator j = k;
         while (!(*itr1 < *j)) ++j;
         std::iter_swap(itr1,j);
         ++itr1;
         ++j;
         itr2 = k;
         std::rotate(itr1,j,last);
         while (last != j)
         {
            ++j;
            ++itr2;
         }
         std::rotate(k,itr2,last);
         return true;
      }
   }
   std::rotate(first,k,last);
   return false;
}

int convertToNumber(string input) {
  int result = 0;
  for (int i=0; i<input.length(); i++) {
    result |= letterToNumber[input[i]];
  }
  return result;
}

string convertToString(int input) {
  string result = "";
  for(map<int, char>::iterator i = numberToLetter.begin(); i != numberToLetter.end(); i++)
  {
    int number = i->first;
    char letter = i->second;
    if ((input & number) == number) {
      result += letter;
    }
  }
  return result;
}

void solve(string filename) {
  int players [4] = {0};
  int exclusions [4] = {0};
  vector<tuple<int, int>> stars;
  vector<string> lines = readFile(filename);
  for (int index = 0; index < lines.size(); index++) {
    string line = readLine(lines[index]);
    if (index == 0) {
      continue;
    }
    if (index == 1) {
      players[0] = convertToNumber(line);
      exclusions[0] = allCardsNumber &~ players[0];
    }
    else
    {
      int player = (index - 2) % 4;
      for(int i = 3; i < line.length(); i++) {
        int responder = (player + i - 2) % 4;
        if (line[i] == '-') {
          exclusions[responder] |= convertToNumber(line.substr(0,3));
        }
        else if(line[i] == '*') {
          stars.push_back(make_tuple(responder, convertToNumber(line.substr(0, 3))));
        }
        else {
          players[responder] |= convertToNumber(line.substr(i,1));
        }
      }
    }
  }

  // for (int index = 0; index < 4; index++) {
  //   cout << index << ": " << convertToString(players[index]) << endl;
  // }
  // for (int index = 0; index < 4; index++) {
  //   cout << index << ": " << convertToString(exclusions[index]) << endl;
  // }
  // for (int index = 0; index < stars.size(); index++) {
  //   cout << "(" << get<0>(stars[index]) << ", " << convertToString(get<1>(stars[index])) << ")" << endl;
  // }

  int knownCards = players[0] | players[1] | players[2] | players[3];
  int notInEnvelope = knownCards;
  for (int i = 0; i < stars.size(); i++) {
    int r1 = get<0>(stars[i]);
    int c1 = get<1>(stars[i]);
    unordered_set<int> playerSet = {r1};
    for (int j=i+1; j < stars.size(); j++) {
      int r2 = get<0>(stars[j]);
      int c2 = get<1>(stars[j]);
      if(c1 == c2) {
        playerSet.insert(r2);
      }
    }
    if(playerSet.size() == convertToString(c1).length()) {
      notInEnvelope |= c1;
    }
  }
  int possiblePeopleNum = peopleNumber &~ notInEnvelope;
  int possibleWeaponsNum = weaponsNumber &~ notInEnvelope;
  int possibleRoomsNum = roomsNumber &~ notInEnvelope;
  int excludedCardsNum = exclusions[0] & exclusions[1] & exclusions[2] & exclusions[3];
  possiblePeopleNum = (peopleNumber & excludedCardsNum) > 0 ? (peopleNumber & excludedCardsNum) : possiblePeopleNum;
  possibleWeaponsNum = (weaponsNumber & excludedCardsNum) > 0 ? (weaponsNumber & excludedCardsNum) : possibleWeaponsNum;
  possibleRoomsNum = (roomsNumber & excludedCardsNum) > 0 ? (roomsNumber & excludedCardsNum) : possibleRoomsNum;

  int envelopePeopleNum = 0;
  int envelopeWeaponsNum = 0;
  int envelopeRoomsNum = 0;

  string possiblePeople = convertToString(possiblePeopleNum);
  string possibleWeapons = convertToString(possibleWeaponsNum);
  string possibleRooms = convertToString(possibleRoomsNum);

  for (int p = 0; p < possiblePeople.length(); p++) {
    for (int w = 0; w < possibleWeapons.length(); w++) {
      for (int r = 0; r < possibleRooms.length(); r++) {
        string envelope = possiblePeople.substr(p, 1) + possibleWeapons.substr(w, 1) + possibleRooms.substr(r, 1);
        int envelopeNum = convertToNumber(envelope);
        int remainingNum = allCardsNumber &~ (knownCards | envelopeNum);
        string remaining = convertToString(remainingNum);
        sort(begin(remaining), end(remaining));
        // cout << "envelope: " << envelope << endl;
        // cout << "remaining: " << remaining << endl;

        int testPlayers[4] = {0};
        testPlayers[0] = players[0];
        bool valid = false;

        string p1Possible = convertToString(remainingNum &~ exclusions[1]);
        size_t p1Num = 5 - convertToString(players[1]).length();
        if (p1Possible.length() < p1Num) continue;
        do {
          int s1 = convertToNumber(string(p1Possible.begin(), p1Possible.begin() + p1Num));
          testPlayers[1] = players[1] | s1;

          string p2Possible = convertToString(remainingNum &~ (s1 | exclusions[2]));
          size_t p2Num = 4 - convertToString(players[2]).length();
          if (p2Possible.length() < p2Num) continue;
          do {
            int s2 = convertToNumber(string(p2Possible.begin(), p2Possible.begin() + p2Num));
            testPlayers[2] = players[2] | s2;
            int s3 = remainingNum &~ (s1 | s2 | exclusions[3]);
            testPlayers[3] = players[3] | s3;
            if (convertToString(testPlayers[3]).length() < 4) {
              continue;
            }

            valid = true;
            for (int s=0; s<stars.size(); s++) {
              int responder = get<0>(stars[s]);
              int cards = get<1>(stars[s]);
              if ((testPlayers[responder] & cards) == 0) {
                valid = false;
                break;
              }
            }
            if (valid) break;
          }
          while (next_combination(p2Possible.begin(), p2Possible.begin() + p2Num, p2Possible.end()));
          if (valid) break;
        }
        while (next_combination(p1Possible.begin(), p1Possible.begin() + p1Num, p1Possible.end()));
        if (valid) {
          envelopePeopleNum |= (peopleNumber & envelopeNum);
          envelopeWeaponsNum |= (weaponsNumber & envelopeNum);
          envelopeRoomsNum |= (roomsNumber & envelopeNum);
        }
      }
    }
  }

  string envelopePeople = convertToString(envelopePeopleNum);
  string envelopeWeapons = convertToString(envelopeWeaponsNum);
  string envelopeRooms = convertToString(envelopeRoomsNum);

  cout << "{" << envelopePeople << "} {" << envelopeWeapons << "} {" << envelopeRooms << "}" << endl;

  string answer = "";
  answer += envelopePeople.length() > 1 ? '?' : envelopePeople[0];
  answer += envelopeWeapons.length() > 1 ? '?' : envelopeWeapons[0];
  answer += envelopeRooms.length() > 1 ? '?' : envelopeRooms[0];
  cout << answer << endl;

  string answerKeyFilename = filename.substr(0, filename.length() - 2) + "ans";
  string answerKey = readFile(answerKeyFilename)[0];
  cout << answerKey << endl;

  if (answer == answerKey) {
    cout << "correct!" << endl;
  }
  else {
    cout << "------------------------------------------ INCORRECT ------------------------------------------" << endl;
  }

  
}

int main() {
  
  for(int i=0; i<allCards.length(); i++) {
    char letter = allCards[i];
    int number = pow(2, i);
    letterToNumber[letter] = number;
    numberToLetter[number] = letter;
  }

  peopleNumber = convertToNumber(people);
  weaponsNumber = convertToNumber(weapons);
  roomsNumber = convertToNumber(rooms);
  allCardsNumber = convertToNumber(allCards);
  
  struct dirent *pDirent;
  DIR *pDir;
  pDir = opendir ("./data");
  if (pDir == NULL) {
      printf ("Cannot open directory ./data\n");
      return 1;
  }
  list<string> inputFiles;
  while ((pDirent = readdir(pDir)) != NULL) {
    string filename = pDirent->d_name;
    int length = filename.length();
    if(filename.length() > 3 && filename.substr(filename.length() - 3, 3) == ".in") {
      inputFiles.push_back("./data/" + filename);
    }
  }
  closedir (pDir);

  for (list<string>::iterator iterator = inputFiles.begin(); iterator != inputFiles.end(); iterator++) {
    cout << *iterator << endl;
    solve(*iterator);
  }
}
