/*
	30/4/2019
	Made by Alejandro Pinel Martínez
	Tuenti Challenge 9
	Challenge 6 - Alphabet from outer space
*/

#include <iostream>
#include <fstream>
#include <list>
#include <vector>

using namespace std;

class Letter {
private:
   char c;
   Letter * previous;

public:
   Letter(char c = '\0') {
      this -> c = c;
      previous = 0;
   }

   void setLetter(char c) {
      this -> c = c;
   }

   char getLetter() const {
      return c;
   }

   void setPrevious(Letter * pr) {
      if (!SearchInLessers(pr -> getLetter())) {
         //cout << "No encontrada letra " << pr -> getLetter() << " como menor de " << getLetter() << endl;
         previous = pr;
      }
      else {
         //cout << "Encontrada letra " << pr -> getLetter() << " como menor de " << getLetter() << endl;
      }
   }

   char getPreviousChar() {
      if (previous == 0)
         return '\0';
      else
         return previous -> getLetter();
   }

   bool SearchInLessers(char less) {
      bool result = false;
      if (c == less)
         result = true;
      else if (previous != 0)
         result = previous -> SearchInLessers(less);

      return result;
   }
};

class Alphabet {
private:
   vector<vector<char> > words;
   vector<Letter> letters;

public:

   void AddWord(vector<char> newword) {
      words.push_back(newword);
   }

   int getLetter(char c) {
      bool exists = false;
      int i = 0;
      while (i < letters.size() && !exists) {
         exists = c == letters[i].getLetter();
         if (!exists)
            i++;
      }
      return i;
   }

   list<char> Order() {
      /*
      for (int i = 0; i < words.size(); i++) {
         for (int j = 0; j < words[i].size(); j++) {
            cout << words[i][j];
         }
         cout << endl;
      }
      */

      list<char> order;
      ExtractLetters();
      ExtractLogic();
      /*
      for (int i = 0; i < letters.size(); i++) {
         cout << letters[i].getLetter() << " anterior " << letters[i].getPreviousChar() << endl;
      }
      */

      if (Consistence()) {
         Letter * first = getFirstLetter();
         while(first != 0) {
            order.push_back(first -> getLetter());
            first = getFirstLetter(first -> getLetter());
         }
      }

      return order;
   }

private:

   void ExtractLetters() {
      letters.clear();
      for (int i = 0; i < words.size(); i++) {
         for (int j = 0; j < words[i].size(); j++) {
            if (getLetter(words[i][j]) == letters.size()) {
               Letter l(words[i][j]);
               letters.push_back(l);
            }
         }
      }
   }

   void ExtractLogic() {
      for (int i = 0; i < words.size() - 1; i++) {
         int depth = 0;
         while (depth < words[i].size() - 1 && depth < words[i + 1].size() - 1 && words[i][depth] == words[i + 1][depth])
            depth++;
         if (words[i][depth] != words[i + 1][depth]) {
            letters[getLetter(words[i + 1][depth])].setPrevious(&letters[getLetter(words[i][depth])]);
         }
      }
   }

   bool Consistence() {
      bool isConsistence = true;
      bool * table = new bool[letters.size()];
      for (int i = 0; i < letters.size(); i++)
         table[i] = false;
      for (int i = 0; i < letters.size() && isConsistence; i++) {
         char previous = letters[i].getPreviousChar();
         if (previous != '\0') {
            if (table[getLetter(previous)] == true)
               isConsistence = false;
            table[getLetter(previous)] = true;
         }
      }
      int errors = 0;
      for (int i = 0; i < letters.size() && isConsistence; i++) {
         if (table[i] == false)
            errors++;
         if (errors > 1)
            isConsistence = false;
      }
      delete [] table;

      return isConsistence;
   }

   Letter * getFirstLetter(char ch = '\0') {
      bool found = false;
      Letter * l = 0;
      for (int i = 0; i < letters.size() && !found; i++) {
         if (letters[i].getPreviousChar() == ch) {
            l = &letters[i];
            found = true;
         }
      }
      return l;
   }
};


//Function to get the data if the file exits
bool GetData (string testfile, int & ncases, Alphabet * & problem) {
   bool error = false;

   //Open the test file
   fstream inputfile;
   inputfile.open (testfile.c_str() , ios::in);

   //Check if the file exists
   if (inputfile.is_open()){
     inputfile >> ncases;
     problem = new Alphabet [ncases];

     int nwords;
     char c;
     for (int i = 0; i < ncases; i++) {
        inputfile >> nwords;
        inputfile.ignore();
        for (int j = 0; j < nwords; j++) {
           vector<char> word;
           inputfile.get(c);
           while (c != '\n') {
             word.push_back(c);
             inputfile.get(c);
          }
          problem[i].AddWord(word);
        }
      }

      inputfile.close();
   }
   else
      error = true;

   return error;
}




//Show the results on screen
void ShowResults (list<char> results, int ncase) {
      cout << "Case #" << ncase + 1 << ":";
      if (results.size() == 0) {
         cout << " AMBIGUOUS";
      }
      else {
         for (list<char>::iterator i = results.begin(); i != results.end(); i++)
            cout << " " << *i;
      }
      cout << endl;
}

int main () {
	const string FILENAME = "submitInput";
   int ncases;
   Alphabet * problem;

   //Get the data
   bool posible_error = GetData(FILENAME, ncases, problem);

   //If the data is correct
   if (!posible_error){
      list<char> result;
      for (int i = 0; i < ncases; i++){
      //for (int i = 36; i < 37; i++){
         result = problem[i].Order();
         ShowResults(result, i);
      }


      delete [] problem;
      problem = 0;
   }
   else
      cout << "Test file is missing, the program can't do anything without it :(\n";


   return 0;
}
