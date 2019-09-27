/*
	5/5/2019
	Made by Alejandro Pinel Martínez
	Tuenti Challenge 9
	Challenge 13 - Character Six
*/

#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <set>
#include <vector>

using namespace std;

class Character;

struct Fusion {
   Character * c1;
   Character * c2;

   void Initialize(Character * c1, Character * c2) {
      this -> c1 = c1;
      this -> c2 = c2;
   }
};

class Character {
private:
   string name;
   int cost;
   int level;
   set<string> skills;
   vector<Fusion> fusions;

public:
   void Initialize(string n, int l, int c, set<string> s) {
      name = n;
      cost = c;
      level = l;
      skills = s;
   }

   void AddFusion(Fusion f) {
      fusions.push_back(f);
   }

   int getNFusions() const {
      return fusions.size();
   }

   Fusion getFusion(int n) const {
      return fusions[n];
   }

   set<string> & getSkills() {
      return skills;
   }

   int getPrice() const{
      return cost;
   }

   string getName() const {
      return name;
   }

   bool operator<(const Character & o) const {
      return level < o.level;
   }

   friend ostream & operator<<(ostream & flujo, Character & c);
};

ostream & operator<<(ostream & flujo, Character & c) {
   flujo << c.name << " Cost: " << c.cost << " Level: " << c.level << " Skills:";
   for (set<string>::iterator i = c.getSkills().begin(); i != c.getSkills().end(); i++)
      cout << *i << " ";
   return flujo;
}

struct Option {
   int cost;
   set<string> skills;

   void Combine(Option o) {
      cost += o.cost;
      for (set<string>::iterator i = o.skills.begin(); i != o.skills.end(); i++) {
         skills.insert(*i);
      }
   }

   bool IsComparable(Option o) {
      bool iscomparable = true;
      for (set<string>::iterator i = skills.begin(); i != skills.end() && iscomparable; i++) {
         if (o.skills.find(*i) == o.skills.end())
            iscomparable = false;
      }
      return iscomparable;
   }

   bool SkillsCovered(Option o) {
      bool iscomparable = true;
      for (set<string>::iterator i = o.skills.begin(); i != o.skills.end() && iscomparable; i++) {
         if (skills.find(*i) == skills.end())
            iscomparable = false;
      }
      return iscomparable;
   }

   bool IsUpgraded(Option o) {
      if (o.cost < cost)
         return true;
      else
         return false;
   }
};

ostream & operator<<(ostream & flujo, Option & c) {
   flujo << c.cost << " skills: ";
   for (set<string>::iterator i = c.skills.begin(); i != c.skills.end(); i++)
      flujo << *i << " ";
   return flujo;
}

class ListOption {
private:
   Character * c;
   int gold;
   Option bestcost;
   list<Option> coveredskills;
   set<string> desiredskills;

public:

   ListOption() {
      c = 0;
      gold = 0;
      bestcost.cost = 0;
   }

   ListOption(Character * chr, int g, set<string> s){
      c = chr;
      gold = g;
      desiredskills = s;
      bestcost.cost = 0;
   }

   ListOption(const ListOption & l) {
      c = l.c;
      gold = l.gold;
      bestcost = l.bestcost;
      coveredskills = l.coveredskills;
      desiredskills = l.desiredskills;
   }

   list<Option> getOptions() {
      list<Option> l = coveredskills;
      if (bestcost.cost != 0)
         l.push_front(bestcost);
      return l;
   }

   list<Option> getOptions(set<string> inherit) {
      list<Option> l = getOptions();

      for (set<string>::iterator i = inherit.begin(); i != inherit.end(); i++) {
         if (desiredskills.find(*i) != desiredskills.end()) {
            for (list<Option>::iterator j = l.begin(); j != l.end(); j++) {
               (*j).skills.insert(*i);
            }
         }
      }

      return l;
   }

   void Inherit(set<string> inherit) {
      for (set<string>::iterator i = inherit.begin(); i != inherit.end(); i++) {
         if (desiredskills.find(*i) != desiredskills.end()) {
            for (list<Option>::iterator j = coveredskills.begin(); j != coveredskills.end(); j++) {
               (*j).skills.insert(*i);
            }
            if (bestcost.cost != 0)
               bestcost.skills.insert(*i);
         }
      }
   }

   void Add(int cst, set<string> sk) {
      Option o;
      o.cost = cst;
      for (set<string>::iterator i = sk.begin(); i != sk.end(); i++) {
         if (desiredskills.find(*i) != desiredskills.end())
            o.skills.insert(*i);
      }

      Add(o);
   }

   void Add(ListOption & l) {
      list<Option> lst = l.getOptions();
      for (list<Option>::iterator i = lst.begin(); i != lst.end(); i++)
         Add(*i);
   }

   void Add(Option o) {
      if (o.cost <= gold) {
         if (bestcost.cost == 0 || bestcost.cost > o.cost)
            bestcost = o;

         //Optimizable
         if (!o.skills.empty()) {
            bool include = true;

            list<Option>::iterator i = coveredskills.begin();
            while ( i != coveredskills.end()) {
               if ((*i).IsComparable(o)) {
                  if ((*i).IsUpgraded(o)) {
                     i = coveredskills.erase(i);
                  }
                  else {
                     if ((*i).SkillsCovered(o)) {
                        include = false;
                     }
                     i++;
                  }
               }
               else
                  i++;
            }

            if (include)
               coveredskills.push_back(o);
         }
      }
   }

   ListOption Combine(ListOption l) {
      ListOption final(c, gold, desiredskills);
      list<Option> options1 = getOptions();
      list<Option> options2 = l.getOptions();

      for (list<Option>::iterator i = options1.begin(); i != options1.end(); i++) {
         for (list<Option>::iterator j = options2.begin(); j != options2.end(); j++) {
            Option o = *i;
            o.Combine(*j);
            final.Add(o);
         }
      }
      return final;
   }

   int BestPrize() {
      list<Option> options = getOptions();
      int bestcost = -1;
      for (list<Option>::iterator i = options.begin(); i != options.end(); i++) {
         if (IsComplete(*i) && (bestcost == -1 || bestcost > (*i).cost))
            bestcost = (*i).cost;
      }
      return bestcost;
   }

   bool IsComplete(Option o) {
      bool complete = true;
      for (set<string>::iterator i = desiredskills.begin(); i != desiredskills.end() && complete; i++) {
         if (o.skills.find(*i) == o.skills.end())
            complete = false;
      }
      return complete;
   }

   friend ostream & operator<<(ostream & flujo, ListOption & c);
};

ostream & operator<<(ostream & flujo, ListOption & c) {
   flujo << "Character: " << c.c -> getName() << endl;
   list<Option> options = c.getOptions();
   for (list<Option>::iterator i = options.begin(); i != options.end(); i++) {
      flujo << *i << endl;
   }
   return flujo;
}

class DesiredCharacter {
private:
   Character * character;
   int gold;
   set<string> desiredskills;
   set<string> realskills;
   list<Character *> bought;

   bool Division(Character * charactertodivide, int nfusion) {
      Fusion newcharacter = charactertodivide -> getFusion(nfusion);
      bool stop = false;
      for (list<Character *>::iterator i = bought.begin(); i != bought.end() && !stop; i++) {
         if (*i == charactertodivide) {
            bought.erase(i);
            stop = true;
         }
      }
      Character * c1 = newcharacter.c1;
      Character * c2 = newcharacter.c2;
      bought.push_back(c1);
      bought.push_back(c2);


      AddSkills(c1);
      AddSkills(c2);

      return HaveSkillsDesired(c1) || HaveSkillsDesired(c2);
   }

   void AddSkills(Character * c) {
      for (set<string>::iterator i = c -> getSkills().begin(); i != c -> getSkills().end(); i++)
         realskills.insert(*i);
   }

   bool HaveSkillsDesired(Character * c) {
      bool have = false;
      for (set<string>::iterator i = c -> getSkills().begin(); i != c -> getSkills().end() && !have; i++)
         have = desiredskills.find(*i) != desiredskills.end();
      return have;
   }

public:

   void Initialize (Character * c, int g, set<string> d) {
      character = c;
      gold = g;
      desiredskills = d;
      bought.push_back(character);
      AddSkills(character);
   }

   bool CorrectSkills() {
      bool correct = true;
      for (set<string>::iterator i = desiredskills.begin(); i != desiredskills.end() && correct; i++)
         correct = realskills.find(*i) != realskills.end();

      return correct;
   }

   int TotalPrice() const {
      int price = 0;
      for (list<Character *>::const_iterator i = bought.begin(); i != bought.end(); i++) {
         price += (*i) -> getPrice();
      }
      return price;
   }

   bool CanAfford () const {
      return TotalPrice() <= gold;
   }


   struct Node {
      Character * ch;
      set<string> inherit;

      bool operator<(const Node & o) const{
         return ch < o.ch || (ch == o.ch && inherit < o.inherit);
      }
   };

   int CalculatePrize() {
      map<Node, ListOption> options;
      set<string> inherit;
      list<Character *> fathers;
      ListOption l = getOption(character, options, inherit);
      int bestprize = l.BestPrize();
      return bestprize;
   }
/*
   list<Option> searchRealOptions(Character * chr, map<Character *, ListOption> & options, set<string> inherit) {
      //Skills de herencia
      for (set<string>::iterator i = chr -> getSkills().begin(); i != chr -> getSkills().end(); i++) {
         if (desiredskills.find(*i) != desiredskills.end()) {
            inherit.insert(*i);
         }
      }

      ListOption l(chr, gold, desiredskills);
      l.Add(chr -> getPrice(), chr -> getSkills());
      for (int i = 0; i < chr -> getNFusions(); i++) {
         list<string> options1, options2;
         Fusion f = chr -> getFusion(i);


         options1 = options[f.c1].getOptions(inherit);
         options2 = options[f.c1].getOptions(inherit);

         l2 = l2.Combine(l3);
         l.Add(l2);

   }*/

   ListOption getOption(Character * chr, map<Node, ListOption> & options, set<string> inherit) {
      ListOption l(chr, gold, desiredskills);
      Node node;
      node.inherit = inherit;
      node.ch = chr;
      if (options.find(node) != options.end()) {
         l = options[node];
      }
      else {
         l.Add(chr -> getPrice(), chr -> getSkills());
         for (int i = 0; i < chr -> getNFusions(); i++) {
            ListOption l2(chr, gold, desiredskills), l3(chr, gold, desiredskills);
            list<string> options1, options2;
            Fusion f = chr -> getFusion(i);

            l2 = getOption(f.c1, options, inherit);

            l3 = getOption(f.c2, options, inherit);

            l2 = l2.Combine(l3);

            l.Add(l2);
         }

         //Skills de herencia
         for (set<string>::iterator i = chr -> getSkills().begin(); i != chr -> getSkills().end(); i++) {
            if (desiredskills.find(*i) != desiredskills.end()) {
               inherit.insert(*i);
            }
         }
         l.Inherit(inherit);
         options[node] = l;
      }
      /*
      cout << "Fusions de " << chr -> getName() << endl;
      for (int i = 0; i < (chr) -> getNFusions(); i++) {
         Fusion f = chr -> getFusion(i);
         cout << f.c1 -> getName() << " " << f.c2 -> getName() << endl;
      }
      cout << l;
      */
      return l;
   }

   string toString() {
      string s;
      for (list<Character *>::iterator i = bought.begin(); i != bought.end(); i++) {
         s.append((*i) -> getName());
         s.append(" ");
      }
      return s;
   }
};


bool GetAlmanac (map<string, Character> & almanac) {
   bool error = false;

   //Open the test file
   fstream inputfile;
   inputfile.open ("almanac.data" , ios::in);

   int ncharacters, nfusions, level, cost, nskills;
   string name, skill;
   //Check if the file exists
   if (inputfile.is_open()){
     inputfile >> ncharacters >> nfusions;

     for (int i = 0; i < ncharacters; i++){
        inputfile >> name >> level >> cost >> nskills;
        set<string> skills;
        for (int j = 0; j < nskills; j++) {
           inputfile >> skill;
           skills.insert(skill);
        }
        Character c;
        c.Initialize(name, level, cost, skills);
        almanac.insert(pair<string, Character>(name, c));
      }

      Character * c1, * c2;
      string name1, name2, name3;
      for (int i = 0; i < nfusions; i++) {
         inputfile >> name1 >> name2 >> name3;
         Fusion f;
         f.Initialize(&almanac[name2], &almanac[name3]);
         almanac[name1].AddFusion(f);
      }

      inputfile.close();
   }
   else
      error = true;

   return error;
}

//Function to get the data if the file exits
bool GetData (string testfile, int & ncases, DesiredCharacter * & problem, map<string, Character> & almanac) {
   bool error = false;

   //Open the test file
   fstream inputfile;
   inputfile.open (testfile.c_str() , ios::in);

   //Check if the file exists
   if (inputfile.is_open()){
     inputfile >> ncases;
     problem = new DesiredCharacter [ncases];

     string name, skill;
     int gold, nskills;
     for (int i = 0; i < ncases; i++){
        inputfile >> gold >> name >> nskills;
        set<string> skills;
        for (int j = 0; j < nskills; j++) {
           inputfile >> skill;
           skills.insert(skill);
        }
        problem[i].Initialize(&almanac[name], gold, skills);
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
      if (results == -1)
         cout << "IMPOSSIBLE";
      else
         cout << results;
      cout << endl;
}

int main () {
	const string FILENAME = "submitInput";
   int ncases;
   DesiredCharacter * problem;

   //Get the almanac
   map<string, Character> almanac;
   bool posible_error = GetAlmanac(almanac);

   //Get the data
   posible_error = GetData(FILENAME, ncases, problem, almanac);

   //If the data is correct
   if (!posible_error){
      int result;
      for (int i = 0; i < ncases; i++){
         result = problem[i].CalculatePrize();
         ShowResults(result, i);
      }


      delete [] problem;
      problem = 0;
   }
   else
      cout << "Test file is missing, the program can't do anything without it :(\n";


   return 0;
}
