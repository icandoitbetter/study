#ifndef LINKNOTE_H
#define LINKNOTE_H

using namespace std;

//单链表类；
template<class T>
struct node{
	T data;
	node<T> *next;
};

template<class T>	
class Link{
	public:
		node<T> *head, *end;
		
		Link();
		Link(T d);
		Link(Link &L);						//自定义复制构造函数； 
	 	virtual ~Link();
		bool Insert(T d);					//插入链表表尾 ； 
		bool show();						//遍历并打印全表； 
		T & operator [](int num);			//重载"[]"运算符； 
		bool LinkDelete(int num, T &e);		//删除第i个元素； 
		void Remove(T d);					//删除与d相等的元素；
		int Length();						//求表长； 
		bool Is_empty();					//判空； 
};
template<class T>
Link<T>::Link(){
	head = end = new node<T>;
	head->next = nullptr;
}
template<class T>
Link<T>::Link(T d){
	head = end = new node<T>;
	end->next = new node<T>;
	end = end->next;
	end->data = d;
	end->next = nullptr;
}
template<class T>
Link<T>::Link(Link<T> &L){
	head = end = new node<T>;
	head->next = nullptr;
	for(node<T> *p = L.head->next; p !=nullptr; p = p->next){
		this->Insert(p->data);
	}
}
template<class T>
Link<T>::~Link(){
	node<T> *q = nullptr, *p = head;
	while(p != nullptr){
		q = p;
		p = p->next;
		delete q;
	}
}
template<class T>
bool Link<T>::Insert(T d){
	if(!(end->next = new node<T>)) return false;
	end = end->next;
	end->data = d;
	end->next = nullptr;
	return true;
}
template<class T>
bool Link<T>::show(){
	if(!(head->next)) exit(false);
	for(node<T> *p = head->next; p != nullptr; p = p->next)
		cout << p->data << " ";
	cout << "\n";
	return true;
}
template<class T>
T & Link<T>::operator [](int num){
	int i = 0;
	node<T> *p;
	for(p = head->next; (p->next != nullptr)&&(i < num); p = p->next, i++);
	return p->data;
}
template<class T>
bool Link<T>::LinkDelete(int num, T &e){
	int i = 0;
	node<T> *front = head;
	node<T> *p = head;
	for( ; (p != nullptr)&&(i != num); p = p->next, i++);
	if((p != nullptr)&&(i == num)){
		for( ; front->next != p; front = front->next);
		front->next = p->next;
		e = p->data, delete p; 
		if(head->next == nullptr) end = head;
		return true;
	}
	else return false;
}
template<class T>
void Link<T>::Remove(T d){
	node<T> *p, *front;
	p = head->next;
	front = head;
	while(p != nullptr){
		if(p->data == d){
			front->next = p->next;
			delete p;
			p = front->next;
			continue;
		}
		front = p;
		p = p->next;
	}
	end = front;
}
template <class T>
int Link<T>::Length(){
	int len = 0;
	node<T> *p = nullptr;
	for(p = head->next; p != nullptr; p = p->next){
		len++;
	}
	return len;
}
template<class T>
bool Link<T>::Is_empty(){
	if(head->next == nullptr) return true;
	else return false;
} 
//由单链表类派生出的堆栈类； 
template<class T>
class Stack: public Link<T>{
	public:
		Stack(): Link<T>(){};
		Stack(T d): Link<T>(d){};
		virtual ~Stack(){};
		bool push(T d);		//入栈； 
		bool pop(T &e);		//出栈； 
		T & GetTop();		//取栈顶元素； 
};
template<class T>
bool Stack<T>::push(T d){
	node<T> *p = nullptr;
	if(!(p = new node<T>)) return false;
	p->data = d;
	p->next = this->head->next;
	this->head->next = p;
	return true;
}
template<class T>
bool Stack<T>::pop(T &e){
	node<T> *p = this->head->next;
	if(p == nullptr) return false;
	this->head->next = p->next;
	e = p->data;
	if(p == this->end){
		this->end = this->head;
		this->head->next = nullptr;
	}
	delete p; 
	return true;
}
template<class T>
T & Stack<T>::GetTop(){
	return this->head->next->data;
} 
//由单链表类派生出的队列类；
template<class T>
class Queue: public Link<T>{
	public:
		Queue(): Link<T>(){};
		virtual ~Queue(){};
		bool EnQueue(T d);		//入队； 
		bool DeQueue(T &e);		//出队； 
		T &GetHead();			//求队首元素； 
};
template<class T>
bool Queue<T>::EnQueue(T d){
	return this->Insert(d);
}
template<class T>
bool Queue<T>::DeQueue(T &e){
	node<T> *p = this->head->next;
	e = p->data;
	this->head->next = p->next;
	if(this->head->next == nullptr) this->end = this->head;
	delete p;
	return true;
}
template<class T>
T &  Queue<T>:: GetHead(){
	return this->head->next->data;
}
#endif







