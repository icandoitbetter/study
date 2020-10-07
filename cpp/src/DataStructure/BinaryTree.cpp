//二叉树类； 
#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "LinkNode.cpp"

using namespace std;
enum T_flag{Visit = true, Delete = false};
enum PoiterTag{thread, child};						//child == 0:指向孩子；thread == 1:指向线索； 
template <class T>
struct BTreeNote{
	T data;
	int balance = 0;								//平衡因子； 
	PoiterTag LTag = thread, RTag = thread;			//线索标记； 
	BTreeNote *lchild = nullptr;
	BTreeNote *rchild = nullptr;
};
template <class T>
class BTree{
	public:
		bool Is_thread;								//成员变量；
		BTreeNote<T> *Root;							//成员变量； 
		//成员函数； 
		BTree(){Root = nullptr;Is_thread = false;} 
		void visit(T d){cout << d << " ";}			//元素的访问函数 ； 
		void Insert_level(T d);						//插入操作（层次遍历）； 
		bool Insert_BSTree(T d);					//插入操作（二叉排序树）；
		bool Traverse_level(T_flag form);			//遍历操作（层次遍历）；
		bool Traverse_InOrder(BTreeNote<T> *p); 	//遍历操作（中序递归遍历）； 
		bool Delete_BSTree(T d);					//删除操作（二叉排序树）； 
		bool Thread_InOrder();						//线索化操作（中序线索化）； 
		bool Traverse_Thread();						//遍历操作（遍历线索化二叉树）； 
		
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
	while(p && p->data != d){ 										//寻找删除对象； 
		front = p;
		PoiterTag p_tag;
		if(d < p->data) p_tag = p->LTag, p = p->lchild;
		else            p_tag = p->RTag, p = p->rchild;
		if(p_tag == thread) p = nullptr;
	}																//p指针所指即为删除对象；
	if(!p) return false;
	if(p->LTag == child && p->RTag == child){						//1.对有左右子树的删除对象p，转变为第2/3/4种类型； 
		BTreeNote<T> *q = p->lchild, *t = p;
		while(q->RTag == child) t = q, q = q->rchild;
		p->data = q->data, p = q, front = t;
	}
	if(p == Root){													//2.删除对象为根结点； 
		if(Root->LTag == child) Root = Root->lchild;
		else					Root = Root->rchild;
	}
	else if(p->LTag == child){										//3.删除对象只有左子树； 
		if(front->lchild == p && front->LTag == child)
				front->lchild = p->lchild, front->LTag = p->LTag; 
		else    front->rchild = p->lchild, front->RTag = p->LTag;
	}
	else {															//4.删除对象只有右子树或为叶子结点； 
		if(front->lchild == p && front->LTag == child) 
				front->lchild = p->rchild, front->LTag = p->RTag;
		else	front->rchild = p->rchild, front->RTag = p->RTag;
	}
	if(this->Is_thread){											//对线索化二叉树的删除结点，连接其前驱和后继； 
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
			cout << "Thread_InOrder(): 重复线索化！\n";
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
			if(!s.pop(p)){											//若为最后一个结点，则跳出循环 
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
			cout << "Traverse_Thread(): 还未线索化！\n";
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
