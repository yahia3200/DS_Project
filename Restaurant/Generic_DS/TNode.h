#ifndef _TNODE_H
#define _TNODE_H
template<typename T>
class Node
{
private:
	T item; //// A data item
	Node<T>* Left; //Pointer to left Node 
	Node<T>* Right;    //Pointer to Right Node 
	float priority = 0;   // priority of the node
public:
	Node();
	Node(const T& id, float p = 0.0);
	Node(Node<T>* ptr);  //copy constructor
	void setitem(const T& id);
	void setpriority(float p);
	void setright(Node<T>* r_item);
	void setleft(Node<T>* l_item);

	T getitem() const;
	float getpriority() const;
	Node<T>*& getright();
	Node<T>*& getleft();
};//end node
template<typename T>
Node<T>::Node(Node<T>* ptr)
{
	item = ptr->getitem();
	priority = ptr->getpriority();
	Right = ptr->getright();
	Left = ptr->getleft();
}
template<typename T>
Node<T>::Node()
{
	Left = Right = nullptr;
}
template<typename T>
Node<T>::Node(const T& data, float p)
{
	item = data;
	priority = p;
	Left = Right = nullptr;
}
template<typename T>
void Node<T>::setitem(const T& data)
{
	item = data;
}
template<typename T>
void Node<T>::setpriority(float p)
{
	priority = p;
}
template<typename T>
void Node<T>::setright(Node<T>* r_item)
{
	Right = r_item;
}
template<typename T>
void Node<T>::setleft(Node<T>* l_item)
{
	Left = l_item;
}
template<typename T>
T Node<T>::getitem() const
{
	return item;
}
template<typename T>
float Node<T>::getpriority() const
{
	return priority;
}

template<typename T>
Node<T>*& Node<T>::getright()
{
	return Right;
}
template<typename T>
Node<T>*& Node<T>::getleft()
{
	return Left;
}



#endif
