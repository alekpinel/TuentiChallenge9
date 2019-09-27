/*
	4/5/2019
	Made by Alejandro Pinel Martínez
	Tuenti Challenge 9
	Challenge 11 - Kepler-452
*/

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <list>

using namespace std;

const float PI = 3.14;

class Moon {
private:
   float distance;
   float rotation;
   float period;
   int unobtanium;

public:
   Moon(float d, float r, float p, int u) {
      distance = d;
      rotation = r;
      period = p;
      unobtanium = u;
   }

   Moon(const Moon & o) {
      distance = o.distance;
      rotation = o.rotation;
      period = o.period;
      unobtanium = o.unobtanium;
   }

   float Distance() const {
      return distance;
   }

   float Rotation() const {
      return rotation;
   }

   float Unobtanium() const {
      return unobtanium;
   }

   void Simulate(int hours) {
      rotation = rotation + (2*PI)*hours/period;
      while (rotation > 2*PI)
         rotation -= 2*PI;
   }

   float DistanceTo(const Moon & o) {
      float angle = max(rotation, o.rotation) - min(rotation, o.rotation);
      angle = min (angle, 2*PI - angle);
      //Law of cosines
      float d = sqrt(distance*distance + o.distance*o.distance - 2 * distance * o.distance * cos(angle));
      return d;
   }

   friend ostream & operator<<(ostream & flujo, const Moon & m);
};

ostream & operator<<(ostream & flujo, const Moon & m) {
   flujo << "Distance: " << m.distance << " Rotation: " << m.rotation << " Period: " << m.period << " Unobtanium: " << m.unobtanium;
   return flujo;
}

class Situation {
private:
   vector<Moon> moons;
   vector<bool> markedmoons;
   list<int> path;
   int pos;
   int capacity;
   int cargo;
   float range;


   void MarkMoon(int i) {
      markedmoons[i] = true;
      path.push_back(i);
   }

   void Load(int i) {
      cargo += moons[i].Unobtanium();
   }

   void Simulate(int hours = 6) {
      for (int i = 0; i < moons.size(); i++)
         moons[i].Simulate(hours);
   }

   void Copy(const Situation & o) {
      moons = o.moons;
      markedmoons = o.markedmoons;
      pos = o.pos;
      capacity = o.capacity;
      range = o.range;
      cargo = o.cargo;
      path = o.path;
   }

public:
   Situation (int p = -1, int c = 0, float r = 0) {
      pos = p;
      capacity = c;
      range = r;
      cargo = 0;
   }

   Situation(const Situation & o) {
      Copy(o);
   }

   Situation & operator=(const Situation & o) {
      Copy(o);
   }

   int Cargo() const {
      return cargo;
   }

   list<int> Path() const {
      return path;
   }

   list<int> CargoFromPath(list<int> path) {
      list<int> cargofrompath;
      for (list<int>::iterator i = path.begin(); i != path.end(); i++){
         cargofrompath.push_back(moons[*i].Unobtanium());
      }
      cargofrompath.sort();
      return cargofrompath;
   }

   int CountCargo(list<int> path) {
      int c = 0;
      for (list<int>::iterator i = path.begin(); i != path.end(); i++)
         c += moons[*i].Unobtanium();
      return c;
   }

   void Initialize(int c, float r, int p = -1) {
      capacity = c;
      range = r;
      pos = p;
   }

   void AddMoon(Moon m) {
      moons.push_back(m);
      markedmoons.push_back(false);
   }

   Situation Jump(int i) {
      Situation s(*this);
      s.MarkMoon(i);
      s.Load(i);
      if (s.pos == -1)
         s.range -= s.moons[i].Distance();
      else
         s.range -= s.moons[i].DistanceTo(s.moons[s.pos]);
      s.pos = i;
      s.Simulate();

      return s;
   }

   bool MoonIsViable(int i) {
      bool viable = false;
      if (!markedmoons[i])
         if ((capacity - cargo) >= moons[i].Unobtanium()) {
            float necessaryfuel;
            if (pos == -1)
               necessaryfuel = moons[i].Distance()*2;
            else
               necessaryfuel = moons[i].DistanceTo(moons[pos]) + moons[i].Distance();

            if (necessaryfuel <= range)
               viable = true;
         }
      return viable;
   }

   list<int> BestPath() {
      list<int> bestpath = path;
      int bestcargo = cargo;
      int othercargo;
      for (int i = 0; i < moons.size(); i++) {
         if (MoonIsViable(i)) {
            Situation s = Jump(i);
            list<int> newpath = s.BestPath();
            othercargo = CountCargo(newpath);
            if (othercargo > bestcargo) {
               bestcargo = othercargo;
               bestpath = newpath;
            }
         }
      }

      return bestpath;
   }

   list<int> BestCargo() {
      return CargoFromPath(BestPath());
   }

   friend ostream & operator<<(ostream & flujo, const Situation & s);
};

ostream & operator<<(ostream & flujo, const Situation & s) {
   flujo << "Capacity: " << s.capacity << " Cargo: " << s.cargo << " Range: " << s.range << " Pos: " << s.pos << endl;
   for (int i = 0; i < s.moons.size(); i++) {
      flujo << "Moon " << i << ":" << endl;
      flujo << s.moons[i] << endl;
      flujo << "Marked: " << s.markedmoons[i] << endl;
   }
   return flujo;
}


//Function to get the data if the file exits
bool GetData (string testfile, int & ncases, Situation * & problem) {
   bool error = false;

   //Open the test file
   fstream inputfile;
   inputfile.open (testfile.c_str() , ios::in);

   //Check if the file exists
   if (inputfile.is_open()){
     inputfile >> ncases;
     problem = new Situation [ncases];

     int nmoons, capacity;
     float range;
     float * distance;
     float * rotation;
     float * period;
     int * unobtanium;
     for (int i = 0; i < ncases; i++){
        inputfile >> nmoons;
        distance = new float[nmoons];
        rotation = new float[nmoons];
        period = new float[nmoons];
        unobtanium = new int[nmoons];

        for (int j = 0; j < nmoons; j++)
            inputfile >> distance[j];
         for (int j = 0; j < nmoons; j++)
             inputfile >> rotation[j];
         for (int j = 0; j < nmoons; j++)
             inputfile >> period[j];
         for (int j = 0; j < nmoons; j++)
             inputfile >> unobtanium[j];

         inputfile >> capacity >> range;

         problem[i].Initialize(capacity, range);
        for (int j = 0; j < nmoons; j++) {
           Moon m(distance[j], rotation[j], period[j], unobtanium[j]);
           problem[i].AddMoon(m);
        }

        delete [] distance;
        delete [] rotation;
        delete [] period;
        delete [] unobtanium;
      }

      inputfile.close();
   }
   else
      error = true;

   return error;
}


//Show the results on screen
void ShowResults (list<int> results, int ncase) {
      cout << "Case #" << ncase + 1 << ":";
      if (results.empty())
         cout << " None";
      else
         for (list<int>::iterator i = results.begin(); i != results.end(); i++ )
            cout << " " << *i;
      cout << endl;
}

int main () {
	const string FILENAME = "testInput";
   int ncases;
   Situation * problem;

   //Get the data
   bool posible_error = GetData(FILENAME, ncases, problem);

   //If the data is correct
   if (!posible_error){
      list<int> result;
      for (int i = 0; i < ncases; i++){
         //cout << problem[i];
         result = problem[i].BestCargo();
         ShowResults(result, i);
      }


      delete [] problem;
      problem = 0;
   }
   else
      cout << "Test file is missing, the program can't do anything without it :(\n";


   return 0;
}
