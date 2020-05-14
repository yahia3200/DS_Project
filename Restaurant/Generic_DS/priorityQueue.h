#include"TNode.h"
#include "..\Rest\Order.h"
#include "..\Rest\Cook.h"
#include<iostream> 
using namespace std;
#ifndef _PRIORITY_QUEUE_H
#define _PRIORITY_QUEUE_H
template<typename T>
class PriorityQueue
{
	TNode<T>* Root;
	int count = 0; //number of nodes in the tree
	T DeleteNode(const T&Value) //Function to delete node which its priority corresponds to "value"
	{
		TNode<T>* del;
		TNode<T>* delLeft;
		T delItem;
		TNode<T>* temp;

		if (Root && Root->getitem() == Value)
		{
			del = Root;
			if (!del->getleft() && !del->getright())
			{
				Root = NULL;
			}
			else if (!del->getleft())
			{
				Root = del->getright();
			}
			else if (!del->getright())
			{
				Root = del->getleft();
			}
			else
			{
				delLeft = del->getleft();
				Root = del->getright();
				temp = Root;
				while (temp->getleft())
				{
					temp = temp->getleft();
				}
				temp->setleft(delLeft);
			}
			delItem = del->getitem();
			delete del;
			return delItem;
		}

		temp = Root;

		while (temp && (temp->getleft() || temp->getright()))
		{
			if (temp->getleft() && (temp->getleft())->getitem() == Value)
			{
				del = temp->getleft();

				if (!del->getleft() && !del->getright())
				{
					temp->setleft(NULL);
				}
				else if (!del->getleft())
				{
					temp->setleft(del->getright());
				}
				else if (!del->getright())
				{
					temp->setleft(del->getleft());
				}
				else
				{
					delLeft = del->getleft();
					temp->setleft(del->getright());
					while (temp->getleft())
					{
						temp = temp->getleft();
					}
					temp->setleft(delLeft);
				}
				delItem = del->getitem();
				delete del;
				return delItem;
			}
			else if (temp->getright() && (temp->getright())->getitem() == Value)
			{
				del = temp->getright();

				if (!del->getleft() && !del->getright())
				{
					temp->setright(NULL);
				}
				else if (!del->getleft())
				{
					temp->setright(del->getright());
				}
				else if (!del->getright())
				{
					temp->setright(del->getleft());
				}
				else
				{
					delLeft = del->getleft();
					temp->setright(del->getright());
					temp = del->getright();
					while (temp->getleft())
					{
						temp = temp->getleft();
					}
					temp->setleft(delLeft);
				}
				delItem = del->getitem();
				delete del;
				return delItem;
			}
			else if (*(Value) > *(temp->getitem()))
			{
				temp = temp->getright();
			}
			else
			{
				temp = temp->getleft();
			}
		}
		//if it wasnot found
		return NULL;
	}
	void insert(TNode<T>*& subroot, T item) // insert new node based on it's priority
	{
		if (subroot == NULL)
			subroot = new TNode<T>(item);
		else if (*(subroot->getitem()) > *item)
		{
			insert(subroot->getleft(), item);

		}
		else
		{
			insert(subroot->getright(), item);
		}
	}

	
	void preorder(TNode<T>* subroot) // in sequence (root -> left -> right)
	{
		if (Root == nullptr) { cout << "NULL"; return; }
		if (subroot == nullptr) { return; }
		cout << subroot->getitem()->GetID() << "  ";
		preorder(subroot->getleft());
		preorder(subroot->getright());
	}

	void dtree(TNode<T>*& subroot) //Destroy Tree (Deletes all the nodes in the tree)
	{
		if (subroot != nullptr)
		{
			dtree(subroot->getleft());

			dtree(subroot->getright());

			delete subroot;
			subroot = nullptr;
		}
	}
	void rec_to_array(TNode<T>* subroot, T*& Array, int & i)
	{
		if (!subroot) { return; }
		else 
		{
			Array[i] = subroot->getitem();    i++;
		}
		rec_to_array(subroot->getleft(), Array, i);
		rec_to_array(subroot->getright(), Array, i);
	}

public:
	PriorityQueue();
	TNode<T>* getroot()const;
	TNode<T>* find_max();
	TNode<T>* find_min();
	bool isEmpty()const;
	void enqueue(T item);
	T dequeue();//menna//modified the return type to be T*
	T Peek(); //menna//modified the return type to be T*
	void Print_preorder();
	T* toArray();
	int getCount() const { return count; }
	~PriorityQueue();
};
template<typename T>
PriorityQueue<T>::PriorityQueue()
{
	Root = nullptr;
}
template <typename T>
bool PriorityQueue<T>::isEmpty() const
{
	if (Root == nullptr)
		return true;
	else
		return false;
}
template<typename T>
TNode<T>* PriorityQueue<T>::getroot()const
{
	return Root;
}
template<typename T>
TNode<T>* PriorityQueue<T>::find_max()
{
	TNode<T>* subroot = Root;
	if (!subroot) return nullptr;
	while (subroot->getright()) {
		subroot = subroot->getright();
	}
	return subroot;
}
template<typename T>
TNode<T>* PriorityQueue<T>::find_min()
{
	return rec_min(this->Root);
}
template<typename T>
void PriorityQueue<T>::enqueue(T item)
{
	count++;
	insert(Root, item);
}
template<typename T>
T PriorityQueue<T>::dequeue()
{
	TNode<T>* max = find_max();
	if (!max)
	{
		return NULL;
	}
	count--;
	return DeleteNode( max->getitem());
}
template<typename T>
T PriorityQueue<T>::Peek()
{
	if (isEmpty())return NULL;
	TNode<T>* max = find_max();
	T max_item = max->getitem();
	return max_item;
}
template<typename T>
T* PriorityQueue<T>::toArray()
{
	if (!Root){ return NULL; }
	int i = 0;
	T* Array = new T[count];
	rec_to_array(Root, Array, i);
	return Array;
}

template<typename T>
void PriorityQueue<T>::Print_preorder() // just for testing the implemention
{
	preorder(Root);
}

template<typename T>
PriorityQueue<T>::~PriorityQueue()
{

	dtree(Root);
}
#endif
