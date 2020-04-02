#include"TNode.h"
#include<iostream> 
using namespace std;
#ifndef _PRIORITY_QUEUE_H
#define _PRIORITY_QUEUE_H


template<typename T>
class PriorityQueue
{
	Node<T>* Root;
	int count = 0; //number of nodes in the tree
	Node<T>* DeleteNode(Node<T>* root, int value, Node<T>*& remove) //Function to delete node which its priority corresponds to "value"
	{
		Node<T>* Rptr = Root;
		if (root == NULL) return root;
		if (Root->getpriority() == value) //in case the node to be deleted is the root 
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

		if (value < root->getpriority())
			root->setleft(DeleteNode(root->getleft(), value, remove));
		else if (value > root->getpriority())
			root->setright(DeleteNode(root->getright(), value, remove));

		else // if value is same as root's priority, then This is the node to be deleted 
		{

			if (root->getright() == NULL)
			{
				Node<T>* target = new Node<T>(root);
				remove = target;
				Node<T>* temp = root;
				root = root->getleft();
				delete temp;
			}
			else if (root->getleft() == NULL)
			{
				Node<T>* target = new Node<T>(root);
				remove = target;
				Node<T>* temp = root;
				root = root->getright();
				delete temp;
			}
			else if (root->getright() == NULL && root->getleft() == NULL)
			{
				Node<T>* target = new Node<T>(root);
				remove = target;
				delete root;
				root = NULL;
			}
			else
			{
				Node<T>* ptr = rec_max(root->getright());
				root->setitem(ptr->getitem());
				root->setpriority(ptr->getpriority());
				root->getright() = DeleteNode(root->getright(), ptr->getpriority(), remove);
			}
		}

		return root;
	}
	void insert(Node<T>*& subroot, T id, int p) // insert new node based on it's priority
	{
		if (subroot == NULL)
			subroot = new Node<T>(id, p);
		else if (subroot->getpriority() < p)
		{
			insert(subroot->getright(), id, p);
		}
		else
		{
			insert(subroot->getleft(), id, p);
		}
	}

	Node<T>* rec_max(Node<T>* subroot) // recursive function to find max priority of the nodes
	{
		if (!subroot) return nullptr;
		Node<T>* max = subroot;
		Node<T>* Lmax = rec_max(subroot->getleft());
		Node<T>* Rmax = rec_max(subroot->getright());
		if (Lmax && Lmax->getpriority() > max->getpriority())   max = Lmax;
		if (Rmax && Rmax->getpriority() > max->getpriority())  max = Rmax;
		//cout << max << endl;
		return max;
	}
	Node<T>* rec_min(Node<T>* subroot) // recursive function to find min priority of the nodes
	{
		if (!subroot) return nullptr;
		Node<T>* min = subroot;
		Node<T>* Rmin = rec_min(subroot->getright());
		Node<T>* Lmin = rec_min(subroot->getleft());
		if (Lmin && Lmin->getpriority() < min->getpriority())   min = Lmin;
		if (Rmin && Rmin->getpriority() < min->getpriority())  min = Rmin;
		return min;
	}
	void preorder(Node<T>* subroot) // in sequence (root -> left -> right)
	{
		if (Root == nullptr) { cout << "NULL"; return; }
		if (subroot == nullptr) { return; }
		cout << subroot->getitem() << ',' << subroot->getpriority() << "  ";
		preorder(subroot->getleft());
		preorder(subroot->getright());
	}

	void dtree(Node<T>*& subroot) //Destroy Tree (Deletes all the nodes in the tree)
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
	PriorityQueue()
	{
		Root = nullptr;
	}
	Node<T>* getroot()const
	{
		return Root;
	}

	Node<T>* find_max()
	{
		return rec_max(this->Root);
	}
	Node<T>* find_min()
	{
		return rec_min(this->Root);
	}
	void Enqueue(T item, float p)
	{
		count++;
		insert(Root, item, p);

	}
	Node<T>* Dequeue()
	{
		Node<T>* max = find_max();

		Node<T>* deleted = nullptr;
		if (!max) return deleted;
		DeleteNode(Root, max->getpriority(), deleted);

		return deleted;
	}


	void Print_preorder() // just for testing the implemention
	{
		preorder(Root);
	}

	~PriorityQueue()
	{

		dtree(Root);
	}


};
#endif
