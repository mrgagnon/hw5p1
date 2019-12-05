/* Maylee Gagnon
 * CS 2223 HW5P1,P2,P3
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
		if (table[i].flag == 0){
			useCt++;
		}
		cout << i << "  :  " << table[i].word << "  :  " << table[i].hashValue << endl;
	}
	cout << "Number of non-empty addresses: " << useCt << endl;
	double loadFactor = (double)useCt / (double)tableSize;
	cout << "     Load factor: " << loadFactor << endl;
}


/* Looks through hashTable and finds and counts the longest fun of full and empty cells. Prints out results
 * @param table The hashTable to be examined
 */
void groupings(hashEntry table[]){
	int maxCtUsed = 0;
	int maxCtEmpty = 0;
	int maxStartUsed = 0;
	int maxStartEmpty = 0;
	int curCtUsed = 0;
	int curCtEmpty = 0;
	int curStartUsed = 0;
	int curStartEmpty = 0;

	int prevFlag;

	if (table[0].flag == 0){
		curCtEmpty = 1;
		prevFlag = 0;
	}
	else {
		curCtUsed = 1;
		prevFlag = 1;
	}

	for (int i = 1; i < tableSize; i++){
		if (table[i].flag == 0 && prevFlag == 0){
			curCtEmpty = curCtEmpty+1;
		}
		else if (table[i].flag == 1 && prevFlag == 1){
			curCtUsed = curCtUsed+1;
		}
		else if (table[i].flag == 0 && prevFlag == 1){
			curStartEmpty = i;
			curCtEmpty = 1;
			prevFlag = 0;

			if (curCtUsed > maxCtUsed){
				maxCtUsed = curCtUsed;
				maxStartUsed = curStartUsed;
			}
		}
		else if (table[i].flag == 1 && prevFlag == 0){
			curStartUsed = i;
			curCtUsed = 1;
			prevFlag = 1;

			if (curCtEmpty > maxCtEmpty){
				maxCtEmpty = curCtEmpty;
				maxStartEmpty = curStartEmpty;
			}
		}
	}

	cout << "Longest empty area: " << maxCtEmpty << endl;
	cout << "     starting at: " << maxStartEmpty << endl;
	cout << "Longest cluster: " << maxCtUsed << endl;
	cout << "     starting at: " << maxStartUsed << endl;
}

/* Looks through a hashTable and finds which hash address has the most number of words associated
 * @param table The hashTable to be searched
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
	cout << "Hash Address with most words: " << pos << endl;
	cout << "     Number of words: " << max << endl;
}

/* Finds the words furthest away from where it suppose to be in the table ie hash address and hash value are furthest apart
 * For words that loop they are counted moving fowared that looping back to 0 ie suppose to be at 998 but placed at 3 would be 5 not 995
 * @param table The hashTable to be checked
 */
void furthest(hashEntry table[]){
	int max = 0;
	string word;
	int temp;
	for (int i = 0; i < tableSize; i++){
		if (i >= table[i].hashValue && table[i].hashValue != -1){
			temp = i - table[i].hashValue;
		}
		else if (i < table[i].hashValue && table[i].hashValue != -1){ //loops
			temp = (tableSize-table[i].hashValue)+i;
		}
		if (temp > max){
			max = temp;
			word = table[i].word;
		}
	}
	cout << "Word furthest away is: " << word << endl;
	cout << "     Spaces away: " << max << endl;
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
	if (str.size() == 0){//empty only punct and was removed already
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

	if (tempHash == -1){ //string was not a word ie '-', therefore doesn't need to inserted
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
			pos = -1;
			return true;
		}
		else { // still looking for open spot or duplicate so next cell
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
