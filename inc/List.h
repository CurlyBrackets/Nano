#ifndef __LIST_H__
#define __LIST_H__
#include <initilizer_list>
#include <algorithm>

/*
	must implement next() and prev() for get/set
	Node must have >/</== for sorting
	Assumes Node links are NULL to start
	Node must have copy ctor
*/

class Node{
	public:
		virtual Node():
		_n(NULL),_p(NULL){}
		virtual Node(const Node& b) = 0;//must include copying of all members as well as calling Node
		void next(const Node* value){
			_n = value;
		}
		Node* next() const{
			return _n;
		}
		void prev(const Node* value){
			_p = value;
		}
		Node* prev() const{
			return _p;
		}
	protected:
		Node* _n, *_p;
}

template <class T>
class List{
	public:
		class iterator{
			public:
				iterator(T* node):
				current(node),end(false){

				}
				iterator():
				end(true),current(NULL){

				}
				void operator++(){
					if(current->next())
						current = current->next();
					else
						end = true;
				}
				void operator--(){
					if(current->prev())
						current = current->prev();
					else
						end = true;
				}
				bool operator==(const iterator& b){
					if(b.end())
						return end();
					return *b == **this;
				}
				T* operator*(){
					return current;
				}
				bool end() const{
					return _end;
				}
			private:
				T* current;
				bool _end;
		};
	List(initializer_list<T*> initial):
	size(0),first(NULL),last(NULL){
		for(T* node:inital){
			push_back(node);
		}
	}

	iterator begin(){
		return iterator(first);
	}
	iterator end(){
		return iterator();
	}
	void push_back(T* node){
		if(!first){
			first = node;
			last = node;
		}
		else{
			last->next(node);
			node->prev(last);
			last = node;
		}
		size++;
	}
	void insert(T* node, unsigned int index){
		T* n = (*this)[index];
		node->next(n->next);
		node->next()->prev(node);
		node->prev(n);
		n->next(node);
		size++;
	}
	T* pop(unsigned int pos){
		T* n = (*this)[pos];
		n->prev()->next(n->next());
		n->next()->prev(n->prev());
		size--;
		return n;
	}


	T* operator[](unsigned int index){
		T* temp = first;
		for(unsigned int i=0;i<index && temp;i++){
			temp = temp->next();
			if(!temp)
				return NULL;
		}
		return temp;
	}
	void operator+=(const List& b){//this acts as a copy
		if(!b.size())
			return;
		std::for_each(b.begin(), b.end(), [this](T* n){this->push_back(new T(*n));});
	}

	protected:
		T* first, *last;
		unsigned int size;
};

#endif // __LIST_H__
