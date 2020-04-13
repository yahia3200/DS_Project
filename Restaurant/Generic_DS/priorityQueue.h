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
	TNode<T>* DeleteNode(TNode<T>* root, Node<T>*value, TNode<T>*& remove) //Function to delete node which its priority corresponds to "value"
	{
		TNode<T>* Rptr = Root;
		if (root == NULL) return root;
		if (Root == value) //in case the node to be deleted is the root 
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
		if (root > value)
			root->setleft(DeleteNode(root->getleft(), value, remove));
		else if (value > root)
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
		else if (subroot->getitem() < item)
		{
			insert(subroot->getright(),item);
		}
		else
		{
			insert(subroot->getleft(), item);
		}
	}

	TNode<T>* rec_max(TNode<T>* subroot) // recursive function to find max priority of the nodes
	{
		if (!subroot) return nullptr;
		TNode<T>* max = subroot;
		TNode<T>* Lmax = rec_max(subroot->getleft());
		TNode<T>* Rmax = rec_max(subroot->getright());
		if (Lmax && Lmax > max)  max = Lmax;
		if (Rmax && Rmax > max)  max = Rmax;
		//cout << max << endl;
		return max;
	}
	void preorder(TNode<T>* subroot) // in sequence (root -> left -> right)
	{
		if (Root == nullptr) { cout << "NULL"; return; }
		if (subroot == nullptr) { return; }
		cout << subroot->getitem() << "  ";
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
	//bool peekFront()const;
	void enqueue(T item);
	T* toArray(int size);
	TNode<T>* dequeue();
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
TNode<T>* PriorityQueue<T>::dequeue()
{
	TNode<T>* max = find_max();

	TNode<T>* deleted = nullptr;
	if (!max) return deleted;
	DeleteNode(Root, max, deleted);
	count--;
	return deleted;
}

template<typename T>
void PriorityQueue<T>::Print_preorder() // just for testing the implemention
{
	preorder(Root);
}

template<typename T>
T* PriorityQueue<T>::toArray(int size) 

{

	T* arr = new T[size];
	for(int i=0;i<size;i++)
	{
		arr[i] = find_max();
	}
	return arr;
}
template<typename T>
PriorityQueue<T>::~PriorityQueue()
{

	dtree(Root);
}
#endif
				