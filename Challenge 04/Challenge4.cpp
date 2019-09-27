/*
	30/4/2019
	Made by Alejandro Pinel Martínez
	Tuenti Challenge 9
	Challenge 4 - Candy patterns
*/

#include <iostream>
#include <fstream>
#include <vector>
#include "BigInteger.h"

using namespace std;

BigInteger mcd(BigInteger num1, BigInteger num2) {
    BigInteger mcd = 0;
    BigInteger a = num1 > num2? num1: num2;
    BigInteger b = num1 > num2? num2: num1;
    do {
        mcd = b;
        b = a%b;
        a = mcd;
    } while (b != 0);
    return mcd;
}

BigInteger mcm(BigInteger num1, BigInteger num2) {
   BigInteger mcm = 0;
   BigInteger a = num1 > num2? num1: num2;
   BigInteger b = num1 > num2? num2: num1;
   mcm = (a/mcd(a,b))*b;
   return mcm;
}

struct fraction {
   BigInteger num;
   BigInteger den;

   void ini(BigInteger num, BigInteger den) {
      this -> num = num;
      this -> den = den;
   }

   void reduce() {
      BigInteger x = mcd(num, den);
      num = num / x;
      den = den / x;
   }
};

class CandyList {
private:
   vector<BigInteger> candies;

public:

   void AddCandy(int candy) {
      if (candy > candies.size()) {
         candies.resize(candy, 0);
      }
      candies[candy - 1] += 1;
   }

   fraction Ratio() {
      int size = candies.size();
      BigInteger X = 1;
      BigInteger n_candies, remainder;
      for (int i = 0; i < size; i++) {
         n_candies = i + 1;
         remainder = candies[i] % n_candies;
         if (remainder == 0) {
            n_candies = 1;
         }
         else if (n_candies % remainder == 0) {
            n_candies = n_candies / remainder;
         }
         X = mcm(X, n_candies);
      }


      //Count candies and people
      BigInteger total_people = 0;
      BigInteger total_candies = 0;
      for (int i = 0; i < size; i++) {
         total_candies += candies[i] * X;
         total_people += (candies[i] * X) / (i + 1);
      }

      fraction total;
      total.ini(total_candies, total_people);
      total.reduce();

      return total;
   }

};


//Function to get the data if the file exits
bool GetData (string testfile, int & ncases, CandyList * & problem) {
   bool error = false;

   //Open the test file
   fstream inputfile;
   inputfile.open (testfile.c_str() , ios::in);

   //Check if the file exists
   if (inputfile.is_open()){
     inputfile >> ncases;
     problem = new CandyList [ncases];

     int ncandies, candy;
     for (int i = 0; i < ncases; i++){
        inputfile >> ncandies;
        for (int j = 0; j < ncandies; j++) {
           inputfile >> candy;
           problem[i].AddCandy(candy);
        }
      }
      inputfile.close();
   }
   else
      error = true;

   return error;
}


//Show the results on screen
void ShowResults (fraction results, int ncase) {
      cout << "Case #" << ncase + 1 << ": " << ((string) results.num) << "/" << ((string) results.den) << endl;
}

int main () {
	const string FILENAME = "submitInput";
   int ncases;
   CandyList * candylist;

   //Get the data
   bool posible_error = GetData(FILENAME, ncases, candylist);

   //If the data is correct
   if (!posible_error){
      fraction result;
      for (int i = 0; i < ncases; i++){
         result = candylist[i].Ratio();
         ShowResults(result, i);
      }


      delete [] candylist;
      candylist = 0;
   }
   else
      cout << "Test file is missing, the program can't do anything without it :(\n";


   return 0;
}
