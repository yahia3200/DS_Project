#ifndef _TNODE_H
#define _TNODE_H
template<typename T>
class TNode
{
private:
	T item; //// A data item
	TNode<T>* Left; //Pointer to left Node 
	TNode<T>* Right;    //Pointer to Right Node 
	float priority = 0;   // priority of the node
public:
	TNode();
	TNode(const T& id, float p = 0.0);
	TNode(TNode<T>* ptr);  //copy constructor
	void setitem(const T& id);
	void setpriority(float p);
	void setright(TNode<T>* r_item);
	void setleft(TNode<T>* l_item);
	T getitem() const;
	float getpriority() const;
	TNode<T>*& getright();
	TNode<T>*& getleft();
};//end node

template<typename T>
TNode<T>::TNode(TNode<T>*ptr)
{
	item = ptr->getitem();
	priority = ptr->getpriority();
	Right = ptr->getright();
	Left = ptr->getleft();
}
template<typename T>
TNode<T>::TNode()
{
	Left = Right = nullptr;
}
template<typename T>
TNode<T>::TNode(const T& data, float p)
{
	item = data;
	priority = p;
	Left = Right = nullptr;
}
template<typename T>
void TNode<T>::setitem(const T& data)
{
	item = data;
}
template<typename T>
void TNode<T>::setpriority(float p)
{
	priority = p;
}
template<typename T>
void TNode<T>::setright(TNode<T>* r_item)
{
	Right = r_item;
}
template<typename T>
void TNode<T>::setleft(TNode<T>* l_item)
{
	Left = l_item;
}
template<typename T>
T TNode<T>::getitem() const
{
	return item;
}
template<typename T>
float TNode<T>::getpriority() const
{
	return priority;
}

template<typename T>
TNode<T>*& TNode<T>::getright()
{
	return Right;
}
template<typename T>
TNode<T>*& TNode<T>::getleft()
{
	return Left;
}
#endif
