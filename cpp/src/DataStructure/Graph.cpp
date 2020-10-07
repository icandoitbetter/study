#ifndef GRAPH_H
#define GRAPH_H

#include <ctime>
#include <limits.h>
#include "LinkNode.cpp"

using namespace std;

enum Graph_tag{Undigraph, Digraph};
struct ArcBox{												//边表结点； 
	int tailvex = 0, headvex = 0;
	ArcBox *tlink = nullptr, *hlink = nullptr;
	int info;
	bool mark = false;
};
template <class T>
struct VexNode{												//顶点结点； 
	T data;
	ArcBox *firstin = nullptr, *firstout = nullptr;
	bool mark = false;
};
//用于测试的临时容器；
template <class T>
struct TestNode{ 
	T headvex, tailvex;
	int weight = 0;
};
template <class T>
class Test{
	public:
		TestNode<T> *arclist;
		T *vexlist;
		int arc_num, vex_num;
		Test(){
			arclist = nullptr;
			vexlist = nullptr;
			arc_num = vex_num = 0;
		}
		void Creat_TestArr(int vex, int arc, int weight_range);
		void show();
		~Test(){
			if(arclist) delete[] arclist;
			if(vexlist) delete[] vexlist;
		}
};
template <class T>																//随机生成图的函数； 
void Test<T>::Creat_TestArr(int vex, int arc, int weight_range){
	this->arclist = new TestNode<T>[arc];
	this->vexlist = new T[vex];
	this->arc_num = arc;
	this->vex_num = vex;
	for(int i = 0; i < vex; i++) vexlist[i] = arclist[i].headvex = 'A'+i;
	srand((int)(time(0)));
	for(int i = 0; i < arc; i++){
		arclist[i].headvex = vexlist[rand()%vex];
		arclist[i].tailvex = vexlist[rand()%vex];
		arclist[i].weight  = rand()%weight_range;
		while(arclist[i].weight == 0) arclist[i].weight  = rand()%weight_range;
	}
	
}
template <class T>
void Test<T>::show(){
	for(int i = 0; i < arc_num; i++){
		cout << arclist[i].headvex << ", ";
		cout << arclist[i].tailvex << ", ";
		cout << arclist[i].weight  << "\n";
	}
	cout << "arc_num = " << this->arc_num;
	cout << ", ";
	cout << "vex_num = " << this->vex_num;	
}
//图类； 
template <class T>
class OLGraph{																		//采用十字链表存储的有向图； 
	private:
		VexNode<T> *xlist;
		int arcnum, vexnum; 
		bool Is_existed(T head, T tail, int &h, int &t, bool &valid_vex);			//判断边表结点是否存在于图中； 
		bool FindArc(T head, T tail, ArcBox* &f_h, ArcBox* &f_t, ArcBox* &obj, int &h, int &t);	//从图中找到指定边结点； 
		inline void visit_arc(ArcBox *p);											//边表结点打印函数 ； 
		void DFS_vex(int n);														//深度优先递归地访问结点 ； 
		inline void visit_vex(int n){												//图顶点打印函数 ；
			cout << xlist[n].data << " ";
			xlist[n].mark = true;
		}
		inline void Clear_mark(){													//将边结点和顶点结点的mark置为false；
			for(int i = 0; i < vexnum; i++){
				xlist[i].mark = false;
				ArcBox *p = xlist[i].firstout;
				for(; p; p = p->tlink) p->mark = false;
			}
		}
	public:
		OLGraph(){xlist = nullptr; arcnum = vexnum = 0;}
		~OLGraph();
		bool CreatGraph(Test<T> &t);												//创建图； 
		bool InsertGraph(T head, T tail, int weight);								//在图中插入边表结点； 
		void show(); 																//打印图； 
		bool DeleteGraph(T head, T tail);											//从图中删除边表结点；
		void DFSearch();															//深度优先搜索遍历； 
		void BFSearch();															//广度优先搜索遍历；
//		void MiniSpanTree_KRUSKAL();												//最小生成树（克鲁斯卡尔算法）；
		void MiniSpanTree_PRIM();													//最小生成树（普里姆算法）；
		bool ShortestPath_DIJ(int n);												//最短路径算法（迪杰斯特拉算法）； 
		void ShortestPath_FLOYD();													//最短路径算法（弗洛伊德算法）；
};
template <class T>
OLGraph<T>::~OLGraph(){
	for(int i = 0; i < vexnum; i++){
		while(xlist[i].firstin){
			ArcBox *p = xlist[i].firstin;
			xlist[i].firstin = p->hlink;
			delete p;
		}
	}
	delete[] xlist;
}
template <class T>
bool OLGraph<T>::CreatGraph(Test<T> &t){											//使用Test结构类型数组创建图； 
	if(xlist) return false;
	this->xlist = new VexNode<T>[t.vex_num];
	this->vexnum = t.vex_num;
	for(int i = 0; i < vexnum; i++) xlist[i].data = t.vexlist[i];					//创建 xlist数组； 
	for(int i = 0; i < t.arc_num; i++){												//插入边表结点； 
		T head = t.arclist[i].headvex;
		T tail = t.arclist[i].tailvex;
		int weight = t.arclist[i].weight;
		InsertGraph(head, tail, weight);
	}
	return true;
}
template <class T>
bool OLGraph<T>::InsertGraph(T head, T tail, int weight){
	int h_arc = 0, t_arc = 0;
	bool valid_vex = false;
	bool Existed = Is_existed(head, tail, h_arc, t_arc, valid_vex);
	if (Existed || !valid_vex) return false;
	ArcBox *p = new ArcBox;
	p->info = weight;
	p->headvex = h_arc;
	p->hlink = xlist[h_arc].firstin;
	xlist[h_arc].firstin = p;
	p->tailvex = t_arc;
	p->tlink = xlist[t_arc].firstout;
	xlist[t_arc].firstout = p;
	(this->arcnum)++;
	return true;
}
template <class T>
bool OLGraph<T>::Is_existed(T head, T tail, int &h, int &t, bool &valid_vex){ 	
	int h_arc = 0, t_arc = 0;
	bool mark_h = false, mark_t = false;
	for(int j = 0; j < vexnum; j++){
		if(xlist[j].data == head) h_arc = j, mark_h = true;
		if(xlist[j].data == tail) t_arc = j, mark_t = true;
	} //找出边的头节点和尾结点在xlist数组的下标；
	if(mark_h && mark_t && (h_arc != t_arc)){
		valid_vex = true;
		h = h_arc, t = t_arc;
		ArcBox* p = xlist[h_arc].firstin;
		for(; p; p = p->hlink){
			if(p->tailvex == t_arc) return true;
		}
	}
	return false;
}
template <class T>
void OLGraph<T>::show(){
	for(int i = 0; i < vexnum; i++){
		cout << "[ " << xlist[i].data <<" ]";
		ArcBox *p = xlist[i].firstin;
		for(; p; p = p->hlink) visit_arc(p);
		cout << "\n";
		cout << "[ " << xlist[i].data <<" ]";
		p = xlist[i].firstout;
		for(; p; p = p->tlink) visit_arc(p);
		cout << "\n";
	}
	cout << "arcnum = " << arcnum << ", ";
	cout << "vexnum = " << vexnum << "\n";
}
template <class T>
bool OLGraph<T>::FindArc(T head, T tail, ArcBox* &f_h, ArcBox* &f_t, ArcBox* &obj, int &h, int &t){
	int h_arc = 0, t_arc = 0;
	bool valid_vex = false;
	bool Existed = Is_existed(head, tail, h_arc, t_arc, valid_vex);
	if (!Existed || !valid_vex) return false;
	ArcBox *p = xlist[h_arc].firstin;
	ArcBox *f = nullptr;
	while(p && p->tailvex != t_arc){
		f = p;
		p = p->hlink;
	}
	ArcBox *q = xlist[t_arc].firstout;
	ArcBox *r = nullptr;
	while(q && q->headvex != h_arc){
		r = q;
		q = q->tlink;
	}
	if(p && q && (p == q)){
		obj = p, f_h = f, f_t = r;
		h = h_arc, t = t_arc;
		return true;
	}else return false;
}
template <class T>
bool OLGraph<T>::DeleteGraph(T head, T tail){
	ArcBox *f_h, *f_t, *obj;
	f_h = f_t = obj = nullptr;
	int h_arc = 0, t_arc = 0;
	if(!FindArc(head, tail, f_h, f_t, obj, h_arc, t_arc)) return false;
	if(!f_h) xlist[h_arc].firstin = obj->hlink;
	else f_h->hlink = obj->hlink;
	if(!f_t) xlist[t_arc].firstout = obj->tlink;
	else f_t->tlink = obj->tlink;
	cout << "Delete element: ";
	this->visit_arc(obj);
	cout << "\n";
	delete obj;
	(this->arcnum)--;
	return true;
}
template <class T>
void OLGraph<T>::visit_arc(ArcBox *p){
	if(p){
		cout << "( ";
		cout << xlist[p->tailvex].data << " - ";
		cout << p->info << " -> ";
		cout << xlist[p->headvex].data << " )";
	}
}
template <class T>
void  OLGraph<T>::DFS_vex(int n){
	visit_vex(n);
	ArcBox *p = xlist[n].firstout;
	for(; p; p = p->tlink){
		if(!xlist[p->headvex].mark) DFS_vex(p->headvex);
	}
}
template <class T>
void OLGraph<T>::DFSearch(){
	for(int i = 0; i < vexnum; i++)
		if(!xlist[i].mark) DFS_vex(i);
}
template <class T>
void OLGraph<T>::BFSearch(){
	for(int i = 0; i < vexnum; i++){
		if(xlist[i].mark) continue;
		Queue<int> q;
		q.EnQueue(i);
		while(!q.Is_empty()){
			int n = 0;
			q.DeQueue(n);
			if(xlist[n].mark) continue;
			ArcBox *p = xlist[n].firstout;
			visit_vex(n);
			for(; p; p = p->tlink) q.EnQueue(p->headvex);
		}
	}
}
template <class T>
void OLGraph<T>::MiniSpanTree_PRIM(){
	Clear_mark();
	bool Is_Begin = true;
	int num = 0;
	while(true){
		ArcBox *min = nullptr;
		for(int i = 0; i < vexnum; i++){
			if(!Is_Begin && !xlist[i].mark) continue;
			ArcBox *p = xlist[i].firstout;
			for(; p; p = p->tlink){
				if(p->mark || xlist[p->headvex].mark) continue;
				if(!min || (p->info < min->info)) min = p;
			}
			p = xlist[i].firstin;
			for(; p; p = p->hlink){
				if(p->mark || xlist[p->tailvex].mark) continue;
				if(!min || (p->info < min->info)) min = p;
			}
		}
		Is_Begin = false;
		if(!min) break;
		xlist[min->tailvex].mark = xlist[min->headvex].mark = true;
		min->mark = true;
		num++;
		this->visit_arc(min); 
	}
	cout << "\nnum = " << num;
}
template <class T>
bool OLGraph<T>::ShortestPath_DIJ(int n){
	if(n > vexnum) return false;
	this->Clear_mark();
	ArcBox* path[vexnum];
	for(int i = 0; i < vexnum; i++) path[i] = nullptr;
	int cost[vexnum];
	for(int i = 0; i < vexnum; i++) cost[i] = INT_MAX;
	cost[n] = 0;
	bool Is_Finish = false;
	int min = n;
	while(!Is_Finish){
		xlist[min].mark = true;	
		ArcBox* p = xlist[min].firstout;
		for( ; p; p = p->tlink){
			if(cost[p->headvex] > cost[min] + p->info){
				path[p->headvex] = p;
				cost[p->headvex] = cost[min] + p->info;
			}
		}
		Is_Finish = true;
		min = 0;
		for(int i = 0; i < vexnum; i++){
			if(xlist[i].mark) continue;
			if(xlist[min].mark || cost[i] < cost[min]) min = i;	
		}
		if((!xlist[min].mark) && (cost[min] < INT_MAX)) Is_Finish = false;
	}
	for(int i = 0; i < vexnum; i++){
		cout << xlist[i].data << ": ";
		if(!path[i]){cout << "\n"; continue;}
		int j = i;
		Stack<int> s;
		while(j != n){s.push(j); j = path[j]->tailvex;}
		cout << xlist[j].data << " ";
		while(!s.Is_empty()){
			s.pop(j);
			cout << xlist[j].data << " ";
		}
		cout << "\n";
	}
	return true;
}
template <class T>
void OLGraph<T>::ShortestPath_FLOYD(){
	int cost[vexnum][vexnum];
	for(int i = 0; i < vexnum; i++)
		for(int j = 0; j < vexnum; j++){
			if(i == j) cost[i][j] = 0;
			else cost[i][j] = INT_MAX;
		}
	ArcBox* path[vexnum][vexnum];
	for(int i = 0; i < vexnum; i++)
		for(int j = 0; j < vexnum; j++) path[i][j] = nullptr;
	for(int i = 0; i < vexnum; i++){
		ArcBox *p = xlist[i].firstout;
		for(; p; p = p->tlink){												//记录各点间已有边； 
			cost[p->tailvex][p->headvex] = p->info;
			path[p->tailvex][p->headvex] = p;
		}
	}
	for(int n = 0; n < vexnum; n++){										//求各点间最短路径； 
		for(int i = 0; i < vexnum; i++){
			if(n == i || !path[i][n]) continue;
			for(int j = 0; j < vexnum; j++){
				if(i == j || !path[n][j]) continue;
				if(cost[i][j] > cost[i][n] + cost[n][j]){
					path[i][j] = path[n][j];
					cost[i][j] = cost[i][n] + cost[n][j];
				}
			}
		}
	}
	cout << "    ";
	for(int i = 0; i < vexnum; i++){
		cout << xlist[i].data << " ";
	}
	cout << "\n";
	for(int i = 0; i < vexnum; i++){
		cout << "[" << xlist[i].data << "] ";
		for(int j = 0; j < vexnum; j++){
			if(!path[i][j]){
				cout << "  "; continue;
			}
			cout << xlist[path[i][j]->tailvex].data << " ";
		}
		cout << "\n";
	}
	cout << "    ";
	for(int i = 0; i < vexnum; i++){
		cout.width(2);
		cout << xlist[i].data << " ";
	}
	cout << "\n";
	for(int i = 0; i < vexnum; i++){
		cout << "[" << xlist[i].data << "] ";
		for(int j = 0; j < vexnum; j++){
			if(cost[i][j] == INT_MAX){
				cout << "__ "; continue;
			}
			cout.width(2);
			cout << cost[i][j] << " ";
		}
		cout << "\n";
	}
}
#endif
