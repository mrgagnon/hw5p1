/* Maylee Gagnon
 * CS 2223 HW5P1-3?
 * 11.26.2019
 */
#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

#define C 123
#define m 1000

/*
 *
 */
int ord(char c){
	if (isalpha(c)){
		return int(c);
	}
	else if (c == '\''){
		return 39;
	}
	return -1;
}

/*
 *
 */
int createHash(char* str){ //TODO how to pass in string properly
	int hash = 0;
	for (int i = 0; i < strlen(str); i++){
		hash = (hash*C +ord(str[i]))%m;
	}
	return hash;
}

/*
 *
 */
bool addToTable(){

}

int main() {
	int testHash = createHash("hello");
	cout << testHash;

	ifstream fin;
	fin.open("filename here");
	while(fin){
		//get a word, (print to test)
		//call createHash
		//call add to table, discard duplicates, flag for words not in first location thing when setting up table
	}
	fin.close();

	return 0;
}
