#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <ratio>
using namespace std;

vector<string> query;

// Trie node 
struct TrieNode{ 
     TrieNode* children[26];
     bool isEndOfWord; 
}; 

// Returns new trie node (initialized to NULLs)
TrieNode* getNode(void){
    TrieNode* pNode =  new TrieNode;
    pNode->isEndOfWord = false;
 
    for (int i = 0; i < 26; i++){
        pNode->children[i] = NULL;
    } 
    return pNode;
}
 
void insert(TrieNode* root, string key){
    TrieNode* pCrawl = root;
 
    for (unsigned i = 0; i < key.length(); i++){
        int index = int(key[i] - 'a');
        if (!pCrawl->children[index]){
            pCrawl->children[index] = getNode();
        }
        pCrawl = pCrawl->children[index];
    }

    pCrawl->isEndOfWord = true;
}

// Returns 0 if current node has a child
// If all children are NULL, return 1.
bool isLastNode(struct TrieNode* root)
{
    for (int i = 0; i < 26; i++)
        if (root->children[i])
            return 0;
    return 1;
}

// Recursive function to print auto-suggestions for given node.
void suggestionsRec(struct TrieNode* root, string& currPrefix){
    // found a string in Trie with the given prefix
    
        //cout << currPrefix << endl;
    //if (root->isEndOfWord)
        for (int i = 0; i < 26; i++)
            if (root->children[i]) {
                // child node character value
                char child = 'a' + i;
                currPrefix += child;
                suggestionsRec(root->children[i], currPrefix);
            }
}
 
int RangeSearch_Trie(TrieNode* root, string& pre){
    struct TrieNode* pCrawl = root;
    for (char c : pre) {
        int index = int(c - 'a');
 
        // no string in the Trie has this prefix
        if (!pCrawl->children[index])
            return 0;
        
        pCrawl = pCrawl->children[index];
    }
    //cout<<"Search Suggestions for ["<<pre<<"]:\n";
    // If prefix is present as a word, but
    // there is no subtree below the last
    // matching node.
    if (isLastNode(pCrawl)) {
        //cout << pre << endl;
        return -1;
    }
    suggestionsRec(pCrawl, pre);
    //cout<<"---------------------------------------------\n";
    return 1;
}

int main(){
	ifstream in;
	string pre;
	// setup query
	in.open("query.txt");
    while(in >> pre){
    	query.push_back(pre);
    }
    in.close();

    TrieNode* mytrie = getNode();

    // setup database 
    in.open("test_data.txt");
    string str;
    while(in >> str){
    	// trie version
    	insert(mytrie, str);
    }
    in.close();

    auto start_time = std::chrono::steady_clock::now();

    for(unsigned i=0;i<query.size();i++){
    	RangeSearch_Trie(mytrie, query[i]);
    }

    auto end_time = std::chrono::steady_clock::now();
    auto nano = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    
    cout << "Trie: elapsed time: " << nano.count() << "ns\n";
    return 0;
}