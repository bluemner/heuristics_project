#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <map>
#include <set>
#include <tuple>
#include <functional>
#include <queue>
#include <cassert>
std::set<long long int> visited;
void print(std::vector<std::vector<int>> &matrix);
void dijkstra (
				std::vector<std::vector<int>> &start,
				std::vector<std::vector<int>> &goal,
				std::map<long long int, long long int> &path,
				std::map<long long int, long> &cost,
				std::map< long long int, std::vector<std::vector<int>> > &look_up
			);

// Given n x n matrix create a unique hash
long long int get_hash_value(std::vector<std::vector<int>> &matrix){
	//start at zero zero
	long long int result =0; // (long long int) ( 0+1 * std::pow(10,(matrix.size()-1)*2) ) + (long long int)  (0+1 * std::pow(10,matrix.size()-1))  + matrix[0][0];
		// Row
	for(int i=0; i< matrix.size();i++){
		// Col
		for(int j=0; j<matrix[i].size(); j++){
			//if(i==0 && j ==0) continue;
			// xx xx xxxx
			// row + col + number
			auto row = (i+1)  * std::pow(10, (matrix.size()-1 ) *2);
			auto col = (j+1) *  std::pow(10,matrix.size()-1);
			result ^= (long long int) (row) + (long long int)  (col )  * matrix[i][j];
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
		for(int j=0; j< (current.size()-1); j++){
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
		}// for - j
	}// for - i
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
		// {4,3},
		// {2,1}
	};

	std::vector<std::vector<int>> goal=
	{
		{1,2,3},
		{4,5,6},
		{7,8,9}
		// {1,2},
		// {3,4}

	};
	std::cout<< "Start Hash:" << get_hash_value(start) <<std::endl;
	std::cout<< "Goal Hash:" << get_hash_value(goal) <<std::endl;
	long long int goal_hash = get_hash_value(goal);
	long long int start_hash =get_hash_value(start);


	std::vector<std::vector<std::vector<int>>> result;


	shift_row(0,2,start,result);

	std::cout<<"============== ROW ============="<<std::endl;
	print(result[0]);
	std::cout<< "R[0] Hash:" << get_hash_value(result[0]) <<std::endl;
	std::cout<<std::endl;

	print(result[1]);
	std::cout<< "R[1] Hash:" << get_hash_value(result[1]) <<std::endl;
	result.empty();
	std::cout<<"============= COLUMN ==========="<<std::endl;
	shift_col(0,2,start,result);

	print(result[0]);
	std::cout<<std::endl;
	print(result[1]);

	std::cout<< "------------------------------" <<std::endl;
	shift_col(1,2,start,result);

	print(result[0]);
	std::cout<<std::endl;
	print(result[1]);

	std::cout<< "------------------------------" <<std::endl;
	shift_col(2,2,start,result);

	print(result[0]);
	std::cout<<std::endl;
	print(result[1]);
	std::cout<<"============= Next  ==========="<<std::endl;
	
	std::map<std::vector<std::vector<int>>, long> _next;
	get_next_node(start,goal,_next);
	for(auto n: _next){
		auto a = n.first;
		auto b = n.second;
		std::cout <<"---------------------------------" << std::endl;
		std::cout <<"Cost:"<< b<< std::endl;
		print(a);
		std::cout <<"---------------------------------" << std::endl;
	}
	//source target
	std::map<long long int, long long int>path;
	std::map<long long int, long> cost;
	std::map< long long int, std::vector<std::vector<int>> > look_up;
	// while( _next.size() > 0 ){
	// 	get_min(_next, current_node);
	// 	_next.erase(current_node);
	// 	get_next_node(current_node,goal,_next);
	// 	auto current_hash =	 get_hash_value(current_node);
	// 	if(visited.find(current_hash) != visited.end()){
	// 		if(current_hash !=previous_hash && ( path.find(current_hash) == path.end() || cost[current_hash]<= _next[current_node]) ){
	// 			path[current_hash] = previous_hash ;
	// 			cost[current_hash] = _next[current_node];
	// 		}
	// 		_next.erase(current_node);
	// 		continue;
	// 	}else{
	// 		look_up[current_hash] = current_node;
	// 		visited.insert(current_hash);
	// 	}

	// 	if( cost.find(current_hash) == cost.end() ){
	// 		cost[current_hash] = _next[current_node];
	// 	}
	// 	if(current_hash !=previous_hash && ( path.find(current_hash) == path.end() || cost[current_hash]<= _next[current_node]) ){
	// 		path[current_hash] = previous_hash;
	// 		cost[current_hash] = _next[current_node];
	// 	}

	// 	if( current_hash == goal_hash ){
	// 		path[current_hash] = previous_hash ;
	// 		std::cout << "Found!!" <<"\nVisited count:"<< visited.size() << std::endl;
	// 		break;
	// 	}
	// 	if( _next.size() == 0 ){
	// 		std::cout << "Fail"<< std::endl;
	// 	}
	// 	previous_hash =current_hash;
	// }
	std::cout<<"============= Algorithm  ==========="<<std::endl;
	dijkstra(start,goal,path,cost, look_up);
	// for(auto n : path){
	// 		std::cout << n.first <<"<-" << n.second << std::endl;
	// }

	long long int cur= goal_hash;
	// for(auto n : look_up){
	// 	std::cout <<"---------------------------------" << std::endl;
	// 	print(n.second);
	// 	std::cout <<"---------------------------------" << std::endl;
	// }
	print(goal);
	while(cur != start_hash || cur !=0){
		std::cout  << "Cost:"<< cost[cur]<< std::endl;
		std::cout << cur <<"->";
		cur = path[cur];
		std::cout << cur <<std::endl;

		if(cur == 0 || cur ==start_hash || cost[cur] ==0 ){
			std::cout<<"Problem with lookup" << std::endl;
			break;
		}
			
		print(look_up[cur]);
	}
	print(start);
	return 0;
}
typedef std::pair<long long int, int> QueueItem;
//
void dijkstra ( std::vector<std::vector<int>> &start,
				std::vector<std::vector<int>> &goal,
				std::map<long long int, long long int> &path,
				std::map<long long int, long> &cost,
				std::map< long long int, std::vector<std::vector<int>> > &look_up)
{
	visited.clear();

	long long int goal_hash = get_hash_value(goal);
	long long int start_hash =get_hash_value(start);

	visited.insert(start_hash);
	look_up[start_hash] = start;


	std::map<std::vector<std::vector<int>>, long> _next;
	std::priority_queue <QueueItem, std::vector<QueueItem>, std::greater<QueueItem>  > frontier;
	frontier.push( QueueItem(start_hash, heuristic_function(start, goal)));
	std::vector<std::vector<int>> current_node = start;
	std::vector<std::vector<int>> prev_node;
	long long int current_hash;
	long long int previous_hash=get_hash_value(start);

	look_up[previous_hash]=start;

	while( frontier.size() > 0 )
	{
		
		current_node= look_up[frontier.top().first];
		current_hash =	frontier.top().first;
		frontier.pop();
	
		auto curret_cost = cost[current_hash];
		
		if( current_hash == goal_hash )
		{
			std::cout << "Found!!" <<"\nVisited count:"<< visited.size() << std::endl;
			break;
		}
		_next.clear();

		// Get neighbors
		get_next_node(current_node,goal,_next);
	

		auto previous_cost = cost[current_hash];

		for(auto nxt : _next)
		{
			auto neighbors_node = nxt.first;
			auto neighbors_hash = get_hash_value(neighbors_node);
			auto neighbors_cost = _next[neighbors_node];
			
			if( visited.find(neighbors_hash) != visited.end() ){
				if( curret_cost >= neighbors_cost ){
					path[neighbors_hash] =current_hash;
					cost[neighbors_hash] = _next[current_node];
				}
			}else{
				path[neighbors_hash] = current_hash;
				cost[neighbors_hash] = neighbors_cost;
				
				if(neighbors_cost == 0){
					std::cout<< "Probelm with ::" << neighbors_hash << std::endl;
					continue;
				}
				look_up[neighbors_hash] = neighbors_node;
				visited.insert(neighbors_hash);
				auto item = QueueItem(neighbors_hash, heuristic_function(start, goal));
				frontier.push(item);
			}
		}// For loop

		// if( frontier.size() == 0 ){
		// 	std::cout << "Fail"<< std::endl;
		// 	break;
		// }

		previous_hash =current_hash;
		prev_node = current_node;
	}
}