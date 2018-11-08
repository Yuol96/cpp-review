#include <iostream>
#include <vector>

using namespace std;

template<typename T>
void printer(const T& val){
	std::cout << val << " ";
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
		std::cout << num << " ";
	}
	std::cout << endl;
}

void test_vector(){
	vector<int> v1(5,42);
	print_vector(v1);
	vector<int> v2 = {0,1,2,3,4,5};
	print_vector(v2);

	v1.push_back(300);
	for(int i=20; i<30; i+=2) v1.push_back(i);
	print_vector(v1);

	auto iter = std::find(v1.cbegin(),v1.cend(),300);
	std::cout << "Find: " << *iter << endl;

	v1.pop_back();
	print_vector(v1);

	for(auto iter=v1.cbegin(); iter != v1.cend(); iter++){
		if(*iter == 300){
			v1.erase(iter);
			break;
		}
	}
	print_vector(v1);

	std::sort(v1.begin(), v1.end());
	print_vector(v1);

	std::cout << v1.size() << endl;

	v1.insert(v1.begin()+1, 100);
	print_vector(v1);

	v1.insert(v1.end()-1, 3, 3000);
	print_vector(v1);
}

int main(int argc, char* argv[]){
	test_vector();
	return 0;
}