/*
	2/5/2019
	Made by Alejandro Pinel Martínez
	Tuenti Challenge 9
	Challenge 7 - Hash of cards
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

typedef unsigned char byte;

const int HASH_SIZE = 16;
const byte MIN = 48;
const byte MAX = 122;

vector<byte> notSoComplexHash(string inputText, int shift = 0) {
	vector<byte> hash(HASH_SIZE);
    for (int i = 0; i < HASH_SIZE; i++)
      hash[i] = (byte) 0x00;
    for (int i = 0; i < inputText.size(); i++) {
        hash[(i + shift) % 16] = (byte) (hash[(i + shift) % 16] + (byte) inputText[i]);
    }
    return hash;
}

vector<byte> AddHash(vector<byte> h1, vector<byte> h2) {
	for (int i = 0; i < HASH_SIZE; i++) {
		h1[i] += h2[i];
	}
	return h1;
}

vector<int> GetDifference(vector<byte> h1, vector<byte> h2) {
   vector<int> h3(HASH_SIZE);
	for (int i = 0; i < HASH_SIZE; i++) {
      h1[i] -= h2[i];
		h3[i] = h1[i];
	}

	return h3;
}

class Document {
private:
   string preamble;
   string print;
   string body;

   int getNV(int shift, int pos) {
      int nv = shift/HASH_SIZE;
      if (pos%HASH_SIZE < shift%HASH_SIZE)
         nv++;
      return nv;
   }

	bool IsInRange(vector<byte> originalhash, vector<byte> alteredhash, int shift) {
      vector<int> diff = GetDifference(originalhash, alteredhash);
      int nv;
		int start = preamble.size();
		bool possible = true;
		int pos;
		for (int i = 0; i < HASH_SIZE && possible; i++) {
			pos = (i + start) % HASH_SIZE;
			nv = getNV(shift, i);
         possible = false;

         while (diff[pos] <= (((int)MAX) * nv) && !possible) {
            if (diff[pos] >= MIN * nv)
               possible = true;
            else
               diff[pos] += 256;
         }


		}

		return possible;
	}

	int NCharRequired(vector<byte> originalhash) {
		vector<byte> alteredhash = Hash();
		int shift = 0;
		while (!IsInRange(originalhash, alteredhash, shift)) {
			shift++;
			alteredhash = AlteredHash(shift);
		}
		return shift;
	}

public:
	void initialize(string preamble, string body) {
		this -> preamble = preamble;
		this -> body = body;
	}

   void SetPrint(string p) {
      print = p;
   }

	string GetPrint() {
		return print;
	}

   string Text() {
      return preamble + print + body;
   }

   vector<byte> Hash() {
	   return notSoComplexHash(Text());
   }

   vector<byte> AlteredHash(int shift) {
	   vector<byte> hash = notSoComplexHash(preamble);
		hash = AddHash(hash, notSoComplexHash(body, preamble.size() + shift));
	   return hash;
   }

	void FitHash(vector<byte> hash) {
      int shift = NCharRequired(hash);

      vector<int> diff = GetDifference(hash, AlteredHash(shift));
      vector<byte> sol(shift);
		int start = preamble.size();
      int tofit, nv, newvalor, pos;

      for (int i = 0; i < HASH_SIZE; i++) {
         pos = (i + start)%HASH_SIZE;
         nv = getNV(shift, i);

         while (diff[pos] < MIN*nv){
            diff[pos] = diff[pos] + 256;
         }
      }

      for (int i = 0; i < shift; i++) {
         pos = (i + start)%HASH_SIZE;
         tofit = diff[pos];
         nv = getNV(shift, i);

         newvalor = tofit - (MAX * (nv - (i/HASH_SIZE) - 1));
         if (newvalor < MIN)
            newvalor = MIN;

         sol[i] = newvalor;
         diff[pos] -= newvalor;

		}
      string newprint;

      for (int i = 0; i < shift; i++) {
         newprint += sol[i];
      }

      print = newprint;
	}

	friend ostream & operator<<(ostream & flujo, const Document & p);
};

ostream & operator<<(ostream & flujo, const Document & p) {
   flujo << p.preamble << endl << p.print << endl << p.body;
   return flujo;
}

class HackermanWork {
private:
	Document original;
	Document fake;

public:
	void initialize(Document original, Document fake) {
		this -> original = original;
		this -> fake = fake;
	}

	string GetPrint() {
		fake.FitHash(original.Hash());
		return fake.GetPrint();
	}

   bool Test() {
      bool b = false;
      if (original.Hash() == fake.Hash())
         b = true;
      return b;
   }

};

//Function to get the data if the file exits
bool GetData (string testfile, int & ncases, HackermanWork * & problem) {
   bool error = false;

   //Open the test file
   fstream inputfile;
   inputfile.open (testfile.c_str() , ios::in);

   //Check if the file exists
   if (inputfile.is_open()){
     inputfile >> ncases;
     problem = new HackermanWork[ncases];

	  int nlines;
	  Document documents[2];
     for (int i = 0; i < ncases; i++){
		  for (int j = 0; j < 2; j++) {
			  inputfile >> nlines;
			  inputfile.ignore();
			  string line, otherline, preamble, body;
			  for (int i = 0; i < nlines; i++) {
				  getline(inputfile, otherline);
				  line.append(otherline);
			  }
			  int pos = line.find("---");
			  preamble = line.substr(0, pos + 3);
			  body = line.substr(pos + 3);
			  documents[j].initialize(preamble, body);
		  }
		  problem[i].initialize(documents[0], documents[1]);
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
   HackermanWork * problem;

   //Get the data
   bool posible_error = GetData(FILENAME, ncases, problem);

   //If the data is correct
   if (!posible_error){
      string result;
      for (int i = 0; i < ncases; i++){
			result = problem[i].GetPrint();
         if (!problem[i].Test())
            cout << "ERROR!!!!" << endl;
         ShowResults(result, i);
      }


      delete [] problem;
      problem = 0;
   }
   else
      cout << "Test file is missing, the program can't do anything without it :(\n";


   return 0;
}
