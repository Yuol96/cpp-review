#include <iostream>
#include <vector>

template <class elem_t>
class Matrix{
typedef std::vector<elem_t> row_t;
typedef std::vector<row_t> mat_t;

private:
	mat_t mat;
	int nRow, nCol;

public:
	Matrix(int k);
	void print_mat();
};

template <class elem_t>
Matrix<elem_t>::Matrix(int k): nRow(k), nCol(k){
	for(int i=0; i<k; i++){
		row_t row(k, 0);
		for(int j=0; j<k; j++){
			if(i==j) row[j] = 1;
		}
		mat.push_back(row);
	}
}

template <class elem_t>
void Matrix<elem_t>::print_mat(){
	for(int i=0; i<nRow; i++){
		for (int j=0;j<nCol;j++){
			std::cout << mat[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

int main(int argc, char* argv[]){
	Matrix<int> m(5);
	m.print_mat();
}