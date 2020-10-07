//哈夫曼树类； 
#ifndef HUFFMANTRE_H
#define HUFFMANTRE_H

#include <limits.h>

using namespace std;

struct HTNode{
	int weight = 0;
	int parent = 0, lchild = 0, rchild = 0;
};

class HuffmanTree{
	private:
		HTNode *HT;
		int length, head;
	public:
		HuffmanTree(){HT = nullptr; length = head = 0;}
		~HuffmanTree(){if(HT) delete [] HT;}
		void Creat_HTree(int d[], int len);
		void Traverse_HTree();
};

void HuffmanTree::Creat_HTree(int d[], int len){
	length = len;
	HT = new HTNode[2*length];
	int begin = 1, end = length;
	for(int i = begin; i <= end; i++) HT[i].weight = d[i-1];			//第0号元素空出；
	while(true){
		int point[2] = {0};
		for(int j = 0; j <= 1; j++){									//每次找出权值最小且无双亲的2个结点； 
			int min = INT_MAX, a = 0;
			for(int i = begin; i <= end; i++){
				if((HT[i].weight < min) && (HT[i].parent == 0)){
					min = HT[i].weight;
					a = i;
				}
			}
			point[j] = a;
			HT[a].parent = end + 1;
		}
		if(point[0] != 0 && point[1] != 0){
			end = end + 1;
			HT[end].lchild = point[0];
			HT[end].rchild = point[1];
			HT[end].weight = HT[point[0]].weight + HT[point[1]].weight;
		}else{																//head指向哈夫曼树根结点 
			HT[point[0]].parent = HT[point[1]].parent = 0;
			if(point[0] != 0) head = point[0];
			else 			  head = point[1];
			break;
		}
	}	
}
bool BubbleSort(int d[], int len){
	if(len <= 0) return false;
	for(int i = 0; i < len; i++){
		bool is_ordered = true;
		for(int j = len-1; j > i; j--){
			if(d[j] < d[j-1]){
				int temp = d[j];
				d[j] = d[j-1];
				d[j-1] = temp;
				is_ordered = false;
			}
		}
		if(is_ordered) break; 
	}
	return true;	
} 

void HuffmanTree::Traverse_HTree(){
	for(int i = 0; i < (2*length); i++){
		cout << "HT[";
		cout.width(2);
		cout << i << "] = ";
		cout.width(3);
		cout << HT[i].weight << "(";
		cout.width(2);
		cout << HT[i].lchild << ", ";
		cout.width(2);
		cout << HT[i].parent << ", ";
		cout.width(2);
		cout << HT[i].rchild << ")\n";
		if(i == length) cout << "**************************\n";
	}
}
#endif
