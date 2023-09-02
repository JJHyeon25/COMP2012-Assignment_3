//this is one of the two files that you need to submit

using namespace std;
//you are NOT allowed to include additional libraries yourself

//you do NOT need to include "tree.h" in this file
//this is NOT a cpp file, but simply a part of "tree.h"
//see the bottom of tree.h for explanation
//just write your tree implementation here right away

template <class T>
Tree<T>::Tree(const Tree& Another){
	if(Another.isEmpty())
		return;

	*this = Another;
}

template <class T>
Tree<T>::~Tree(){
	delete root;
}

template <class T>
const Tree<T>& Tree<T>::operator=(const Tree& Another){
	if (root == Another.root)
		return *this;

	delete root;

	root = new Node<T>(*Another.root);

	return *this;
}

template <class T>
Tree<T>* Tree<T>::find(const T& check_data){
	if (isEmpty())
		return nullptr;

	if (root->data == check_data)
		return this;

	Tree<T>* temp = nullptr;

	for (int i = 0; i < root->childCount; ++i){
		temp = root->children[i].find(check_data);

		if (!temp)
			continue;

		else
			return temp;
	}

	return temp;
}

template <class T>
const Tree<T>* Tree<T>::find(const T& check_data) const{
	if (isEmpty())
		return nullptr;

	if (root->data == check_data)
		return this;

	Tree<T>* temp = nullptr;

	for (int i = 0; i < root->childCount; ++i){
		temp = root->children[i].find(check_data);

		if (!temp)
			continue;

		else
			return temp;
	}

	return temp;
}

template <class T>
int Tree<T>::getDepth(const T& check_data) const{
	if (isEmpty())
		return -1;

	if (!find(check_data))
		return -1;

	if (root->data == check_data)
		return 0;

	for (int i = 0; i < root->childCount; ++i){
		if (!(root->children[i].find(check_data)))
			continue;
		return 1 + root->children[i].getDepth(check_data);
	}

	return 0;
}

template <class T>
int Tree<T>::getDescendantCount(const T& check_data) const{
	const Tree<T>* temp = find(check_data);

	if (isEmpty())
		return -1;

	if (!temp)
		return -1;

	return temp->getDescendantCount();
}

template <class T>
int Tree<T>::getDescendantCount() const{

	int sum{};

	if (isEmpty())
		return -1;
	else
		for (int i = 0; i < root->childCount; ++i){
			if(root->children[i].isEmpty())
				continue;
			sum = sum + 1 + root->children[i].getDescendantCount();
		}

	return sum;
}

template <class T>
bool Tree<T>::addRoot(const T& check_data){
	Tree<T>* temp = find(check_data);

	if (temp != nullptr)
		return false;

	if(root == nullptr){
		root = new Node<T>(check_data);
	}
	else{
		temp = new Tree<T>[1];
		temp->root = new Node<T>(*root);
		root->data = check_data;
		root->childCount = 1;
		delete[] root->children;
		root->children = new Tree<T>[1];
		root->children[0] = *temp;
	}
	temp = nullptr;

	return true;
}

template <class T>
bool Tree<T>::addChild(const T& parentData, const T& childData){
	Tree<T>* temp = find(parentData);

	if (!temp)
		return false;

	if (find(childData))
		return false;

	Tree<T>* temp2 = new Tree<T>[1];
	temp2->root = new Node<T>(childData);

	if (temp->root->childCount == 0){
		++temp->root->childCount;
		temp->root->children = new Tree<T>[1];
		temp->root->children[0] = *temp2;
	}
	else{

		++temp->root->childCount;
		Tree<T>* tempchild = new Tree<T>[temp->root->childCount];

		for (int i = 0; i < temp->root->childCount; ++i){
			if (i == temp->root->childCount - 1){
				tempchild[i] = *temp2;
				break;
			}
			tempchild[i] = temp->root->children[i];
		}

		delete[] temp->root->children;
		temp->root->children = tempchild;

		tempchild = nullptr;
		delete tempchild;
	}
	temp2->root = nullptr;
	delete temp2->root;
	temp2 = nullptr;
	delete temp2;

	return true;
}
