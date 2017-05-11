#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <map>
void successor(){
}

void get_position(int n, std::vector<std::vector<int>> &matrix, int * result){
	for(int i=0; i<matrix.size(); i++ ){
		for(int j=0; j< matrix.at(i).size(); j++ )
		{
			if(matrix[i][j] == n){
				result[0]=i;
				result[1]=j;
				return;
			}
		}
	}
}

int heuristic_function(std::vector<std::vector<int>> &current,
					   std::vector<std::vector<int>> &goal )
{
	int cost = 0;//std::numeric_limits<int>::max();
	int temp[2]={0,0};
	for(int i=0; i < current.size(); i++ ){
		for(int j=0; j< current.at(i).size(); j++ ){
			get_position(current[i][j], goal, temp);
			int dx = temp[0]-i;
			int dy = temp[1]-j;
			cost += std::abs(dx) +  std::abs(dy);
		}
	}
	return cost;
}

void shift_col(int col, int n,
			   std::vector<std::vector<int>> &matrix,
			   std::vector<std::vector<int>> &result)
{

	int shift = 0;
	result = std::vector<std::vector<std::vector<int>>>(2);
	result[0] = matrix;
	result[1] = matrix;
	//left
	for( int i = 0; i < matrix.size(); i++ ){ 
		shift = (i + n) % matrix.size();
		result[0][row][i]=matrix[row][shift];
	}
	//right
	for( int i = 0; i < matrix.size(); i++ ){ 
		shift = (i-n <0)? i-n+ matrix.size(): (i-n);
		result[1][row][i]=matrix[row][shift];
	}

}
void shift_row(int row, int n,
			   std::vector<std::vector<int>> &matrix,
			   std::vector<std::vector<std::vector<int>>> &result)
{
	int shift = 0;
	result = std::vector<std::vector<std::vector<int>>>(2);
	result[0] = matrix;
	result[1] = matrix;
	//left
	for( int i = 0; i < matrix[row].size(); i++ ){ 
		shift = (i + n) % matrix[row].size();
		result[0][row][i]=matrix[row][shift];
	}
	//right
	for( int i = 0; i < matrix[row].size(); i++ ){ 
		shift = (i-n <0)? i-n+ matrix[row].size(): (i-n);
		result[1][row][i]=matrix[row][shift];
	}
}

void next(std::vector<std::vector<int>> &current, 
		  std::map<std::vector<std::vector<int>>, long> &next)
{

}

void start(int size, std::vector<std::vector<int>> &result)
{

}
void goal(int size, std::vector<std::vector<int>> &result)
{
	
}

void print(std::vector<std::vector<int>> &matrix){
	for(int i = 0 ; i < matrix.size(); i++){
		for(int j = 0 ; j < matrix[i].size(); j++){
			std::cout << matrix[i][j] <<"\t";
		}
		std::cout << std::endl;
	}
}
//
//
//
int main(int argc, char* argv[])
{
	std::vector<std::vector<int>> start=
	{
		{9,8,7},
		{6,5,4},
		{3,2,1}
	};

	std::vector<std::vector<int>> goal=
	{
		{1,2,3},
		{4,5,6},
		{7,8,9}
	};

	int h = heuristic_function(start, goal);
	std::cout<< "H:" << h <<std::endl;

	std::vector<std::vector<std::vector<int>>> result;
	shift_row(0,1,start,result);
	
	print(result[0]);
	std::cout<<std::endl;
	print(result[1]);
	
	return 0;
}