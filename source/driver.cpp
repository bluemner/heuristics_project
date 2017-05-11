#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <map>
#include <set>
#include <tuple>
std::set<long long int> visited;
void print(std::vector<std::vector<int>> &matrix);
// Given n x n matrix create a unique hash 
long long int get_hash_value(std::vector<std::vector<int>> &matrix){
	//start at zero zero
	long long int result = matrix[0][0];
		// Row
	for(int i=0; i< matrix.size();i++){
		// Col
		for(int j=0; j<matrix[i].size(); j++){
			result ^= (i+1) * (j+1)  * matrix[i][j];
		}			
	}

	return result;
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
			    std::vector<std::vector<std::vector<int>>> &result)
{

	int shift = 0;
	result = std::vector<std::vector<std::vector<int>>>(2);
	result[0] = matrix;
	result[1] = matrix;
	//Up
	for( int i = 0; i < matrix.size(); i++ ){ 
		shift = (i + n) % matrix.size();
		result[0][i][col]=matrix[shift][col];
	}
	//Down
	for( int i = 0; i < matrix.size(); i++ ){ 
		shift = (i-n <0)? i-n+ matrix.size(): (i-n);
		result[1][i][col]=matrix[shift][col];
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

void get_next_node(std::vector<std::vector<int>> &current,std::vector<std::vector<int>> &goal, 
		  std::map<std::vector<std::vector<int>>, long> &next)
{

	std::vector<std::vector<std::vector<int>>> result;
	std::map<long long int, int> check;
	// Row
	for(int i=0; i< current.size();i++){
		// Col
		for(int j=0; j<current.size(); j++){
			result.empty();
			shift_row(i,j,current, result);
			for(int k =0; k<result.size(); k++){
				int heuristic =	heuristic_function(result[k],goal);
				long long int hash = get_hash_value(result[k]);
				if(visited.find(hash) != visited.end()  )
				{
					continue;
				}else
				if(check.find(hash) ==  check.end() || check[hash]> heuristic + j){
					check[hash] = heuristic +j;
					next[result[k]]=  heuristic +j;
				}
			}// For -k
			
			result.empty();
			shift_col(i,j,current, result);
			for(int k =0; k<result.size(); k++){
				int heuristic =	heuristic_function(result[k],goal);
				long long int hash = get_hash_value(result[k]);
				if(visited.find(hash) != visited.end()  )
				{
					continue;
				}else
				if(check.find(hash) ==  check.end() || check[hash]> heuristic + j){
					check[hash] = heuristic +j;
					next[result[k]]=  heuristic +j;
				}
			}// For -k
			
		

		}
	}
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
void get_min(std::map<std::vector<std::vector<int>>, long> _next,
			 std::vector<std::vector<int>> &result){
	long min = std::numeric_limits<long>::max();
	std::vector<std::vector<int>> temp;
	for(auto np : _next)
	{
		if(  np.second < min){
			temp = np.first;
			min = np.second;
		}
	}
	result = temp;
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
	std::cout<< "Start Hash:" << get_hash_value(start) <<std::endl;
	std::cout<< "Goal Hash:" << get_hash_value(goal) <<std::endl;
	long long int goal_hash = get_hash_value(goal);
	long long int start_hash =get_hash_value(start);
	int h = heuristic_function(start, goal);
	std::cout<< "H:" << h <<std::endl;

	std::vector<std::vector<std::vector<int>>> result;
	
	shift_row(0,1,start,result);

	std::cout<<"============== ROW ============="<<std::endl;
	print(result[0]);
	std::cout<< "R[0] Hash:" << get_hash_value(result[0]) <<std::endl;
	std::cout<<std::endl;

	print(result[1]);
	std::cout<< "R[1] Hash:" << get_hash_value(result[1]) <<std::endl;
	result.empty();
	std::cout<<"============= COLUMN ==========="<<std::endl;
	shift_col(0,1,start,result);

	print(result[0]);
	std::cout<<std::endl;
	print(result[1]);
	std::map<std::vector<std::vector<int>>, long> _next;
	
	get_next_node(start,goal,_next);

	std::vector<std::vector<int>> current_node = start;
	long long int previous_hash=get_hash_value(start);
	std::map< long long int, std::vector<std::vector<int>> > look_up;
	look_up[previous_hash]=start;
	//source target
	std::map<long long int, long long int>path;
	std::map<long long int, long> cost;
	while(_next.size()>0){
		get_min(_next, current_node);
		_next.erase(current_node);
		get_next_node(current_node,goal,_next);
		auto current_hash =	 get_hash_value(current_node);
		if(visited.find(current_hash) != visited.end()){
			if(current_hash !=previous_hash && ( path.find(current_hash) == path.end() || cost[current_hash]<= _next[current_node]) ){
				path[current_hash] = previous_hash ;
				cost[current_hash] = _next[current_node];
			}	
			_next.erase(current_node);		
			continue;
		}else{
			look_up[current_hash] = current_node;
			visited.insert(current_hash);
		}
		if( cost.find(current_hash) == cost.end() ){
			cost[current_hash] = _next[current_node];
		}
		if(current_hash !=previous_hash && ( path.find(current_hash) == path.end() || cost[current_hash]<= _next[current_node]) ){
			path[current_hash] = previous_hash ;
			cost[current_hash] = _next[current_node];
		}	
	
		if(current_hash == goal_hash ){
			path[current_hash] = previous_hash ;
			std::cout << "Found!!" <<"\nVisited count:"<< visited.size() << std::endl;
			
			break;
		}
		if(_next.size() ==0){
			std::cout << "Fail"<< std::endl;
		}
		previous_hash =current_hash;
	}
	long long int cur= goal_hash;
	while(cur != start_hash || cur !=0){
		std::cout << cur <<"->";
		cur = path[cur];		
		std::cout << cur << "\tCost:"<< cost[cur]<< std::endl;
		if(cur == 0 || cur ==start_hash)
			break;
		//print(look_up[cur]);
	}
	// for(auto  n : path){
	// 	std::cout << n.first <<"->" << n.second << std::endl;
	// }
	// std::cout<< "Unique states from start:"<< _next.size() <<std::endl;
	// for(auto np : _next)
	// {

	// 	std::cout<<"H:"<< np.second <<std::endl;
	// 	auto temp = np.first;
	// 	print(temp);
	// }
	return 0;
}