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
#define tableSize 1000

struct hashEntry{
	string word;
	int flag = 0;
	int hashValue = 0;
};

/* Prints the given hash Table
 * Skips empty cells
 */
void printHashTable(hashEntry table[]){
	int useCt = 0;
	for (int i = 0; i < tableSize; i ++){
		//cout << "Hash Address: " << i << " Hashed Word: " << table[i].word << " Hash Value: " << table[i].hashValue << endl;
		if (table[i].flag == 1){
			useCt++;
			cout << i << "     " << table[i].word << "     " << table[i].hashValue << endl;
		}
	}
	cout << "number of non-empty addresses: " << useCt << endl;
	long loadFactor = (long)useCt / (long)tableSize;
	cout << "load factor: " << loadFactor << endl;
}

void groupings(hashEntry table[]){
	int longestEmptyCt = 0;
	int emptyStart = 0;
	int longestFullCt = 0;
	int fullStart = 0;

	int curEmptyCt = 0;
	int curFullCt = 0;

	int prevFlag = table[0].flag;
	for (int i = 1; i < tableSize; i++){
		if (table[i].flag == 0 && prevFlag == 0){
			curEmptyCt++;
		}
		else if (table[i].flag == 1 && prevFlag == 1){
			curFullCt++;
		}
		else if (table[i].flag == 1 && prevFlag == 0){
			if (curEmptyCt > longestEmptyCt){
				longestEmptyCt = curEmptyCt;
				curEmptyCt = 0;
				emptyStart = i-longestEmptyCt;
				prevFlag = 1;
			}
		}
		else if (table[i].flag == 0 && prevFlag == 1){
			if (curFullCt >longestFullCt){
				longestFullCt = curFullCt;
				curFullCt = 0;
				fullStart = i-longestFullCt;
				prevFlag = 0;
			}
		}
	}
	cout << "Longest empty area: " << longestEmptyCt << " starting at: " << emptyStart << endl;
	cout << "Longest cluster: " << longestFullCt << " starting at: " << fullStart << endl;
}


/* Helper function to createHash()
 * @param c Letter to be converted
 * @return the ascii value of the given letter is[a,z],[A,Z],'apostrophe, -1 otherwise for other characters such as punctuation
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

/* Generates the hash value,
 * @param str The string to be stored
 * @return The value of the given string
 */
int createHash(string str){
	int hash = 0;
	for (int i = 0; i < (int)str.size(); i++){
		int val = ord(str[i]);
		if (val !=-1){
			hash = (hash*C +ord(str[i]))%m;
		}
		if (str.size() == 1 && !isalpha(str[0]) && str[0] != '\'') { //char is a value such as '-'
			return -1;
		}
	}
	return hash;
}

bool strCompare(string w1, string w2){

}

/* Takes the given word and inserts it into the hashTable as appropriate
 * Discards duplicates and loops (closed hashing/open addressing)
 * @param word The word/string to be inserted
 * @param table The hash table where the word will be inserted into
 * @return True on completion, false otherwise
 */
bool addToTable(string word, hashEntry table[]){
	int tempHash = createHash(word);
	cout << word << ": " << tempHash << endl;

	if (tempHash == -1){ //string is not a word ie '-', therefore doesn't need to inserted
		return true;
	}

	int pos = tempHash;
	while (pos != -1){
		if (pos >= tableSize){
			pos = 0;
		}
		if (table[tempHash].flag == 0){ // no duplicates and empty so insert here
			table[tempHash].flag = 1;
			table[tempHash].word = word;
			table[tempHash].hashValue = tempHash;
			return true;
		}
		else if (word.compare(table[pos].word)){ //TODO issue in general! issue with when punctuation is attached //compare strings, match = duplicate so stop
			pos = -1;
			return true;
		}
		else {
			pos++;
		}
	}// end while

	return false; //word not a duplicate and not inserted for some reason
}
/*
bool addToTable(string word, hashEntry table[]){
	int tempHash = createHash(word);
	cout << word << ": " << tempHash << endl;

	if (tempHash == -1){ //string is not a word ie '-', therefore doesn't need to inserted
		return true;
	}

	if (table[tempHash].flag == 0){ // first cell is empty, no duplicates and can insert here
		table[tempHash].flag = 1;
		table[tempHash].word = word;
		table[tempHash].hashValue = tempHash;
		return true;
	}
	else { //check cells for match (ie duplicate) or first empty cell (add new entry)
		int pos = tempHash;
		while (pos != -1){
			if (pos >= tableSize){
				pos = 0;
			}
			if (word.compare(table[pos].word)){ //TODO issue with when puncation is attached //compare strings, match = duplicate so stop
				cout << table[pos].word << "*** duplicate" << endl;
				pos = -1;
				return true;
			}
			else if (table[pos].flag == 0){ //cell is empty, no duplicate found and add entry here
				cout << "empty ----" << endl;
				table[pos].flag = 1;
				table[pos].word = word;
				table[pos].hashValue = tempHash;
				return true;
			}
			//else if (!(word.compare(table[pos].word)) && table[pos].flag == 1 ){ // cell full but not a match, so continue to next cell
			else {
				cout << table[pos].word << "****** skip to next" << endl;
				pos++;
			}
		}// end while
	}
	return false; //word not a duplicate and not inserted for some reason
}*/
//reclining 990 bends 991
//930 raven, from 928, its at 928 already

int main() {
	hashEntry hashTable[tableSize];

	fstream fin;
	fin.open("Raven.txt");
	string word;

	while(fin >> word){
		addToTable(word, hashTable);
	}
	fin.close();

	printHashTable(hashTable);
	groupings(hashTable);
	//TODO functions for p3 d&e!!!

	return 0;
}
