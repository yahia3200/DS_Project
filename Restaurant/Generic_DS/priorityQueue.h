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
	TNode<T>* DeleteNode(TNode<T>* root, TNode<T>*value, TNode<T>*& remove) //Function to delete node which its priority corresponds to "value"
	{ 
		T origin_Root_dta=Root->getitem();
		T root_item = root->getitem();
		T value_item = value->getitem();

		if (root == NULL) return root;
		if (*origin_Root_dta == *value_item) //in case the node to be deleted is the root 
		{
			remove = Root;
			if (!Root->getleft()) Root = Root->getright();
			else if (!Root->getright()) Root = Root->getleft();
			else if (!Root->getright() && !Root->getleft()) // the tree has only one node (root)
			{
				Root = NULL;
			}
			return Root;
		}
		
		if (*root_item > *value_item)
			root->setleft(DeleteNode(root->getleft(), value, remove));
		else if (*root_item > *value_item)
			root->setright(DeleteNode(root->getright(), value, remove));

		else // if value is same as root's priority, then This is the node to be deleted 
		{

			if (root->getright() == NULL)
			{
				TNode<T>* target = new TNode<T>(root);
				remove = target;
				TNode<T>* temp = root;
				root = root->getleft();
				delete temp;
			}
			else if (root->getleft() == NULL)
			{
				TNode<T>* target = new TNode<T>(root);
				remove = target;
				TNode<T>* temp = root;
				root = root->getright();
				delete temp;
			}
			else if (root->getright() == NULL && root->getleft() == NULL)
			{
				TNode<T>* target = new TNode<T>(root);
				remove = target;
				delete root;
				root = NULL;
			}
			else
			{
				TNode<T>* ptr = rec_max(root->getright());
				root->setitem(ptr->getitem());
				root->setright(DeleteNode(root->getright(), ptr, remove));
			}
		}

		return root;
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

	TNode<T>* rec_max(TNode<T>* subroot) // recursive function to find max priority of the nodes
	{
		if (!subroot) return nullptr;
		TNode<T>* max = subroot;
		T Lmax_dta, max_dta, Rmax_dta;
		if(max)  max_dta = max->getitem();
		TNode<T>* Lmax = rec_max(subroot->getleft());
		TNode<T>* Rmax = rec_max(subroot->getright());
		if (Lmax)  Lmax_dta = Lmax->getitem();
		if (Rmax)  Rmax_dta = Rmax->getitem();
		if (Lmax && *Lmax_dta > *max_dta)  max = Lmax;
		if (Rmax && *Rmax_dta > *max_dta)  max = Rmax;
		//cout << max << endl;
		return max;
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
	return rec_max(this->Root);
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
	TNode<T>* deleted = nullptr;
	T deleted_item;
	if (!max) 
	{
		
		return NULL;
	}
	DeleteNode(Root, max, deleted);
	count--;
	deleted_item = deleted->getitem();
	return deleted_item;
}
template<typename T>
T PriorityQueue<T>::Peek()
{
	TNode<T>* max = find_max();
	T max_item = max->getitem();
	return max_item;
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
				