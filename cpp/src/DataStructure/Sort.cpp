#ifndef SORT_H
#define SORT_H

#include <ctime>
using namespace std;
template <class T>
struct SqListNode{
	int key = 0;
	T data;
};
template <class T>
class Sort{
	private:
		int length;
		SqListNode<T> *List;
		
		void ShellInsert(int dk);					//����Ϊdk��һ��ϣ���������� 
		void HeapAdjust(int len);					//������ǰ��Ϊ�󶥶ѣ� 
		int Partition(int l, int h);				//һ�ֿ������򣬺������������±ꣻ
		void Merge(int L, int mid, int R);			//һ�ֹ鲢����
		inline void Change(SqListNode<T> &a, SqListNode<T> &b){
			SqListNode<T> temp = a;					//����List[]�������Ԫ�أ� 
			a = b;
			b = temp;
		}
		
	public:
		Sort(){length = 0, List = nullptr;}
		~Sort(){if(List) delete[] List;}
		bool CreatList_auto(int len, int key_range);
		void show();
		bool IsOrdered();							//�жϵ�ǰ�����Ƿ����� �� 
		int  BinarySearch(int e, int head, int end);//�۰���ң�
		void BInsertSort();							//�۰��������
		void ShellSort();							//ϣ������
		void SelectSort();							//��ѡ������
		void HeapSort();							//������
		void BubbleSort();							//ð������
		void QuickSort(int low, int high);			//��������
		void MergeSort(int low, int high);			//�鲢����
//		void RadixSort();							//��������
};
template <class T>
bool Sort<T>::CreatList_auto(int len, int key_range){
	if(len < 1) return false;
	length = len;
	if(!(List = new SqListNode<T>[length + 1])) return false;
	srand((int)(time(0)));
	for(int i = 1; i <= length; i++){
		List[i].key = rand()%key_range;
	}
	return true;
}
template <class T>
void Sort<T>::show(){
	cout << "length = " << length << "\n";
	cout << "List   = ";
	for(int i = 1; i <= length; i++) cout << List[i].key << " ";
	cout << "\n";
}
template <class T>
int Sort<T>::BinarySearch(int e, int head, int end){
	int low = head, high = end;
	int mid = (low + high)/2;
	while(low <= high){ 							//ѭ������ʱ�������ҵ�������һ��ֻʣһ��Ԫ�� ����low == high == mid�� 
		if(e >= List[mid].key) low = mid + 1;
		else                 high = mid - 1;
		mid = (low + high)/2;
	}
	return mid;
}
template <class T>
bool Sort<T>::IsOrdered(){
	bool Is_ordered = true;
	int i;
	for(i = 1; i < length; i++)
		if(List[i].key > List[i + 1].key){
			Is_ordered = false;
			break;
		}
	if(Is_ordered) cout << "Yes, it's in order!\n";
	else		   cout << "List[" << i << "]: No, it's disordered!\n";
	return Is_ordered;
}
template <class T>
void Sort<T>::BubbleSort(){
	for(int i = 1; i < length; i++){
		bool Is_ordered = true;
		for(int j = 1; j < length; j++){
			if(List[j].key > List[j + 1].key){
				Change(List[j],List[j + 1]);
				Is_ordered = false;
			}
		}
		if(Is_ordered) break;
	}
}
template <class T>
void Sort<T>::BInsertSort(){
	for(int i = 2; i <= length; i++){
		List[0] = List[i];
		int n = BinarySearch(List[0].key, 1, i-1);
		for(int j = i - 1; j > n; j--) List[j + 1] = List[j];
		List[n + 1] = List[0];
	}
}
template <class T>
void Sort<T>::ShellSort(){
	int dk[] = {1,2,3,5,7,11,13};
	int len = sizeof(dk)/sizeof(dk[0]);
	for(int i = len - 1; i >= 0; i--) ShellInsert(dk[i]);
}
template <class T>
void Sort<T>::ShellInsert(int dk){
	for(int i = 1; i < dk + 1; i++){
		for(int j = i + dk; j <= length; j += dk){
			int l;
			List[0] = List[j];
			for(l = j - dk; l > 0; l -= dk){
				if(List[l].key <= List[0].key) break;
				else List[l + dk] = List[l];
			}
			List[l + dk] = List[0];
		}
	}
}
template <class T>
void Sort<T>::SelectSort(){
	for(int i = 1; i <= length; i++){
		int min = i;
		for(int j = i; j <= length; j++){
			if(List[j].key < List[min].key) min = j;
		}
		Change(List[i],List[min]);
	}
}
template <class T>
void Sort<T>::HeapSort(){
	for(int i = length; i > 1; i--){
		this->HeapAdjust(i);
		Change(List[1],List[i]);
	}
}
template <class T>
void Sort<T>::HeapAdjust(int len){
	for(int i = len/2; i > 0; i--){
		int j = i;
		while(true){								//������List[i]Ϊ�Ѷ��Ĵ󶥶ѣ� 
			if((j*2 + 1) <= len){ 					//1.�������������Һ��ӣ� 
				int max = j*2 +1;
				if(List[max].key < List[j*2].key) max = j*2;
				if(List[j].key < List[max].key){
					Change(List[max],List[j]);
					j = max;
				}else break;
			}
			else if(j*2 <= len){					//2.ֻ�����ӣ� 
				int max = j*2;
				if(List[j].key < List[max].key){
					Change(List[max],List[j]);
				}
				break;
			}
			else break;								//3.Ҷ�ӽڵ㣻 
		}
	}
}
template <class T>
int Sort<T>::Partition(int l, int h){
	int low = l, high = h;
	SqListNode<T> pivotkey = List[low];
	while(low < high){
		while(low < high && List[high].key >= pivotkey.key) high--;
		List[low] = List[high];
		while(low < high && List[low].key <= pivotkey.key)   low++;
		List[high] = List[low];
	}
	List[low] = pivotkey;
	return low;
}
template <class T>
void Sort<T>::QuickSort(int low, int high){
	if(low < high){
		int mid = Partition(low, high);
		QuickSort(low, mid - 1);
		QuickSort(mid + 1, high);
	}	
}
template <class T>
void Sort<T>::Merge(int L, int mid, int R){
	SqListNode<T> *arr = new SqListNode<T>[R - L + 1];
	for(int i = L; i <= R; i++) arr[i - L] = List[i];
	int a = 0, b = mid - L + 1;
	int i = L;
	while(a <= mid - L && b <= R - L){
		List[i++] = (arr[a].key <= arr[b].key)? arr[a++]: arr[b++];
	}
	while(a <= mid - L) List[i++] = arr[a++];
	while(b <= R - L)   List[i++] = arr[b++];
	delete[] arr;
}
template <class T>
void Sort<T>::MergeSort(int low, int high){
	if(low < high){
		int mid = (low + high)/2;
		MergeSort(low, mid);
		MergeSort(mid + 1, high);
		Merge(low, mid, high);
	}
}
#endif
