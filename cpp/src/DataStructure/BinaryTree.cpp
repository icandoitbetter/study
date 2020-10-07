//�������ࣻ 
#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "LinkNode.cpp"

using namespace std;
enum T_flag{Visit = true, Delete = false};
enum PoiterTag{thread, child};						//child == 0:ָ���ӣ�thread == 1:ָ�������� 
template <class T>
struct BTreeNote{
	T data;
	int balance = 0;								//ƽ�����ӣ� 
	PoiterTag LTag = thread, RTag = thread;			//������ǣ� 
	BTreeNote *lchild = nullptr;
	BTreeNote *rchild = nullptr;
};
template <class T>
class BTree{
	public:
		bool Is_thread;								//��Ա������
		BTreeNote<T> *Root;							//��Ա������ 
		//��Ա������ 
		BTree(){Root = nullptr;Is_thread = false;} 
		void visit(T d){cout << d << " ";}			//Ԫ�صķ��ʺ��� �� 
		void Insert_level(T d);						//�����������α������� 
		bool Insert_BSTree(T d);					//�����������������������
		bool Traverse_level(T_flag form);			//������������α�������
		bool Traverse_InOrder(BTreeNote<T> *p); 	//��������������ݹ�������� 
		bool Delete_BSTree(T d);					//ɾ���������������������� 
		bool Thread_InOrder();						//�������������������������� 
		bool Traverse_Thread();						//������������������������������ 
		
		~ BTree(){this->Traverse_level(Delete);}
};
template <class T>
void BTree<T>::Insert_level(T d){
	BTreeNote<T> *p = new BTreeNote<T>;
	p->data = d;
	Queue<BTreeNote<T> *> Q;
	BTreeNote<T> *q = Root;
	if(!Root) Root = p;
	else{
		Q.EnQueue(Root);
		while(!Q.Is_empty() && q != nullptr){
			Q.DeQueue(q);
			if(!(q->LTag == child && q->RTag == child)) break;
			if(q->LTag == child) Q.EnQueue(q->lchild);
			if(q->RTag == child) Q.EnQueue(q->rchild);
		}
		if(q->LTag == child) q->rchild = p, q->RTag = child;
		else                 q->lchild = p, q->LTag = child;
	}
}
template <class T>
bool BTree<T>::Insert_BSTree(T d){
	BTreeNote<T> *q = nullptr;
	if(!(q = new BTreeNote<T>)) exit(false);
	q->data = d;
	if(!Root) Root = q;
	else{
		BTreeNote<T> *p = Root, *front = nullptr;
		while(p){
			PoiterTag tag;
			front = p;
			if     (d < p->data) tag = p->LTag, p = p->lchild;
			else if(d > p->data) tag = p->RTag, p = p->rchild;
			else {delete q; return false;}
			if(tag !=child) p = nullptr;
		}
		if(d < front->data){
			if(this->Is_thread){
				q->lchild = front->lchild;
				q->rchild = front;
			}
			front->lchild = q;
			front->LTag = child;
		} 
		else {
			if(this->Is_thread){
				q->rchild = front->rchild;
				q->lchild = front;
			}
			front->rchild = q;
			front->RTag = child;
		}		        
	}
	return true;
}
template <class T>
bool BTree<T>::Traverse_level(T_flag form){
	if(!Root) return false;
	Queue<BTreeNote<T> *> Q;
	BTreeNote<T> *q = Root;
	Q.EnQueue(q);
	while(!Q.Is_empty() && q != nullptr){
		Q.DeQueue(q);
		if(q->LTag == child) Q.EnQueue(q->lchild);
		if(q->RTag == child) Q.EnQueue(q->rchild);
		form? this->visit(q->data): delete q;
	}
	return true;
}
template <class T>
bool BTree<T>::Traverse_InOrder(BTreeNote<T> *p){
	if(!p) return false;
	if(p->LTag == child)
		this->Traverse_InOrder(p->lchild);
	this->visit(p->data);
	if(p->RTag == child)
		this->Traverse_InOrder(p->rchild);
	return true;
}
template <class T>
bool BTree<T>::Delete_BSTree(T d){
	BTreeNote<T> *p, *front;
	front = p = Root;
	while(p && p->data != d){ 										//Ѱ��ɾ������ 
		front = p;
		PoiterTag p_tag;
		if(d < p->data) p_tag = p->LTag, p = p->lchild;
		else            p_tag = p->RTag, p = p->rchild;
		if(p_tag == thread) p = nullptr;
	}																//pָ����ָ��Ϊɾ������
	if(!p) return false;
	if(p->LTag == child && p->RTag == child){						//1.��������������ɾ������p��ת��Ϊ��2/3/4�����ͣ� 
		BTreeNote<T> *q = p->lchild, *t = p;
		while(q->RTag == child) t = q, q = q->rchild;
		p->data = q->data, p = q, front = t;
	}
	if(p == Root){													//2.ɾ������Ϊ����㣻 
		if(Root->LTag == child) Root = Root->lchild;
		else					Root = Root->rchild;
	}
	else if(p->LTag == child){										//3.ɾ������ֻ���������� 
		if(front->lchild == p && front->LTag == child)
				front->lchild = p->lchild, front->LTag = p->LTag; 
		else    front->rchild = p->lchild, front->RTag = p->LTag;
	}
	else {															//4.ɾ������ֻ����������ΪҶ�ӽ�㣻 
		if(front->lchild == p && front->LTag == child) 
				front->lchild = p->rchild, front->LTag = p->RTag;
		else	front->rchild = p->rchild, front->RTag = p->RTag;
	}
	if(this->Is_thread){											//����������������ɾ����㣬������ǰ���ͺ�̣� 
		BTreeNote<T> *prior = p->lchild, *tail = p->rchild;
		if(p->LTag == child){
			while(prior->RTag == child) prior = prior->rchild;
		}
		if(p->RTag == child){
			while(tail->LTag == child) tail = tail->lchild;
		}
		if(prior && prior->RTag == thread) prior->rchild = tail;
		if(tail  &&  tail->LTag == thread) tail->lchild = prior;
	}
	delete p;
	return true;
}
template <class T>
bool BTree<T>::Thread_InOrder(){ 
	if(this->Is_thread||Root == nullptr){
		if(this->Is_thread)
			cout << "Thread_InOrder(): �ظ���������\n";
		this->Is_thread = true;
		return false;
	}
	BTreeNote<T> *p = Root, *front = nullptr;
	Stack<BTreeNote<T> *> s(p);
	while(!(s.Is_empty())){
		if(p->LTag != child) s.pop(p);
		else p = p->lchild;
		while(p->LTag == child)	s.push(p), p = p->lchild;
		p->lchild = front;
//		visit(p->data);
		front = p;
		while(p->RTag != child){
			if(!s.pop(p)){											//��Ϊ���һ����㣬������ѭ�� 
				p = p->rchild = nullptr;
				break;
			}
//			visit(p->data);
			front->rchild = p;
			front = p;
		}
		if(s.Is_empty() && !p) break;
		p = p->rchild, s.push(p);
	}
	this->Is_thread = true;
	return true;
}
template<class T>
bool  BTree<T>::Traverse_Thread(){
	if(!this->Is_thread || Root == nullptr){
		if(!this->Is_thread)
			cout << "Traverse_Thread(): ��δ��������\n";
		return false;
	} 
	BTreeNote<T> *p = Root;
	while(p->LTag == child) p = p->lchild;
	while(p != nullptr){
		visit(p->data);
		if(p->RTag == child){
			p = p->rchild;
			while(p->LTag == child) p = p->lchild;
		}
		else p = p->rchild;
	}
	return true;
}
#endif
