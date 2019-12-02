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

void printHashTable(hashEntry table[]){
	for (int i = 0; i < tableSize; i ++){
		//cout << "Hash Address: " << i << " Hashed Word: " << table[i].word << " Hash Value: " << table[i].hashValue << endl;
		cout << i << ", " << table[i].word << ", " << table[i].hashValue << endl;
	}
}

/* Helper function to createHash()
 *
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
	return -1; //TODO fix return value for non letter values, this still affects the result, need to skip entirely
}

/* Generates the hash value,
 * @param str The string to be stored
 * @return The value of the given string
 */
int createHash(string str){
	int hash = 0;
	for (int i = 0; i < str.size(); i++){
		hash = (hash*C +ord(str[i]))%m;
	}
	return hash;
}

/* Takes the given word and inserts it into the hashTable as appropriate
 * Discards duplicates and loops (closed hashing/open addressing)
 * @param word The word/string to be inserted
 * @param table The hash table where the word will be inserted into
 * @return True if successful false otherwise TODO fix return values
 */
bool addToTable(string word, hashEntry table[]){
	int tempHash = createHash(word);
	cout << word << ": " << tempHash << endl;

	if (tempHash < 0){ //TODO string is not a word to be added ie '-'
		return true;
	}

	if (table[tempHash].flag == 0){ // pos is empty, no duplicates and can insert here
		table[tempHash].flag = 1;
		table[tempHash].word = word;
		table[tempHash].hashValue = tempHash;
	}
	else {
		//word is not a duplicate, check following cells until match(search success, duplicate) or empty cell(search fail and add new entry here)
		if (!(word.compare(table[tempHash].word))){
			int pos = tempHash+1;
			while (pos != -1){
				if (pos >= tableSize){
					pos = 0;
				}
				if (word.compare(table[pos].word)){ // compare strings, match = duplicate so stop
					pos = -1;
				}
				else if (table[pos].flag == 0){ //cell is empty, no duplicate found and add entry here
					table[pos].flag = 1;
					table[pos].word = word;
					table[pos].hashValue = tempHash;
				}
			}
		}
	}
	return true;
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

	//printHashTable(hashTable);

	return 0;
}
