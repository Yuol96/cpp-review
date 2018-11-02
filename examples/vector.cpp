#include <iostream>
#include <vector>

using namespace std;

template<typename T>
void printer(const T& val){
	cout << val << " ";
}

void print_vector(const vector<int> & v){
	// for(int i=0;i<v.size();i++){
	// 	cout << v.at(i) << " ";
	// }
	// for(vector<int>::const_iterator iter=v.cbegin(); iter!=v.cend(); iter++){
	// 	cout << *iter << " ";
	// }
	// for(auto iter=v.cbegin(); iter!=v.cend(); iter++){
	// 	cout << *iter << " ";
	// }
	// for_each(v.cbegin(),v.cend(),printer<int>);
	for(auto num : v){
		cout << num << " ";
	}
	cout << endl;
}

void test_vector(){
	vector<int> v1(5,42);
	print_vector(v1);
	vector<int> v2 = {0,1,2,3,4,5};
	print_vector(v2);
}

int main(int argc, char* argv[]){
	test_vector();
	return 0;
}