#ifndef __LIST_H__
#define __LIST_H__
#include <initializer_list>
#include <algorithm>

#include <fstream>

/*
	must implement next() and prev() for get/set
	Node must have >/</== for sorting
	Assumes Node links are NULL to start
	Node must have copy ctor
*/

template <class T>
class List{
	public:
		class base_iterator{
			public:
				base_iterator(T* node):
				current(node),_end(false){

				}
				base_iterator():
				current(NULL),_end(true){

				}
				void operator++(){
					if(current->next())
						current = current->next();
					else
						_end = true;
				}
				void operator--(){
					if(current->prev())
						current = current->prev();
					else
						_end = true;
				}
				bool end() const{
					return _end;
				}
			protected:
				T* current;
				bool _end;
		};
		class iterator:public base_iterator{
			public:
				iterator(T* node){
					this->current = node;
					if(!node)
						this->_end = true;
					else
						this->_end = false;
				}
				iterator(){
					this->current = NULL;
					this->_end = true;
				}
				bool operator==(iterator b){
					if(b.end())
						return end();
					return *b == **this;
				}
				bool operator!=(iterator b){
					if(b.end())
						return !end();
					return *b != **this;
				}
				T* operator*(){
					return this->current;
				}
		};
		class const_iterator:public base_iterator{
			public:
				const_iterator(T* node){
					this->current = node;
					if(!node)
						this->_end = true;
					else
						this->_end = false;
				}
				const_iterator(){
					this->current = NULL;
					this->_end = true;
				}
				const_iterator(const iterator& b){
					this->current = *b;
					this->_end = b.end();
				}
				bool operator==(const const_iterator& b){
					if(b.end())
						return end();
					return *b == **this;
				}
				bool operator!=(const const_iterator& b){
					if(b.end())
						return !end();
					return *b != **this;
				}
				T* operator*() const{
					return const_cast<T*>(this->current);
				}
		};
		List():
		first(NULL),last(NULL),size(0){

		}
		List(std::initializer_list<T*> initial):
		first(NULL),size(0),last(NULL){
			for(auto it = initial.begin();it != initial.end();++it){
				push_back(*it);
			}
		}

		iterator begin(){
			return iterator(first);
		}
		iterator end(){
			return iterator();
		}
		const_iterator cbegin() const{
			return const_iterator(first);
		}
		const_iterator cend() const{
			return const_iterator();
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

		//standard index based methods

		void insert(T* node, unsigned int index){
			/*T* n = *iter_at(index);
			node->next(n->next());
			node->next()->prev(node);
			node->prev(n);
			n->next(node);
			size++;*/
			insert(node, iter_at(index));
		}
		T* pop(unsigned int pos){
			/*T* n = (*this)[pos];
			n->prev()->next(n->next());
			n->next()->prev(n->prev());
			size--;
			return n;*/
			return pop(iter_at(pos));
		}

		//iterator based methods

		void insert(T* node, iterator it){
			if(it.end()){
				push_back(node);
			}
			else{
				if((*it)->prev()){
					node->prev((*it)->prev());
					node->prev()->next(node);
				}
				node->next((*it));
				(*it)->prev(node);
				size++;
			}
		}
		T* pop(iterator it){
			if(it.end())
				return NULL;
			if((*it)->next())
				(*it)->next()->prev((*it)->prev());
			if((*it)->prev())
				(*it)->prev()->next((*it)->next());
			size--;
			return (*it);
		}

		iterator iter_at(unsigned int index){
			iterator ret = begin();
			for(unsigned int i=0;i<index && ret != end();i++){
				++ret;
			}
			return ret;
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
			if(!b.length())
				return;
			std::for_each(b.cbegin(), b.cend(), [this](T* n){this->push_back(new T(*n));});
		}

		unsigned int length() const{
			return size;
		}

	protected:
		T* first, *last;
		unsigned int size;
};

#endif // __LIST_H__
