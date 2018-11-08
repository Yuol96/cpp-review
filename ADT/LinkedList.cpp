#include <iostream>

template<class T>
class Node{
public:
	T val;
	Node * next;

	Node(const T v, Node * const nextNode = NULL);
	void print();
};

template<class T>
Node<T>::Node(const T v, Node<T>* const nextNode){
	val = v;
	next = nextNode;
}

template<class T>
void Node<T>::print(){
	std::cout << val << " ";
}

// template<class T>
// class LinkedList{
// public:
// 	Node<T> * head;
// 	Node<T> * tail;

// };

int main(int argc, char * argv[]){
	Node<double> b(200.3);
	Node<double> a(100.4, &b);
	a.print();
	a.next->print();
	std::cout << std::endl;
}