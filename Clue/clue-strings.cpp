#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <unordered_set>
#include <exception>
#include <dirent.h>

using namespace std;

string people = "ABCDEF";
string weapons = "GHIJKL";
string rooms = "MNOPQRSTU";
string allCards = people + weapons + rooms;

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

bool contains(string one, string two) {
  if (one.find(two) == string::npos) {
    return false;
  }
  return true;
}
bool contains(string one, char two) {
  if (one.find(two) == string::npos) {
    return false;
  }
  return true;
}

string doUnion(string one, string two) {
  string result = one;
  for (int i=0; i<two.length(); i++) {
    if (!contains(result, two[i])) {
      result += two[i];
    }
  }
  return result;
}

string doDifference(string one, string two) {
  string result = "";
  for (int i=0; i<one.length(); i++) {
    if (!contains(two, one[i])) {
      result += one[i];
    }
  }
  return result;
}

bool hasOne(string one, string two) {
  for (int i=0; i<two.length(); i++) {
    if (contains(one, two[i])) {
      return true;
    }
  }
  return false;
}

string doIntersection(string one, string two) {
  string result = "";
  for (int i=0; i<two.length(); i++) {
    if (contains(one, two[i])) {
      result += two[i];
    }
  }
  return result;
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

void solve(string filename) {
  string players [4];
  string exclusions [4];
  vector<string> stars;
  vector<string> lines = readFile(filename);
  for (int index = 0; index < lines.size(); index++) {
    string line = readLine(lines[index]);
    if (index == 0) {
      continue;
    }
    if (index == 1) {
      players[0] = line;
      exclusions[0] = doDifference(allCards, players[0]);
    }
    else
    {
      int player = (index - 2) % 4;
      for(int i = 3; i < line.length(); i++) {
        int responder = (player + i - 2) % 4;
        if (line[i] == '-') {
          exclusions[responder] = doUnion(exclusions[responder], line.substr(0,3));
        }
        else if(line[i] == '*') {
          stars.push_back(to_string(responder) + line.substr(0,3));
        }
        else {
          players[responder] = doUnion(players[responder], line.substr(i,1));
        }
      }
    }
  }

  // for (int index = 0; index < 4; index++) {
  //   cout << index << ": " << players[index] << endl;
  // }
  // for (int index = 0; index < 4; index++) {
  //   cout << index << ": " << exclusions[index] << endl;
  // }
  // for (int index = 0; index < stars.size(); index++) {
  //   cout << stars[index] << endl;
  // }

  string knownCards = doUnion(players[0], doUnion(players[1], doUnion(players[2], players[3])));
  string notInEnvelope = knownCards;
  for (int index = 0; index < stars.size(); index++) {
    string star = stars[index];
    string playerSet = star.substr(0, 1);
    for (int j=index+1; j < stars.size(); j++) {
      string star2 = stars[j];
      if(star.substr(1) == star2.substr(1)) {
        playerSet = doUnion(playerSet, star2.substr(0,1));
      }
    }
    if(playerSet.length() == star.substr(1).length()) {
      notInEnvelope = doUnion(notInEnvelope, star.substr(1));
    }
  }
  string possiblePeople = doDifference(people, notInEnvelope);
  string possibleWeapons = doDifference(weapons, notInEnvelope);
  string possibleRooms = doDifference(rooms, notInEnvelope);
  string excludedCards = doIntersection(exclusions[0], doIntersection(exclusions[1], doIntersection(exclusions[2], exclusions[3])));
  for(int i=0; i<excludedCards.length(); i++) {
    string card = excludedCards.substr(i, 1);
    if(contains(people, card)) {
      possiblePeople = card;
    }
    if(contains(weapons, card)) {
      possibleWeapons = card;
    }
    if(contains(rooms, card)) {
      possibleRooms = card;
    }
  }

  string envelopePeople = "";
  string envelopeWeapons = "";
  string envelopeRooms = "";

  for (int p = 0; p < possiblePeople.length(); p++) {
    for (int w = 0; w < possibleWeapons.length(); w++) {
      for (int r = 0; r < possibleRooms.length(); r++) {
        string envelope = possiblePeople.substr(p, 1) + possibleWeapons.substr(w, 1) + possibleRooms.substr(r, 1);
        string remaining = doDifference(allCards, doUnion(knownCards, envelope));
        sort(begin(remaining), end(remaining));
        // cout << "envelope: " << envelope << endl;
        // cout << "remaining: " << remaining << endl;

        string testPlayers[4];
        testPlayers[0] = players[0];
        bool valid = false;

        string p1Possible = doDifference(remaining, exclusions[1]);
        size_t p1Num = 5 - players[1].length();
        if (p1Possible.length() < p1Num) continue;
        do {
          string s1 = string(p1Possible.begin(), p1Possible.begin() + p1Num);
          testPlayers[1] = players[1] + s1;

          string p2Possible = doDifference(remaining, doUnion(s1, exclusions[2]));
          size_t p2Num = 4 - players[2].length();
          if (p2Possible.length() < p2Num) continue;
          do {
            string s2 = string(p2Possible.begin(), p2Possible.begin() + p2Num);
            testPlayers[2] = players[2] + s2;
            string s3 = doDifference(remaining, doUnion(s1, doUnion(s2, exclusions[3])));
            testPlayers[3] = players[3] + s3;
            if (testPlayers[3].length() < 4) {
              continue;
            }

            valid = true;
            for (int s=0; s<stars.size(); s++) {
              string star = stars[s];
              int responder = stoi(star.substr(0, 1));
              if (!hasOne(testPlayers[responder], star.substr(1))) {
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
          envelopePeople = doUnion(envelopePeople, doIntersection(people, envelope));
          envelopeWeapons = doUnion(envelopeWeapons, doIntersection(weapons, envelope));
          envelopeRooms = doUnion(envelopeRooms, doIntersection(rooms, envelope));
        }
      }
    }
  }

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
