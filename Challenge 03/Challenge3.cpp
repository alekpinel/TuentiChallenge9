/*
	30/4/2019
	Made by Alejandro Pinel Martínez
	Tuenti Challenge 9
	Challenge 3 - Origami Punchout
*/

#include <iostream>
#include <fstream>
#include <list>

using namespace std;

struct Hole {
   int x;
   int y;

   void init(int x, int y) {
      this -> x = x;
      this -> y = y;
   }

   bool operator<(const Hole & other) const {
      return (x < other.x) || (x == other.x && y < other.y);
   }
};

class Paper {
private:
   int width;
   int heigh;
   list<char> folds;
   list<Hole> holes;

public:
   void Initialize(int w, int h, list<char> f, list<Hole> p) {
      width = w;
      heigh = h;
      folds = f;
      holes = p;
   }

   void Unfold() {
      for (list<char>::iterator i = folds.begin(); i != folds.end(); i++) {
         char fold = *i;
         list<Hole> newholes;
         for (list<Hole>::iterator j = holes.begin(); j != holes.end(); j++) {
            Hole hole1, hole2;
            int x = (*j).x;
            int y = (*j).y;
            switch (fold) {
               case 'T':
                  hole1.init(x, y + heigh);
                  hole2.init(x, heigh - y - 1);
                  break;
               case 'B':
                  hole1.init(x, y);
                  hole2.init(x, heigh * 2 - 1 - y);
                  break;
               case 'R':
                  hole1.init(x, y);
                  hole2.init(width * 2 - 1 - x, y);
                  break;
               case 'L':
                  hole1.init(x + width, y);
                  hole2.init(width - 1 - x, y);
                  break;
            }
            newholes.push_back(hole1);
            newholes.push_back(hole2);
         }
         holes = newholes;

         //Actualize width and heigh
         switch (fold) {
            case 'T':
            case 'B':
               heigh *= 2;
               break;
            case 'R':
            case 'L':
               width *= 2;
               break;
         }
      }
   }

   list<Hole> getHoles() {
      holes.sort();
      return holes;
   }
};

//Function to get the data if the file exits
bool GetData (string testfile, int & ncases, Paper * & paper) {
   bool error = false;

   //Open the test file
   fstream inputfile;
   inputfile.open (testfile.c_str() , ios::in);

   //Check if the file exists
   if (inputfile.is_open()){

      inputfile >> ncases;
      paper = new Paper [ncases];

     int w, h, f, p;
     char chr;
     Hole hole;
     for (int i = 0; i < ncases; i++){
        inputfile >> w >> h >> f >> p;
        list<char> folds;
        for (int j = 0; j < f; j++){
           inputfile >> chr;
           folds.push_back(chr);
        }
        list<Hole> holes;
        for (int j = 0; j < p; j++){
           inputfile >> hole.x >> hole.y;
           holes.push_back(hole);
        }

        paper[i].Initialize(w, h, folds, holes);
      }

      inputfile.close();
   }
   else
      error = true;

   return error;
}




//Show the results on screen
void ShowResults (list<Hole> results, int ncase) {
      cout << "Case #" << ncase + 1 << ":" << endl;
      for (list<Hole>::iterator i = results.begin(); i != results.end(); i++) {
         cout << (*i).x << " " << (*i).y << endl;
      }
}

int main () {
	const string FILENAME = "submitInput";
   int ncases;
   Paper * paper;

   //Get the data
   bool posible_error = GetData(FILENAME, ncases, paper);

   //If the data is correct
   if (!posible_error){

      for (int i = 0; i < ncases; i++){
         paper[i].Unfold();
         ShowResults(paper[i].getHoles(), i);
      }


      delete [] paper;
      paper = 0;
   }
   else
      cout << "Test file is missing, the program can't do anything without it :(\n";


   return 0;
}
