/*
	30/4/2019
	Made by Alejandro Pinel Martínez
	Tuenti Challenge 9
	Challenge 5 - Forbidden Love
*/

#include <iostream>
#include <fstream>
#include <list>

using namespace std;

const int NROWS = 4;
const int NLETTERS = 10;
const char TYPEWRITER[NROWS][NLETTERS] = {
               {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'},
               {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'},
               {'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';'},
               {'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '-'}  };

struct Pos {
   int row;
   int letter;

   Pos(int row = 0, int letter = 0) {
      this -> row = row;
      this -> letter = letter;
   }

   char Letter() {
      return TYPEWRITER[row][letter];
   }

   Pos & operator-(const Pos & other) {
      row = (row + NROWS - other.row) % NROWS;
      letter = (letter + NLETTERS - other.letter) % NLETTERS;
      return *this;
   }

   Pos & operator+(const Pos & other) {
      row = (row + other.row) % NROWS;
      letter = (letter + other.letter) % NLETTERS;
      return *this;
   }
};

ostream & operator<<(ostream & flujo, const Pos & p) {
   flujo << p.row << " " << p.letter;
   return flujo;
}

class Message {
private:
   list<char> letters;
   char key;

   Pos getPosition(char c) const {
      Pos pos(0, 0);
      while (pos.row < NROWS && pos.Letter() != c) {
         while (pos.letter < NLETTERS && pos.Letter() != c) {

            pos.letter++;
         }
         if (pos.letter == NLETTERS || pos.Letter() != c) {
            pos.row++;
            pos.letter = 0;
         }
      }
      return pos;
   }

public:

   void Initialize(char key, list<char> message) {
      this -> key = key;
      this -> letters = message;
   }

   list<char> Decode() {
      char l = letters.back();
      Pos shift = getPosition(key) - getPosition(l);

      list<char> decodedmessage;
      int c;
      for (list<char>::iterator i = letters.begin(); i != letters.end(); i++) {
         if ((*i) != ' ')
            c = (getPosition(*i) + shift).Letter();
         else
            c = *i;

         decodedmessage.push_back(c);
      }

      return decodedmessage;
   }


};


//Function to get the data if the file exits
bool GetData (string testfile, int & ncases, Message * & message) {
   bool error = false;

   //Open the test file
   fstream inputfile;
   inputfile.open (testfile.c_str() , ios::in);

   //Check if the file exists
   if (inputfile.is_open()){
     inputfile >> ncases;
     message = new Message [ncases];

     char key, c;
     for (int i = 0; i < ncases; i++){
        inputfile >> key;
        list<char> letters;
        inputfile.ignore();
        inputfile.get(c);
        while (c != '\n') {
           letters.push_back(c);
           inputfile.get(c);
        }
        message[i].Initialize(key, letters);
      }

      inputfile.close();
   }
   else
      error = true;

   return error;
}




//Show the results on screen
void ShowResults (list<char> results, int ncase) {
      cout << "Case #" << ncase + 1 << ": ";
      for (list<char>::iterator i = results.begin(); i != results.end(); i++) {
         cout << *i;
      }
      cout << endl;
}

int main () {
	const string FILENAME = "testInput";
   int ncases;
   Message * message;

   //Get the data
   bool posible_error = GetData(FILENAME, ncases, message);

   //If the data is correct
   if (!posible_error){
      list<char> result;
      for (int i = 0; i < ncases; i++){
         result = message[i].Decode();
         ShowResults(result, i);
      }


      delete [] message;
      message = 0;
   }
   else
      cout << "Test file is missing, the program can't do anything without it :(\n";


   return 0;
}
