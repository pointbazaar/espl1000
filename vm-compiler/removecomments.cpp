#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> remove_comments(vector<string> lines){
	vector<string> result;
	
	for(int i=0;i<lines.size();i++){
		string s = lines.at(i);
		
		int pos=s.find("//");
		if(pos==-1){
			result.push_back(s);
		}else{
			result.push_back(s.substr(0,pos));
		}
	}
	
	return result;
}


