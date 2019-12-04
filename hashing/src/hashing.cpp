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
	int hashValue = -1;
};

/* Prints the given hash Table
 */
void printHashTable(hashEntry table[]){
	int useCt = 0;
	for (int i = 0; i < tableSize; i ++){
		useCt++;
		cout << i << "  :  " << table[i].word << "  :  " << table[i].hashValue << endl;
	}
	cout << "number of non-empty addresses: " << useCt << endl;
	long loadFactor = (long)useCt / (long)tableSize;
	cout << "load factor: " << loadFactor << endl;
}

/*
 *
 */
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

/*
 *
 */
void mostHashAddress(hashEntry table[]){
	int ct[tableSize];

	for (int i = 0; i < tableSize; i++){
		ct[i] = 0;
	}

	for (int i = 0; i < tableSize; i++){
		int tempVal = table[i].hashValue; //get where this string is suppose to be
		if (tempVal != -1){
			ct[tempVal] = ct[tempVal]+1; // increment count at the location by one
		}
	}

	int max = 0;
	int pos = 0;
	for (int i = 0; i < tableSize; i++){
		if (ct[i] > max){
			max = ct[i];
			pos = i;
		}
	}
	cout << "Hash Address with most words: "<< pos << " with " << max << " number of words" << endl;
}

/*
 *
 */
void furthest(hashEntry table[]){
	int max = 0;
	string word;
	int temp;
	for (int i = 0; i < tableSize; i++){
		if (i >= table[i].hashValue){//ie suppose to be at 20, but placed at 25,
			temp = i - table[i].hashValue;
		}
		else { //loops ie suppose to be at 998 but placed at 3
			temp = (tableSize-table[i].hashValue)+i;
		}
		if (temp > max){
			max = temp;
			word = table[i].word;
			//cout << word << endl;
		}
	}
	cout << word;
	cout << "Word furthest away" << word << " is " << max << " far away" << endl;
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
	if (str.size() == 0){//empty, punct was stripped
		return -1;
	}
	for (int i = 0; i < (int)str.size(); i++){
		int val = ord(str[i]);
		if (val !=-1){
			hash = (hash*C +ord(str[i]))%m;
		}
	}
	return hash;
}


/* Takes the given word and inserts it into the hashTable as appropriate
 * Discards duplicates and loops (closed hashing/open addressing)
 * @param word The word/string to be inserted
 * @param table The hash table where the word will be inserted into
 * @return True on completion, false otherwise
 */
bool addToTable(string w, hashEntry table[]){
	string word = w;
	int len = word.size();
	for (int i = 0; i < len; i++){ // removing excess punctuation
		if (ispunct(word[i]) && !(word[i] == '\'')){
			word.erase(i--,1);
			len = word.size();
		}
	}

	int tempHash = createHash(word);

	if (tempHash == -1){ //string is not a word ie '-', therefore doesn't need to inserted
		return true;
	}

	int pos = tempHash;
	while (pos != -1){
		if (pos >= tableSize){
			pos = 0;
		}
		if (table[pos].flag == 0){ // no duplicates and empty so insert here
			table[pos].flag = 1;
			table[pos].word = word;
			table[pos].hashValue = tempHash;
			pos = -1;
			return true;
		}
		else if ((word.compare(table[pos].word) == 0)){ // duplicate
			cout << word << endl;
			cout << table[pos].word << endl;
			pos = -1;
			return true;
		}
		else {
			pos++;
		}
	}// end while
	return false; //word not a duplicate and not inserted for some reason
}

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
	mostHashAddress(hashTable);
	furthest(hashTable);

	return 0;
}
