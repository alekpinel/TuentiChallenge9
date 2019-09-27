/*
	30/4/2019
	Made by Alejandro Pinel Martínez
	Tuenti Challenge 9
	Challenge 1 - Onion wars
*/

#include <iostream>
#include <fstream>

using namespace std;

class Problem {
private:
   //peoble who likes onion
   int inteligentpeople;
   //infra-humans who doesn't like onion
   int dumbidiots;

public:
   void Initialize(int data1, int data2) {
      inteligentpeople = data1;
      dumbidiots = data2;
   }

   int NTortillas() {
      return inteligentpeople/2 + inteligentpeople%2 + dumbidiots/2 + dumbidiots%2;
   }
};

//Function to get the data if the file exits
bool GetData (string testfile, int & ncases, Problem * & problem) {
   int MIN_CASES = 1;
   int MAX_CASES = 100;
   bool error = false;

   //Open the test file
   fstream inputfile;
   inputfile.open (testfile.c_str() , ios::in);

   //Check if the file exists
   if (inputfile.is_open()){

      inputfile >> ncases;

      if (ncases >= MIN_CASES && ncases <= MAX_CASES){
        problem = new Problem [ncases];

        int data1, data2;
        for (int i = 0; i < ncases; i++){
           inputfile >> data1;
           inputfile >> data2;

           problem[i].Initialize(data1, data2);
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
   Problem * problem;

   //Get the data
   bool posible_error = GetData(FILENAME, ncases, problem);

   //If the data is correct
   if (!posible_error){
      int result;

      for (int i = 0; i < ncases; i++){
         result = problem[i].NTortillas();
         ShowResults(result, i);
      }


      delete [] problem;
      problem = 0;
   }
   else
      cout << "Test file is missing, the program can't do anything without it :(\n";


   return 0;
}
