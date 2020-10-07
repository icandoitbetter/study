#ifndef LINKNOTE_H
#define LINKNOTE_H

using namespace std;

//�������ࣻ
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
		Link(Link &L);						//�Զ��帴�ƹ��캯���� 
	 	virtual ~Link();
		bool Insert(T d);					//���������β �� 
		bool show();						//��������ӡȫ�� 
		T & operator [](int num);			//����"[]"������� 
		bool LinkDelete(int num, T &e);		//ɾ����i��Ԫ�أ� 
		void Remove(T d);					//ɾ����d��ȵ�Ԫ�أ�
		int Length();						//����� 
		bool Is_empty();					//�пգ� 
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
//�ɵ��������������Ķ�ջ�ࣻ 
template<class T>
class Stack: public Link<T>{
	public:
		Stack(): Link<T>(){};
		Stack(T d): Link<T>(d){};
		virtual ~Stack(){};
		bool push(T d);		//��ջ�� 
		bool pop(T &e);		//��ջ�� 
		T & GetTop();		//ȡջ��Ԫ�أ� 
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
//�ɵ��������������Ķ����ࣻ
template<class T>
class Queue: public Link<T>{
	public:
		Queue(): Link<T>(){};
		virtual ~Queue(){};
		bool EnQueue(T d);		//��ӣ� 
		bool DeQueue(T &e);		//���ӣ� 
		T &GetHead();			//�����Ԫ�أ� 
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







