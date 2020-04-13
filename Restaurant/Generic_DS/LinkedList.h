#ifndef __LINKEDLIST_H_
#define __LINKEDLIST_H_

#include <iostream>
using namespace std;

#include"Node.h"

template < typename T>
class LinkedList
{
private:
	int count;     // number of nodes in a list
	Node<T>* Head; // Pointer to Head node
	Node<T>* Tail; // Pointer to Tail node
public:
	LinkedList();
	void insertEnd(const T& newEntry);          //Inserts a new node at the end of the list
	bool isEmpty();
	bool deleteNode(const T& theEntry);			//Specifically for deleting a normal order that has been cancelled or promoted
	Node<T>* getPrevPointer(const T& theEntry); //returning the node before the required node
												//i.e the node whose next is the required node
	void clear();                               //delete all nodes in the list

	void printList();     //To test the linkedlist implementation
						  //Should be DELETED before delivering phase1
	//hala //i need getter for head
	Node<T>*GetHead();


}; // end Node


template < typename T>
LinkedList<T>::LinkedList()
{
	Head = nullptr;
	Tail = nullptr;
	count = 0;
}

template<typename T>
Node<T>* LinkedList<T>::GetHead() {
	return Head;
}

template < typename T>
Node<T>* LinkedList<T>::getPrevPointer(const T& theEntry)
{
	if (!Head || (Head->getItem() == theEntry))
	{
		return nullptr;
	}

	Node<T>* temp = Head;
	Node<T>* nextTemp = temp->getNext();
	while (nextTemp)
	{
		if (nextTemp->getItem() == theEntry)
		{
			return temp;
		}
		temp = nextTemp;
		nextTemp = nextTemp->getNext();
	}
	return nullptr;
}

template < typename T>
void LinkedList<T>::insertEnd(const T& newEntry)
{
	if (isEmpty())
	{
		Head = Tail = new Node<T>(newEntry);
		count++;
		return;
	}

	Node<T>* addedNode = new Node<T>(newEntry);
	Tail->setNext(addedNode);
	Tail = addedNode;
	count++;
}

template < typename T>
bool LinkedList<T>::isEmpty()
{
	return (Head == nullptr);
}

template < typename T>
bool LinkedList<T>::deleteNode(const T& requiredEntry)
{
	if (isEmpty())
	{
		return false;
	}
	
	Node<T>* temp;

	//Incase the requiredEntery is found in the first node
	if (Head->getItem() == requiredEntry)
	{
		temp = Head;
		Head = Head->getNext();
		delete temp;
		count--;

		//If unfortunately the requiredNode to be deleted is the only node in the list
		if (!Head)
		{
			Tail = nullptr;
		}
		
		return true;
	}

	Node<T>* prevNode = getPrevPointer(requiredEntry);

	//if the requiredEntery is not found;
	//ofcourse there isn't a prevpointer to a not-found Entry
	if (!prevNode)
	{
		return false;
	}

	Node<T>* requiredNode = prevNode->getNext();
	
	//If the requiredNode is the last Node in the list
	//Handling the Tail
	if (!(requiredNode->getNext()))
	{
		Tail = prevNode;
	}

	prevNode->setNext(requiredNode->getNext());
	delete requiredNode;
	count--;
	return true;
}

template < typename T>
void LinkedList<T>::clear()
{
	if (!isEmpty())
	{
		Node<T>* temp;

		do
		{
			temp = Head;
			Head = Head->getNext();
			delete temp;
		} 
		while (Head);

		Tail = nullptr;
		count = 0;
	}
}

template < typename T>
void LinkedList<T>::printList()
{
	Node<T>* temp = Head;
	while (temp)
	{
		cout << temp->getItem() << endl;
		temp = temp->getNext();
	}
	cout << endl;
}


#endif