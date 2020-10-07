#include <iostream>
#include <string>
using namespace std;



void Get_Nextval(int *next, string T){					//next�����󷨣���֪next[i] = k���� next[i+1]�� 
	next[1] = 0;
	for(int i = 1; i < T.size(); i++){	
		int k = next[i];								// T[i] == T[k]ʱ��next[i+1] = k+1; 
		while(k > 0 && T[i] != T[k]) k = next[k];		// T[i] != T[k]ʱ��k������ǰ���ݣ�ֱ��T[k] == T[i]; 
		next[i+1] = k+1;								//���next[i+1]�� 
		
	}
	for(int i = 2; i <= T.size(); i++)					//��next��������Ż���ʹT[i] != T[k]�� 
		while(T[i] == T[next[i]]) next[i] = next[next[i]];	
}

int Index_KMP(string S, string T, int pos, int *next){
	int i = pos, j = 1;
	while(i < S.size() && j < T.size()){
		if(S[i] == T[j]) i++, j++;
		else j = next[j];
		if(j == 0) i++, j++;
	}
	if(j >= T.size()) return (i - (T.size()-1));
	else return 0;
}


int main(){
	string T = "0ncjnakak";
	string S = "0jufaaaabguiaaaabncjnakakaabachunkiaaaabhiv";
	const int len = T.size();
	static int next[6];
	
	int pos = Index_KMP(S, T, 1, next);
	for(int i = 1; i < S.size(); i++){
		if(i == pos) cout << "[";
		cout << S[i];
		if(i == pos + T.size()-2) cout << "]";
	}
	
	return 0;
} 
