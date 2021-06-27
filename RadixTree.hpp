#ifndef RADIX_TREE
#define RADIX_TREE

#include <string.h>
#include <tuple>
#include <iostream>

class TreeNode
{ /*****START OF TREENODE DEFINTION*****/
private:
	char* prefix;
	//legnth of the prefix
	int length;
	//head of the linked list of the children (traversing vertically)
	TreeNode* children;
	//next sibling in the current linked list (traversing horizontally)
	TreeNode* next; 
	int freq;
	bool isWord;
public:
	TreeNode(char* newPrefix, int l, int f , bool isWord) {
		prefix = new char[l+1];
		children = nullptr;
		next = nullptr;
		memcpy(prefix, newPrefix, l);
		prefix[l] = '\0';
		length = l;
		freq = f;
		this->isWord = isWord;
	}

	~TreeNode() {
		delete[] prefix;
	}

	friend class Radix;
}; /*****END OF TREENODE DEFINTION*****/

class Radix /***BEGINGING OF RADIX TREE DEFINTION***/
{
private:

	/*****MEMBER DATA*****/
	TreeNode* root;

	
	/*****HELPER FUNCTIONS*****/

	std::tuple<TreeNode*, bool,bool> find(TreeNode*& parent, TreeNode* curr, char*& key, int& startingIndex);
	/*----------------------------------------------------------------------------------------------
	Parameters
		- parent: TreeNode pointer passed ny reference to save the parent of the found node in it
		- curr: The starting node of the search (usually the root)
		- key: The the input word to be looked for in the tree
		- StartingIndex: The index at which comparing starts passed by reference to save the changes

	Output:
		-TreeNode pointer in case the word is found the last node representing the word is returned
			otherwise return the node at which the word should be inserted (part of it's children)
		-boolean indicating whether the word exists in the tree or not
		-boolean indicating whether a word with common part exists
	----------------------------------------------------------------------------------------------*/

	char* common(char* key, char* child);
	/*---------------------------------------------------------------------------------------------
		Parameters
			-Takes in 2 char pointer propertly null terminated
		
		Output
			-return the an array of char of the first n charater between the input arrays
	----------------------------------------------------------------------------------------------*/

	int keyLength(char* key);
	/*---------------------------------------------------------------------------------------------
		Parameters
			-Takes in a char pointer properly null terminated

		Output
			-Returns the length of the input array after looping throught it
	---------------------------------------------------------------------------------------------*/

	void printWord(char * word);
	/*---------------------------------------------------------------------------------------------
		Parameters
			-Takes in a char pointer properly null termianted

		Output
			-Print the array of charaters to the console and has no return value
	---------------------------------------------------------------------------------------------*/

	char* different_str(char* str, char* common);
	/*---------------------------------------------------------------------------------------------
		Parameters
			-Takes a prefix char pointer "common"
			-Takes a word char pointer "str"
		
		Output
			-if "common" and "str" match length the function returns NULL
			-otherwise return array of chars of the remaining charaters in "str" after the
			 prefix "common"

		WARNING
			-The function doesn't check if "common" and "str" have common prefix
			-"common" is assumed to be a valid prefix for "str"
	---------------------------------------------------------------------------------------------*/

	void merge(TreeNode * current);
	/*---------------------------------------------------------------------------------------------
		Parameters
			-Takes a TreeNode in the tree

		Output
			-Concatenate the data in the child node into the current node and deletes the child node
			-The function has no return value

		WARNING
			-The function assumes as the current node has one and only child node
	---------------------------------------------------------------------------------------------*/

	void delete_family(TreeNode * current);
	/*---------------------------------------------------------------------------------------------
		Parameters
			-Takes a TreeNode from which we start deleting
		
		Output
			-Deletes all the nodes starting from "current" and returns no value
	---------------------------------------------------------------------------------------------*/

	void copyTree(TreeNode * tree1 , TreeNode *& tree2);
	/*---------------------------------------------------------------------------------------------
		Parameters
			-Tree1: The root of the tree to be copied from
			-Tree2: The root of the tree to be copied into

		Output
			-copies creates a deep copy of "tree1" into "tree2" and returns no value
	---------------------------------------------------------------------------------------------*/
	
	void displayAux(TreeNode * current ,char * prefix , int prefixLength);
	/*---------------------------------------------------------------------------------------------
		Parameters
			-Current: TreeNode from which the function starts traversing
			-Prefix: Should be send an empty array of charaters properly null terminated
			 it's used to concatenate char pointer and send it to children nodes
			-PrefixLength: The length of the sent prefix should be passe zero on first call
		
		Output
			-Traverse all the nodes starting from the given node, printing each one of them
			 to the console in a new line
			-The function doesn't return a value
	---------------------------------------------------------------------------------------------*/
	

public:
	/*****CONSTRUCTORS AND DESTRUCTOR*****/

	Radix();
	Radix(const Radix& otherTree);
	Radix operator=(const Radix& otherTree);
	~Radix();



	/*****MEMBER FUNCTIONS*****/

	bool search(char* key);
	/*---------------------------------------------------------------------------------------------
		Parameters
			-Takes in a word to be looked for in the tree
		
		Output
			-Returns true if the word is found in the tree, false otherswise
	---------------------------------------------------------------------------------------------*/


	void insert(char* key);
	/*---------------------------------------------------------------------------------------------
		Parameters
			-Takes in a word to be inserted in the tree

		Output
			-Inserts the word in the tree if not found and returns true, returns false otherwise
	---------------------------------------------------------------------------------------------*/

	bool erase(char* key);
	/*---------------------------------------------------------------------------------------------
		Parameters
			-Takes in a word to be removed from the tree

		Output
			-Removes the word from the tree and returns true if found, returns false otherwise
	---------------------------------------------------------------------------------------------*/
	
	void display();
	/*---------------------------------------------------------------------------------------------
		Parameters
			-Takes no parameters
		
		Output
			-Prints all the words in the tree to the console each in a new line and doesn't return
			 a value
	---------------------------------------------------------------------------------------------*/

}; /***END OF RADIX TREE DEFINTION***/

#endif 