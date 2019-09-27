/*
	30/4/2019
	Made by Alejandro Pinel Martínez
	Tuenti Challenge 9
	Challenge 2 - Help Battlestar Galactica and save humanity
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <map>
#include <utility>
#include <string>

using namespace std;

const string INI = "Galactica";
const string END = "New Earth";

class Planet {
private:
   list<Planet *> conexions;
   int npaths;

public:
   Planet() {
      npaths = -1;
   }

   Planet(const Planet & other) {
      conexions = other.conexions;
      npaths = other.npaths;
   }

   void AddConexion(Planet * conexion) {
      conexions.push_back(conexion);
   }

   int getNPaths() {
      if (npaths == -1) {
         if (conexions.empty()) {
            npaths = 1;
         }
         else {
            npaths = 0;
            for (list<Planet *>::iterator i = conexions.begin(); i != conexions.end(); i++) {
               npaths += (*i) -> getNPaths();
            }
         }
      }
      return npaths;
   }
};

class Universe {
private:
   map<string, Planet> planets;

public:

   void AddPlanet(string name, list<string> conexions) {
      //If the planet isn't in the list
      if (planets.find(name) != planets.end()) {
         Planet planet;
         planets.insert(pair<string, Planet>(name, planet));
      }

      for (list<string>::iterator i = conexions.begin(); i != conexions.end(); i++) {
         //If the conected planet isn't in the list
         if (planets.find(name) != planets.end()) {
            Planet newplanet;
            planets.insert(pair<string, Planet>(*i, newplanet));
         }
         planets[name].AddConexion(&planets[*i]);
      }
   }

   int getNPaths() {
      return planets[INI].getNPaths();
   }

};

//Function to get the data if the file exits
bool GetData (string testfile, int & ncases, Universe * & universe) {
   int MIN_CASES = 1;
   int MAX_CASES = 1000000;
   bool error = false;

   //Open the test file
   fstream inputfile;
   inputfile.open (testfile.c_str() , ios::in);

   //Check if the file exists
   if (inputfile.is_open()){

      inputfile >> ncases;

      if (ncases >= MIN_CASES && ncases <= MAX_CASES){
        universe = new Universe [ncases];

        int nplanets;
        for (int i = 0; i < ncases; i++){
           inputfile >> nplanets;
           inputfile.ignore();
           for (int j = 0; j < nplanets; j++) {
             string line, name, other;
             getline(inputfile, line);
             stringstream linestream(line);
             getline(linestream, name, ':');
             list<string> conexions;
             while (linestream.good()) {
                getline(linestream, other, ',');
                conexions.push_back(other);
             }
             universe[i].AddPlanet(name, conexions);
           }
         }
      }

      inputfile.close();
   }
   else
      error = true;

   return error;
}




//Show the results on screen
void ShowResults (int results, int ncase) {
      cout << "Case #" << ncase + 1 << ": ";
      cout << results;
      cout << endl;
}

int main () {
	const string FILENAME = "submitInput";
   int ncases;
   Universe * universe;

   //Get the data
   bool posible_error = GetData(FILENAME, ncases, universe);

   //If the data is correct
   if (!posible_error){
      int result;

      for (int i = 0; i < ncases; i++){
         result = universe[i].getNPaths();
         ShowResults(result, i);
      }

      delete [] universe;
      universe = 0;
   }
   else
      cout << "Test file is missing, the program can't do anything without it :(\n";


   return 0;
}
