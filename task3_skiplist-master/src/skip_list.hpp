////////////////////////////////////////////////////////////////////////////////
// Module Name:  skip_list.h/hpp
// Authors:      Leonid Dworzanski, Sergey Shershakov
// Version:      2.0.0
// Date:         28.10.2018
//
// This is a part of the course "Algorithms and Data Structures"
// provided by  the School of Software Engineering of the Faculty
// of Computer Science at the Higher School of Economics.
////////////////////////////////////////////////////////////////////////////////

// !!! DO NOT include skip_list.h here, 'cause it leads to circular refs. !!!

#include <cstdlib>

//==============================================================================
// class NodeSkipList
//==============================================================================

template <class Value, class Key, int numLevels>
void NodeSkipList<Value, Key, numLevels>::clear(void)
{
	for (int i = 0; i < numLevels; ++i)
		Base::nextJump[i] = 0;

	Base::levelHighest = -1;
}

//------------------------------------------------------------------------------

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(void)
{
	clear();
}

//------------------------------------------------------------------------------

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(const Key& tkey)
{
	clear();

	Base::Base::key = tkey;
}

//------------------------------------------------------------------------------

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(const Key& tkey, const Value& val)
{
	clear();

	Base::Base::key = tkey;
	Base::Base::value = val;
}


//==============================================================================
// class SkipList
//==============================================================================

template <class Value, class Key, int numLevels>
SkipList<Value, Key, numLevels>::SkipList(double probability)
{
	_probability = probability;

	// Lets use _preHead as a final sentinel element
	for (int i = 0; i < numLevels; ++i)
		Base::_preHead->nextJump[i] = Base::_preHead;

	Base::_preHead->levelHighest = numLevels - 1;
}


// TODO: !!! One need to implement all declared methods !!!


/// \brief Insert a new element into your list.
///
/// Think very hard how will you guarantee thatnew element will be
/// included in sparse levels correctly!
///
/// Make different test. Check border cases - empty list, end of the list,
/// et cetera ...
template <class Value, class Key, int numLevels>
void SkipList<Value, Key, numLevels>::insert(const Value& val, const Key& key)
{
	Node* curr = Base::_preHead;
	Node* ins = new Node(key, val);

	//Search for position for "ins" using upper levels
	//and save pointers to previous element for "ins" on each level
	//in "ins->nextJump" in order to access it later
	for (int i = numLevels - 1; i >= 0; --i)
	{
		while (curr->nextJump[i] != Base::_preHead && curr->nextJump[i]->key <= key)
			curr = curr->nextJump[i];

		ins->nextJump[i] = curr;  //store previous elements
	}

	//Search for position for "ins" on lowest level
	while (curr->next != _preHead && curr->next->key <= key)
		curr = curr->next;

	//Insert "ins" to lowest level
	ins->next = curr->next;
	curr->next = ins;

	//Insert "ins" on upper levels depending on probability
	for (int i = 0; i < numLevels; ++i)
	{
		double probability = (double)rand() / RAND_MAX; //in [0;1]

		//If probability is greater than probability of insert
		//save the index of the most upper level where 
		//"ins" occurs and finish insert on upper levels
		if (probability > _probability)
		{
			ins->levelHighest = i - 1;
			break;
		}

		//Insert "ins" on upper level with index "i"
		Node* temp = ins->nextJump[i];
		ins->nextJump[i] = temp->nextJump[i];
		temp->nextJump[i] = ins;
	}

	//Fill the rest pointers to next elements with "_preHead"
	for (int i = ins->levelHighest + 1; i < numLevels; ++i)
		ins->nextJump[i] = Base::_preHead;
}


/// \brief Remove the nodeBefore from the list and delete it from the memory.
///
/// Check if an idiot called your function
/// (i.e. don't delete pprehead occasionaly, and other cases)
/// (Throw std::invalid_argument in such case.)
///
/// Continue to think hard about sparse levels.
/// Check different cases.
template <class Value, class Key, int numLevels>
void SkipList<Value, Key, numLevels>::removeNext(Node* nodeBefore)
{
	if (nodeBefore == _preHead)
		return;

	Node* curr = _preHead;
	Node* prevNodes[numLevels]; //stores nodes previous to "nodeBefore" on each level (excluding lowest)

	//Search for position of "nodeBefore" using upper levels
	//and save nodes previous to "nodeBefore"
	for (int i = numLevels - 1; i >= 0; --i)
	{
		while (curr->nextJump[i] != _preHead && curr->nextJump[i]->key < nodeBefore->key)
			curr = curr->nextJump[i];

		prevNodes[i] = curr;  //store previous nodes
	}

	//Search for "nodeBefore" on lowest level
	while (curr->next != _preHead && curr->next != nodeBefore)
		curr = curr->next;

	//If nodeBefore wasn't found report about it
	if (curr->next != nodeBefore)
		throw std::invalid_argument("Wrong pointer to nodeBefore for delete");

	//Connect previous nodeBefore for "nodeBefore" on each level
	//to next nodeBefore for "nodeBefore" on corresponding level 
	for (int i = 0; i <= nodeBefore->levelHighest; ++i)
		prevNodes[i]->nextJump[i] = nodeBefore->nextJump[i];

	//Connect previous and next nodes for "nodeBefore" on lowest level
	curr->next = nodeBefore->next;

	delete nodeBefore;
}

template<class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>* SkipList<Value, Key, numLevels>::findLastLessThan(const Key& key) const
{
	Node* curr = _preHead;

	//Search for position of last nodeBefore with key less than given key
	for (int i = numLevels - 1; i >= 0; --i)
	{
		while (curr->nextJump[i] != _preHead && curr->nextJump[i]->key < key)
			curr = curr->nextJump[i];
	}

	//Search for last nodeBefore which key is less than given key
	while (curr->next != _preHead && curr->next->key < key)
		curr = curr->next;

	return curr;
}

template<class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>* SkipList<Value, Key, numLevels>::findFirst(const Key& key) const
{
	//Find last nodeBefore which key is less than given key and get next nodeBefore
	Node* result = findLastLessThan(key)->next;

	//Check if found nodeBefore key equals to given key (it could be greater)
	if (result->key == key)
		return result;

	return nullptr;
}