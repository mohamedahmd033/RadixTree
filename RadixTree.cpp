#include "RadixTree.hpp"

/*****HELPER FUNCTIONS*****/

std::tuple<TreeNode*, bool,bool> Radix::find(TreeNode*& parent, TreeNode* curr, char*& key, int& startingIndex) {
	//temporary pointer to loop through the node children
	TreeNode* currChild = curr->children;

	//Looping through the Children and comparing the prefix in each
	while (currChild != nullptr) {
		//if a child matches the prefix 
		//we traverse down the children ("children" pointer)
		//otherwise we swipe through the current list of children ("next" pointer)
		if (memcmp(key + startingIndex, currChild->prefix, currChild->length) == 0) {
			//increment the startingIndex for comparing
			startingIndex += currChild->length;

			//checking if we reached the end of a given key
			if (key[startingIndex] == '\0') {
				parent = curr;
				return { currChild , currChild->isWord, true };

			}
			//else keeps traversing
			else {
				parent = curr;
				return find(parent, currChild, key, startingIndex);
			}
		}

	else
		{
			if ('\0'!= *common(key+ startingIndex,currChild->prefix)) //is there's common characters between the key and currChild?
			{
				return { currChild , false,true }; 
				/*
					return the Node , the key have existed,
					there's a common characters between key and currChild prefix 
				*/
			}
			else //no common characters
			{
				currChild = currChild->next; //continue traversing
			}
		}
		
	}
	//word wasn't found
	//returning the node it should be inserted at
	return { curr , false,false };
}


char* Radix::common(char* key, char* child) {
	char* commonStr = new char[keyLength(key) + 1];

	int i = 0;
	for (; key[i] != '\0' && child[i] != '\0'; i++) {
		if (key[i] == child[i])
			commonStr[i] = key[i];
		else
			break;
	}
	commonStr[i] = '\0';

	return commonStr;

}


int Radix::keyLength(char* key) {
	int i = 0;
	while (key[i] != '\0')
	{
		i++;
	}
	return i;
}

void Radix::printWord(char * word){
	for(int i=0 ; word[i] != '\0' ; i++)
		std::cout << word[i];
}

char* Radix:: different_str(char* str, char* common)
{

	int str_size = keyLength(str); //number of elements in str
	int common_size = keyLength(common); //number of common elements

	if (str_size == common_size) //there's no different string
		return NULL;

	else //there's different string
		return str + common_size;//return the rest of str after common part
}

void Radix::merge(TreeNode * current){
	//creating new prefix and copying the current and child prefixes into it
	char * newPrefix = new char[current->length + current->children->length + 1];
	memcpy(newPrefix, current->prefix , current->length);
	memcpy(newPrefix + current->length , current->children->prefix , current->children->length);
	newPrefix[current->length + current->children->length] = '\0';

	//fixing the current node prefix 
	current->length = current->length + current->children->length; 
	current->prefix = newPrefix;
	current->freq = current->children->freq;
	current->isWord = true;

	//linking the children of the single child to the current node
	TreeNode * temp = current->children;
	current->children = current->children->children;
	delete temp;
}

void Radix::delete_family(TreeNode * current){

	if(current != nullptr){
		delete_family(current->children);
		delete_family(current->next);
		delete current;
	}
}

void Radix::copyTree(TreeNode * orginalTree , TreeNode *& copiedTree){
	if(orginalTree != nullptr){
		char * newPrefix = new char[orginalTree->length + 1];
		memcpy(newPrefix, orginalTree->prefix , orginalTree->length);
		newPrefix[orginalTree->length] = '\0';

		copiedTree = new TreeNode(newPrefix , orginalTree->length , orginalTree->freq,orginalTree->isWord);

		copyTree(orginalTree->children , copiedTree->children);
		copyTree(orginalTree->next , copiedTree->next);
	}
}

void Radix::displayAux(TreeNode * current  , char * parentPrefix , int prefixLength){
	if(current == nullptr)
		return;
	
	//print current node 
	if(current->isWord){
		printWord(parentPrefix);
		printWord(current->prefix);
		std::cout <<"\tfrequancy: " << current->freq << std::endl;
	}

	//traversing children
	if(current->children){
		//creating new prefix for children
		char * newPrefix = new char[current->length + prefixLength + 1];
		memcpy(newPrefix , parentPrefix , prefixLength );
		memcpy(newPrefix + prefixLength , current->prefix , current->length);
		newPrefix[current->length + prefixLength] = '\0';

		displayAux(current->children , newPrefix , current->length + prefixLength);
		delete[] newPrefix;
	}
	//traversing next nodes
	displayAux(current->next, parentPrefix , prefixLength);
}

/*****MEMBER FUNCTIONS*****/

bool Radix::search(char* key) {
	int index = 0;
	TreeNode* parent = nullptr;
	return std::get<1>(find(parent, root, key, index));
} 



void Radix::insert(char* key)
{
	TreeNode* Parent = NULL; //for find function arguments
	int startingIndex = 0; //for find function arguments

	TreeNode* current;//"current" points to the most similar node to the key
	bool word;//word shows if the key is existed or not: true for existence flase for not
	bool most_similar_node;
	/*most_similar_node shows if the type of node returned form "find" function 
	is most similar node "true" or it will be a parent to insert key as its child*/
	
	std::tie(current, word, most_similar_node) = find(Parent, root, key, startingIndex); //finding the nearest similar node to the key


	if (word == true) //the key is already inside the tree
	{
		current->freq++;
		return; //the function haven't inserted the key, it is already existed
	}
	else //the key is't existed in the tree
	{
		//exact node exists in the tree but not inserted
		if(key[startingIndex] == '\0'){
			current->isWord = true;
			current->freq++;
			return;
		}

		else if (!most_similar_node) // is the node is to be a parent to insert the key as its child?
		{
			TreeNode* temp = current->children; //saving the children pointer in the current node
			current->children = new TreeNode(key + startingIndex, keyLength(key + startingIndex),1 , 1);
			/*the current node child is the remaining 
			of the inserted key, +1 is for the NULL character*/
			(current->children)->next = temp; //the next ptr of the new node of key points to the rest of the parent children 
		}
		else //the current node is partially similar to the key 
		{
			/*they are similar but not typical*/
			char* similar_str = common(key+ startingIndex, current->prefix);//get the common string between the key and the prefix of the node
			char* node_different_str = different_str(current->prefix, similar_str);
			char* keys_different_str = different_str(key+ startingIndex, similar_str);


			current->prefix = similar_str; //the current node is now hold the common string between the prefix and the key
			current->length = keyLength(current->prefix);// setting the new length of the current node
			TreeNode* temp = current->children ;// temp save the link to the current node children

			
			if (NULL != keys_different_str) //there is different characters between the key and the common string
			{
				current->children = new TreeNode(keys_different_str, keyLength(keys_different_str), 1,1);
					/*the node has new child for the uncommon part of key,
					+1 of the keyLength is for the NULL character*/

				(current->children)->next= new TreeNode(node_different_str, keyLength(node_different_str),current->freq,1);
				/*the next node to the new node that contains the uncommon part of key is the uncommon part of prefix ,
				+1 of the keyLength is for the NULL character*/

				((current->children)->next)->children = temp;
				current->isWord = false;
				current->freq = 0;
				/*the children of the main current node is now the children of the new node that contains
				the uncommon characters of the current node prefix*/

				return ;

			}
			else //there is no different characters between the key and the common string
			{
				current->children = new TreeNode(node_different_str, keyLength(node_different_str),current->freq, 1);
				/*the new node contains the uncommon characters of the prefix,
				+1 of the keyLength is for the NULL character*/

				((current->children))->children = temp;
				/*the children of the main current node is now the children of the new node that contains
				the uncommon characters of the current node prefix*/

				current->isWord = true;
				current->freq = 1;
				return;
			}

		}
	}
	return;
}



bool Radix::erase(char * key){
	int startingIndex = 0 ;
	TreeNode * parent = nullptr;

	TreeNode * current;
	bool isWord;
	bool smiliarWord;
	//searching for the node to be deleted
	std::tie(current , isWord , smiliarWord) = find(parent , root , key , startingIndex);

	//word doesn't exist in the tree
	if(isWord == false)
		return false;
	
	//the node has no children
	if(current->children == nullptr){
		/*
			finding the previous node in the linked list of children
			deleting the current node from the list
		*/
		TreeNode * prev = nullptr;
		TreeNode * currChild = parent->children;

		//finding the previous node in the linked list
		while(currChild != nullptr){
			if(currChild == current)
				break;
			else{
				prev = currChild;
				currChild = currChild->next;
			}
		}
		//deleting the node from the linked list
		if(prev != nullptr){
			prev->next = prev->next->next;
		}
		else 	
			parent->children = current->next;

		delete currChild;
		currChild = nullptr;

		//if now the parent has only one child left and the parent is not a word
		//we merge them
		if(parent->children->next == nullptr && parent->isWord == false)
			merge(parent);
	}

	//node has 1 child
	else if(current->children->next == nullptr){
		/*
			merging the single child with current node
			deleting the child node
		*/

		merge(current);
	}

	//node has multiple children
	else{
		current->isWord = false;
	}
	
	return true;
}

void Radix::display(){
	displayAux(root->children , (char*)"" , 0);
}



/*****CONSTRUCTORS AND DESTRUCTORS*****/

Radix::Radix() {
	root = new TreeNode((char*)"\0", 0 ,0, 0);
}

Radix::Radix(const Radix& otherTree):Radix() {
	copyTree(otherTree.root->children ,root->children);
}

Radix Radix::operator=(const Radix& otherTree){
	delete_family(root->children);
	copyTree(otherTree.root->children , root->children);
	return *this;
}

Radix::~Radix()
{
	delete_family(root);
}