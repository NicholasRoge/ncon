/******************************************************************************\
|File:  /src/headers/list.h     											   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#ifndef LIST_H
#define LIST_H

#include <iostream>
//#include "exception.h"

/***************************************************************\
|Class[ List]:													|
|	This template class is intended to be used by the programmer|
|	to replace the std::Vector class object.					|
\***************************************************************/
template <typename T> class List{
	protected:
		T* _list;
		unsigned _size;
		unsigned _currentIndex;

	public:
		/***************************************************************\
		|Method[ List ]:												|
		|	Constructor;  Simply sets this classes private variables to |
		|	their default values.										|
		\***************************************************************/
		List();
		
		/***************************************************************\
		|Method[ List ]:												|
		|	Constructor;  Simply sets this classes private variables to |
		|	their default values, and performs a deep copy of the _list	|
		|	variable.													|
		|																|
		|Param[ copy ]:													|
		|	List to be passed and copied.								|
		\***************************************************************/
		List(const List &copy);
		
		/***************************************************************\
		|Method[ ~List ]:												|
		|	Destructor;  Clears out the data in the _list variable.		|
		\***************************************************************/
		~List();

		/***************************************************************\
		|Method[ addElement ]:											|
		|	Adds the element 'element' at the specified index.  If no	|
		|	index is given, the element will be added to the top (last	|
		|	element) of the list.										|
		|Return:														|
		|	Returns the number fo elements in the array after the		|
		|	delete.														|
		|Throws[ Exception ]:											|
		|	Throws generic exception if:								|
		|		An attempt is made to access an element of the array	|
		|		which is outside of the bounds (0 to getSize()) of the  |
		|		list.													|
		|																|
		|Param[ element ]:												|
		|	Value to be added to _list.									|
		\Param[ index ]:												|
		|	Specifies the location in _list where 'element' should be 	|
		|	added.														|
		\***************************************************************/
		unsigned addElement(const T& element,unsigned index);
		
		/***************************************************************\
		|Method[ addElement ]:											|
		|	Wrapper for above addElement.  Will always add 'element' to |
		|	the top of the List.										|
		|																|
		|Note:															|
		|	See above definition for specific uses.						|
		\***************************************************************/
		unsigned addElement(const T& element);

		/***************************************************************\
		|Method[ removeElementAt ]:										|
		|	Removes element at given index.								|
		|Throws[ Exception ]:											|
		|	Throws generic exception if:								|
		|		An attempt is made to access an element of the array	|
		|		which is outside of the bounds (0 to getSize()) of the  |
		|		list.													|
		|																|
		|Param[ index ]:												|
		|	Index of the element in _list to be removed.				|
		\***************************************************************/
		unsigned removeElementAt(unsigned index);
		
		/***************************************************************\
		|Method[ removeElement ]:										|
		|	Iterates through _list and attempts to find element whose	|
		|	value is equal to 'element', and deletes the element at that|
		|	index.														|
		|Return:														|
		|	Returns the number fo elements in the array after the		|
		|	delete.														|
		|Throws[ Exception ]:											|
		|	Throws generic exception if:								|
		|		An attempt is made to access an element of the array	|
		|		which is outside of the bounds (0 to getSize()) of the  |
		|		list.													|
		|																|
		|Param[ element ]:												|
		|	Element to be found and deleted.							|
		\***************************************************************/
		unsigned removeElement(const T &element);

		/***************************************************************\
		|Method[ getElementAt ]:										|
		|	Gets teh element at the specified index.					|
		|Return:														|
		|	Returns the value of the element at _list[index].			|
		|																|
		|Param[ index ]:												|
		|	Index of the element in _list to get.						|
		\***************************************************************/
		T getElementAt(unsigned index) const;

		/***************************************************************\
		|Method[ getSize ]:												|
		|	Gets the number of elements in the array.					|
		|Return:														|
		|	Returns the number of elements in the array.				|
		|Throws[ Exception ]:											|
		|	Throws generic exception if:								|
		|		An attempt is made to access an element of the array	|
		|		which is outside of the bounds (0 to getSize()) of the  |
		|		list.													|
		|																|
		|Param[ copy ]:													|
		|	Gets the number of elements currently added to the array.	|
		|																|
		|Note:															|
		|	This is not the value of _size, which is the amount of		|
		|	space currently allocated.									|
		\***************************************************************/
		unsigned getSize() const;

		/***************************************************************\
		|Method[ clear ]:												|
		|	Deletes the _list array and sets all the private variables	|
		|	back to their default values.								|
		\***************************************************************/
		void clear();

		/***************************************************************\
		|Method[ operator[] ]:											|
		|	Operator overload;  Gets the value at the specified index.	|
		|Return:														|
		|	Returns a reference to the element at index which may be	|
		|	modified by the user in the same way that an array element	|
		|	may be modified.											|
		|Throws[ Exception ]:											|
		|	Throws generic exception if:								|
		|		An attempt is made to access an element of the array	|
		|		which is outside of the bounds (0 to getSize()) of the  |
		|		list.													|
		|																|
		|Param[ index ]:												|
		|	Index of the element in _list to get.						|
		\***************************************************************/
		T& operator[](unsigned index) const;

		/***************************************************************\
		|Method[ operator= ]:											|
		|	Operator overload;  Turns the calling object into a copy	|
		|	of the List object on the right of the equal sign.			|
		|Return:														|
		|	Returns a reference to calling object.						|
		|																|
		|Param[ copy ]:													|
		|	List object to be copied.									|
		\***************************************************************/
		List<T>& operator=(const List<T> &copy);
		
		/***************************************************************\
		|Method[ operator+= ]:											|
		|	Adds all of 'add's elements to the top of _list.			|
		|Return:														|
		|	Returns a reference to calling object.						|
		|																|
		|Param[ add ]:													|
		|	List object to be added to the top of stack.				|
		\***************************************************************/
		List<T>& operator+=(const List<T> &add);
		
		/***************************************************************\
		|Method[ operator+ ]:											|
		|	Adds the value to the left and right of the + operator and	|
		|	returns the results.										|
		|Return:														|
		|	The combiniation of '*this' and 'add'						|
		|																|
		|Param[ listToCopy ]:											|
		|	List to be added and returned.								|
		\***************************************************************/
		List<T> operator+(const List<T> &listToCopy);
};



/******************************************************************************\
|Class Definitions:												               |
\******************************************************************************/
template <typename T> List<T>::List(){
	this->_list=NULL;
	this->_size=0;
	this->_currentIndex=0;
}

template <typename T> List<T>::List(const List &copy){
	this->_list=NULL;
	this->_size=0;
	this->_currentIndex=0;
	
	//I DUN LIKE THIS, but apparently I can't use memcpy to do this.  :<
	for(unsigned index=0;index<copy.getSize();index++){
		this->addElement(copy[index]);
	}
}

template <typename T> List<T>::~List(){
	this->clear();
}

template <typename T> unsigned List<T>::addElement(const T& element,unsigned index){
	T* list_tmp_1=NULL;
	T* list_tmp_2=NULL;

	if(index<0||index>this->getSize()){  //in this particular case, index can be greater than or equal to size in order to push something onto the top of the stack
		//throw Exception("Attempt to access index location outside of List bounds.",1);
	}

	if(this->_currentIndex==this->_size){  //We need to expand the array to add more elements
		if(this->_size==0){
			this->_list=new T[10];
			this->_list[0]=element;
		}else{
			if(index==0){
				list_tmp_2=new T[this->_size];
				for(unsigned sub_index=0;sub_index<this->_size;sub_index++){
					list_tmp_2[sub_index]=this->_list[sub_index];
				}

				//delete pointer
				delete[] this->_list;
				this->_list=new T[this->_size+10];
				for(unsigned sub_index=0;sub_index<this->_size;sub_index++){
					this->_list[sub_index+1]=list_tmp_2[sub_index];
				}
				this->_list[0]=element;

				delete[] list_tmp_2;
			}else if(index==this->_size){
				list_tmp_1=new T[this->_size];
				for(unsigned sub_index=0;sub_index<this->_size;sub_index++){
					list_tmp_1[sub_index]=this->_list[sub_index];
				}

				//delete pointer
				delete[] this->_list;
				this->_list=new T[this->_size+10];
				for(unsigned sub_index=0;sub_index<this->_size;sub_index++){
					this->_list[sub_index]=list_tmp_1[sub_index];
				}
				this->_list[this->_size]=element;

				delete[] list_tmp_1;
			}else{
				list_tmp_1=new T[index];  //[x][x][index] - Number of elements before index will always be index
				list_tmp_2=new T[this->_size-index];
				for(unsigned sub_index=0;sub_index<index;sub_index++){
					list_tmp_1[sub_index]=this->_list[sub_index];
				}
				for(unsigned sub_index=0;sub_index<(this->_size-index);sub_index++){
					list_tmp_2[sub_index]=this->_list[sub_index+index];
				}

				//delete the pointer
				delete[] this->_list;
				this->_list=new T[this->_size+10];

				for(unsigned sub_index=0;sub_index<index;sub_index++){
					this->_list[sub_index]=list_tmp_1[sub_index];
				}
				for(unsigned sub_index=0;sub_index<(this->_size-index);sub_index++){
					this->_list[sub_index+index+1]=list_tmp_2[sub_index];
				}
				this->_list[index]=element;

				delete[] list_tmp_1;
				delete[] list_tmp_2;
			}
		}

		this->_size+=10;
	}else{
		//Copy into tmp array
		if(index<this->_currentIndex){
			list_tmp_1=new T[this->_currentIndex-index];  //We don't need to allocate 'size' memory because we don't need that much memory
			for(unsigned sub_index=index;sub_index<this->_currentIndex;sub_index++){
				list_tmp_1[sub_index-index]=this->_list[sub_index];
			}

			//Copy back into main array
			for(unsigned sub_index=index;sub_index<=this->_currentIndex;sub_index++){
				if(sub_index==index){
					this->_list[sub_index]=element;
				}else{ //if(sub_index>index)
					this->_list[sub_index]=list_tmp_1[sub_index-index-1];
				}
			}
			delete[] list_tmp_1;
		}else{
			this->_list[index]=element;
		}
	}
	this->_currentIndex++;

	return this->getSize();
}

template <typename T> unsigned List<T>::addElement(const T& element){
	return this->addElement(element,this->getSize());
}

template <typename T> unsigned List<T>::removeElementAt(unsigned index){
	T* list_tmp_1;
	T* list_tmp_2;
	
	if(index<0||index>this->_currentIndex){
		//throw Exception("Attempt to access index location outside of List bounds.",1);
	}

	if(index==0){
		list_tmp_2=new T[this->_size-1];
		for(unsigned sub_index=1;sub_index<this->_size;sub_index++){
			list_tmp_2[sub_index-1]=this->_list[sub_index];
		}

		//delete pointer
		delete[] this->_list;
		this->_list=new T[this->_size-1];
		for(unsigned sub_index=0;sub_index<this->_size-1;sub_index++){
			this->_list[sub_index]=list_tmp_2[sub_index];
		}

		delete[] list_tmp_2;
	}else if(index==(this->_size-1)){
		list_tmp_1=new T[this->_size-1];
		for(unsigned sub_index=0;sub_index<this->_size-1;sub_index++){
			list_tmp_1[sub_index]=this->_list[sub_index];
		}

		//delete pointer
		delete[] this->_list;
		this->_list=new T[this->_size-1];
		for(unsigned sub_index=0;sub_index<this->_size-1;sub_index++){
			this->_list[sub_index]=list_tmp_1[sub_index];
		}

		delete[] list_tmp_1;
	}else{
		list_tmp_1=new T[index];  //[x][x][index] - Number of elements before index will always be index
		list_tmp_2=new T[this->_size-index-1];
		for(unsigned sub_index=0;sub_index<index;sub_index++){
			list_tmp_1[sub_index]=this->_list[sub_index];
		}
		for(unsigned sub_index=0;sub_index<(this->_size-index-1);sub_index++){  //TODO:  check this for accuracy
			list_tmp_2[sub_index]=this->_list[sub_index+(index+1)];
		}

		//delete the pointer
		delete[] this->_list;
		this->_list=new T[this->_size-1];

		for(unsigned sub_index=0;sub_index<index;sub_index++){
			this->_list[sub_index]=list_tmp_1[sub_index];
		}
		for(unsigned sub_index=0;sub_index<(this->_size-index-1);sub_index++){
			this->_list[sub_index+index]=list_tmp_2[sub_index];
		}

		delete[] list_tmp_1;
		delete[] list_tmp_2;
	}

	this->_size--;
	this->_currentIndex--;

	return this->_currentIndex;
}

template <typename T> unsigned List<T>::removeElement(const T &element){
	for(unsigned index=0;index<this->_size;index++){
		if(this->_list[index]==element){
			return removeElementAt(index);
		}
	}

	return this->_size;
}

template <typename T> T List<T>::getElementAt(unsigned index) const{
	if(index<0||index>=this->getSize()){
		//throw Exception("Attempt to access index location outside of List bounds.",1);
	}

	return this->_list[index];
}

template <typename T> unsigned List<T>::getSize() const{
	return this->_currentIndex;
}

template <typename T> void List<T>::clear(){
	if(this->_list!=NULL){
		delete[] this->_list;
		this->_list=NULL;	
		this->_size=0;
		this->_currentIndex=0;
	}
}

template <typename T> T& List<T>::operator[](unsigned index) const{
	if(index<0||index>=this->getSize()){
		//throw Exception("Attempt to access index location outside of List bounds.",1);
	}

	return this->_list[index];
}

template <typename T> List<T>& List<T>::operator=(const List<T>& copy){
	this->clear();

	for(unsigned index=0;index<copy.getSize();index++){
		this->addElement(copy[index]);
	}

	return *this;
}

template <typename T> List<T>& List<T>::operator+=(const List<T>& add){
	for(unsigned index=0;index<add.getSize();index++){
		this->addElement(add[index]);
	}

	return *this;
}

template <typename T> List<T> List<T>::operator+(const List<T>& listToCopy){
	List<T> copy=*this;
	copy+=listToCopy;

	return copy;
}

#endif