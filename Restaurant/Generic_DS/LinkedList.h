#ifndef __LINKEDLIST_H_
#define __LINKEDLIST_H_

#include <iostream>
using namespace std;

#include"Node.h"
#include "../Rest/Order.h"
#include "../Rest/Cook.h"

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
	void insertSorted(const T& item);          //Inserts a new node in its correct position

	
	bool isEmpty();
	T deleteNode(const T& theEntry);			//Specifically for deleting a normal order that has been cancelled or promoted
	Node<T>* getPrevPointer(const T& theEntry); //returning the node before the required node
												//i.e the node whose next is the required node
	void clear();                               //delete all nodes in the list

	void printList();     //To test the linkedlist implementation
						  //Should be DELETED before delivering phase1
	T peekHead();

	T* toArray();
	T Remove_Head();
	int getCount();

}; // end Node

template<class T>
void LinkedList<T>::insertSorted(const T& item)
{
	//If the ll is empty
	if (!Head){ Tail = Head = new Node<T>(item); return; }

	Node<T>* temp = Head;

	//If the node to be inserted, gretter than the Head 
	//(i.e must be inserted before the head)
	//(i.e the node to be inserted is the new Head for the ll)

	if (item > temp->getItem())
	{
		Head = new Node<T>(item);
		Head->setNext(temp);
		return;
	}

	//loop till you reach the node that should be after the inserted node.
	while (temp->getNext() && item < (temp->getNext())->getItem())
	{
		temp = temp->getNext();
	}
	//next is the node that should be after the inserted one.
	Node<T>* next = temp->getNext();
	Node<T>* newPtr = new Node<T>(item);
	//temp is the node that should be before the inserted one.
	temp->setNext(newPtr);
	newPtr->setNext(next);

	//Handiling the Tail
	//If the node to be inserted was the last Node in the ll
	if (!next)
	{
		Tail = newPtr;
	}
}


template < typename T>
int LinkedList<T>::getCount()
{
	return count;
}

template < typename T>
T LinkedList<T>::peekHead()
{
	return Head->getItem();
}

template < typename T>
T* LinkedList<T>::toArray()
{
	if (!Head){ return NULL; }

	T* Array = new T[count];
	Node<T>* temp = Head;
	for (int i = 0; i < count;i++)
	{
		Array[i] = temp->getItem();
		temp = temp->getNext();
	}
	return Array;
}

template < typename T>
LinkedList<T>::LinkedList()
{
	Head = nullptr;
	Tail = nullptr;
	count = 0;
}

template<typename T>
T LinkedList<T>::Remove_Head() 
{
	if (!Head)
		return nullptr;

	T item = Head->getItem();

	Node<T>* temp = Head;
	Head = Head->getNext();

	if (!Head){ Tail = nullptr; }

	delete temp;
	count--;
	return item;
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
T LinkedList<T>::deleteNode(const T& requiredEntry)
{
	if (isEmpty())
	{
		return nullptr;
	}
	
	Node<T>* temp;
	T item;

	//Incase the requiredEntery is found in the first node
	if ( Head->getItem() == requiredEntry)
	{
		item = Head->getItem();

		temp = Head;
		Head = Head->getNext();
		delete temp;
		count--;

		//If unfortunately the requiredNode to be deleted is the only node in the list
		if (!Head)
		{
			Tail = nullptr;
		}
		
		return item;
	}

	Node<T>* prevNode = getPrevPointer(requiredEntry);

	//if the requiredEntery is not found;
	//ofcourse there isn't a prevpointer to a not-found Entry
	if (!prevNode)
	{
		return nullptr;
	}

	Node<T>* requiredNode = prevNode->getNext();
	
	//If the requiredNode is the last Node in the list
	//Handling the Tail
	if (!(requiredNode->getNext()))
	{
		Tail = prevNode;
	}

	item = requiredNode->getItem();

	prevNode->setNext(requiredNode->getNext());
	delete requiredNode;
	count--;
	return item;
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


////////////////////////////////////////////////////////////
//////// Full Template Specialization For Orders////////////
////////////////////////////////////////////////////////////

template <>
class LinkedList<Order*>
{
private:
	int count;     
	Node<Order*>* Head; 
	Node<Order*>* Tail; 
public:
	LinkedList()
	{
		Head = nullptr;
		Tail = nullptr;
		count = 0;
	}
	void insertEnd(Order*& newEntry)
	{
		if (isEmpty())
		{
			Head = Tail = new Node<Order*>(newEntry);
			count++;
			return;
		}

		Node<Order*>* addedNode = new Node<Order*>(newEntry);
		Tail->setNext(addedNode);
		Tail = addedNode;
		count++;
	}

	void insertSorted(Order*& item)
	{
		//If the ll is empty
		if (!Head)
		{ 
			Tail = Head = new Node<Order*>(item); count++; return; 
		}

		Node<Order*>* temp = Head;

		//If the node to be inserted, gretter than the Head 
		//(i.e must be inserted before the head)
		//(i.e the node to be inserted is the new Head for the ll)

		if (*item > *(temp->getItem()))
		{
			Head = new Node<Order*>(item);
			Head->setNext(temp);
			count++;
			return;
		}

		//loop till you reach the node that should be after the inserted node.
		while (temp->getNext() && *((temp->getNext())->getItem()) > *item)
		{
			temp = temp->getNext();
		}

		//next is the node that should be after the inserted one.
		Node<Order*>* next = temp->getNext();
		Node<Order*>* newPtr = new Node<Order*>(item);

		//temp is the node that should be before the inserted one.
		temp->setNext(newPtr);
		newPtr->setNext(next);
		count++;

		//Handiling the Tail
		//If the node to be inserted was the last Node in the ll
		if (!next)
		{
			Tail = newPtr;
		}
	}


	bool isEmpty()
	{
		return (Head == nullptr);
	}

	Order* deleteNode(Order*& requiredEntry)
	{
		if (isEmpty())
		{
			return nullptr;
		}

		Node<Order*>* temp;
		Order* item;

		//Incase the requiredEntery is found in the first node
		if (Head->getItem() == requiredEntry)
		{
			item = Head->getItem();

			temp = Head;
			Head = Head->getNext();
			delete temp;
			count--;

			//If unfortunately the requiredNode to be deleted is the only node in the list
			if (!Head)
			{
				Tail = nullptr;
			}

			return item;
		}

		Node<Order*>* prevNode = getPrevPointer(requiredEntry);

		//if the requiredEntery is not found;
		//ofcourse there isn't a prevpointer to a not-found Entry
		if (!prevNode)
		{
			return nullptr;
		}

		Node<Order*>* requiredNode = prevNode->getNext();

		//If the requiredNode is the last Node in the list
		//Handling the Tail
		if (!(requiredNode->getNext()))
		{
			Tail = prevNode;
		}

		item = requiredNode->getItem();

		prevNode->setNext(requiredNode->getNext());
		delete requiredNode;
		count--;
		return item;
	}
	
	Node<Order*>* getPrevPointer(Order*& theEntry)
	{
		if (!Head || (Head->getItem() == theEntry))
		{
			return nullptr;
		}

		Node<Order*>* temp = Head;
		Node<Order*>* nextTemp = temp->getNext();
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
												
	void clear()
	{
		if (!isEmpty())
		{
			Node<Order*>* temp;

			do
			{
				temp = Head;
				Head = Head->getNext();
				delete temp;
			} while (Head);

			Tail = nullptr;
			count = 0;
		}
	}

	Order* peekHead()
	{
		return Head->getItem();
	}

	Order** toArray()
	{
		if (!Head) { return NULL; }

		Order** Array = new Order * [count];
		Node<Order*>* temp = Head;
		for (int i = 0; i < count; i++)
		{
			Array[i] = temp->getItem();
			temp = temp->getNext();
		}
		return Array;
	}
	Order* Remove_Head()
	{
		if (!Head)
			return nullptr;

		Order* item = Head->getItem();

		Node<Order*>* temp = Head;
		Head = Head->getNext();

		if (!Head) { Tail = nullptr; }

		delete temp;
		count--;
		return item;
	}
	int getCount()
	{
		return count;
	}
	Order* get_by_index(int  index)
	{
		Node<Order*>* ptr = Head;

		while (ptr)
		{
			if (*(ptr->getItem()) == index)
				return ptr->getItem();

			ptr = ptr->getNext();
		}

		return nullptr;
	}
};


#endif