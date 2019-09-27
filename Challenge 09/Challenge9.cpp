/*
	30/4/2019
	Made by Alejandro Pinel Martínez
	Tuenti Challenge 9
	Challenge 9 - Helping Nobita
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>

using namespace std;

int Traduce (string c) {
   int i;
   if (c.compare("一") == 0)
      i = 1;
   else if (c.compare("二") == 0)
      i = 2;
   else if (c.compare("三") == 0)
      i = 3;
   else if (c.compare("四") == 0)
      i = 4;
   else if (c.compare("五") == 0)
      i = 5;
   else if (c.compare("六") == 0)
      i = 6;
   else if (c.compare("七") == 0)
      i = 7;
   else if (c.compare("八") == 0)
      i = 8;
   else if (c.compare("九") == 0)
      i = 9;
   else if (c.compare("十") == 0)
      i = 10;
   else if (c.compare("百") == 0)
      i = 100;
   else if (c.compare("千") == 0)
      i = 1000;
   else if (c.compare("万") == 0)
      i = 10000;

   return i;
}

bool IsMultiple(int i) {
   return (i == 10 || i == 100 || i == 1000 || i == 10000);
}

struct Digits {
   list<int> basic;
   list<int> multiple;
};

class Homework {
private:
   string op1;
   string op2;
   string result;
   char operat;

   Digits GetDigit(string s) {
      Digits d;
      int n;
      for (int i = 0; i < s.size(); i+=3) {
         n = Traduce(s.substr(i, 3));
         if (IsMultiple(n))
            d.multiple.push_back(n);
         else
            d.basic.push_back(n);
      }
      d.multiple.sort();
      return d;
   }

   list<int> Posibilities(int actual, Digits digits) {
      list<int> results;
      /*
      cout << "iteracion actual: " << actual << endl;
      for (list<int>::iterator i = digits.basic.begin(); i != digits.basic.end(); i++) {
         cout << *i << " ";
      }
      cout << endl;
      for (list<int>::iterator i = digits.multiple.begin(); i != digits.multiple.end(); i++) {
         cout << *i << " ";
      }
      cout << endl;
      */

      //There are basic
      if (!digits.basic.empty()) {
         //No operators
         if (digits.multiple.empty()) {
            //If only one basic remains
            if (digits.basic.size() == 1){
               results.push_back(actual + digits.basic.back());
               digits.basic.pop_back();
            }
         }
         //Operators and Basics
         else {
            //Take one operator
            if (digits.multiple.back() != 10000) {
               Digits optionA = digits;
               int n = actual + optionA.multiple.back();
               optionA.multiple.pop_back();
               list<int> other = Posibilities(n, optionA);
               results.insert(results.end(), other.begin(), other.end());
            }
            //Take one operator * one basic
            int oper, val, it = 0;
            for (list<int>::iterator i = digits.basic.begin(); i != digits.basic.end(); i++) {
               Digits optionB = digits;
               oper = optionB.multiple.back();
               optionB.multiple.pop_back();
               val = *i;
               if (oper == 10000 || val != 1) {
                  oper = oper * val;
                  oper += actual;
                  //Remove from optionB
                  list<int>::iterator x = optionB.basic.begin();
                  for (int j = 0; j < it; j++)
                     x++;
                  x = optionB.basic.erase(x);

                  list<int> other = Posibilities(oper, optionB);
                  results.insert(results.end(), other.begin(), other.end());

               }
               it++;
            }
          }
      }
      else {
         //Only Operators
         if (!digits.multiple.empty()) {
            if (digits.multiple.back() != 10000) {
               int n = actual;
               while (!digits.multiple.empty()) {
                  n += digits.multiple.back();
                  digits.multiple.pop_back();
               }
               results.push_back(n);
            }
         }
         //Nothing
         else {
            results.push_back(actual);
         }
      }
      return results;
   }

public:
   void initialize(string op1, string op2, string result) {
      this -> op1 = op1;
      this -> op2 = op2;
      this -> result = result;
      operat = '/';
   }

   string Adjust() {
      list<int> posso1 = PosibleTraduction(op1);
      list<int> posso2 = PosibleTraduction(op2);
      list<int> possres = PosibleTraduction(result);
      list<int>::iterator i = posso1.begin(), j = posso2.begin(), k = possres.begin();
      int op = 0;
      bool found = false;
      string traduction;

      while (i != posso1.end() && !found) {
         j = posso2.begin();
         while (j != posso2.end() && !found) {
            k = possres.begin();
            while (k != possres.end() && !found) {
               op = 0;
               while (op < 3 && !found) {
                  switch (op) {
                     case 0:
                        found = (*i) + (*j) == (*k);
                        break;
                     case 1:
                        found = (*i) - (*j) == (*k);
                        break;
                     case 2:
                        found = (*i) * (*j) == (*k);
                        break;
                  }
                  traduction.clear();
                  if (!found)
                     op++;
               }
               if (!found)
                  k++;
            }
            if (!found)
               j++;
         }
         if (!found)
            i++;
      }
      traduction.append(to_string(*i));
      switch (op) {
         case 0: traduction.append(" + "); break;
         case 1: traduction.append(" - "); break;
         case 2: traduction.append(" * "); break;
      }
      traduction.append(to_string(*j));
      traduction.append(" = ");
      traduction.append(to_string(*k));
      return traduction;
   }

   list<int> PosibleTraduction(string s) {
      list<int> posibilities;
      Digits digit = GetDigit(s);

      return Posibilities(0, digit);
   }

   friend ostream & operator<<(ostream & flujo, const Homework & p);
};

ostream & operator<<(ostream & flujo, const Homework & p) {
   flujo << p.op1 << " " << p.operat << " " << p.op2 << " = " << p.result << endl;
   return flujo;
}

//Function to get the data if the file exits
bool GetData (string testfile, int & ncases, Homework * & problem) {
   bool error = false;

   //Open the test file
   fstream inputfile;
   inputfile.open (testfile.c_str() , ios::in);

   //Check if the file exists
   if (inputfile.is_open()){
     inputfile >> ncases;
     problem = new Homework [ncases];

     string op1, op2, result;
     for (int i = 0; i < ncases; i++){
        inputfile >> op1 >> op2 >> op2 >> result >> result;
        problem[i].initialize(op1, op2, result);
      }

      inputfile.close();
   }
   else
      error = true;

   return error;
}




//Show the results on screen
void ShowResults (string results, int ncase) {
      cout << "Case #" << ncase + 1 << ": " << results << endl;
}

int main () {
	const string FILENAME = "submitInput";
   int ncases;
   Homework * problem;

   //Get the data
   bool posible_error = GetData(FILENAME, ncases, problem);

   //If the data is correct
   if (!posible_error){
      string result;
      for (int i = 0; i < ncases; i++){
         result = problem[i].Adjust();
         ShowResults(result, i);
      }


      delete [] problem;
      problem = 0;
   }
   else
      cout << "Test file is missing, the program can't do anything without it :(\n";


   return 0;
}
