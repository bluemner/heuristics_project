void heuristic_function(const int size, __global int * goal, __global int * matrix){
	int result=0;	
	for( int i=0; i < size * size; i++){
		result += abs_diff( goal[i] , matrix[i]);
	}
	//return result;
}
void rotate_up(const int size,const int col, const int value,const int start, __global const int *matrix, __global int *result){
	int shift;
	for( int i = 0; i < size; i++ ){
		shift = (i + value) % size;
		result[start+(size * i + col)]=matrix[ size * shift + col];
	}
}
void rotate_down(const int size,const int col,const int value,const int start,__global int *matrix,__global int *result){
	int shift;
	for( int i = 0; i < size; i++ ){
		shift = (i-value <0)? i - value + size: (i-value);
		result[start+ (i * size + col)]=matrix[size * shift +col];
	}
}
void rotate_left(const int size,const int row,const int value,const int start ,__global int *matrix,__global  int *result){
	int shift;
	for( int i = 0; i < size; i++ ){
		shift = (i + value) % size;
		result[start+ (row * size + i) ]=matrix[ row * size + shift ];
	}
}
void rotate_right(const int size,const int row,const int value,const int start,__global int *matrix,__global int *result){
	int shift;
	for(int i = 0; i <  size; i++ ){
		shift = (i-value <0)? i-value+ size : (i-value);
		result[start+(row*size + i)]=matrix[row * size + shift];
	}
}
// result  = [up,down,left,right][nxn]
__kernel void next_nodes(const int size,
						 __constant int * goal,
						 __global int *current,
						 __global int *result,
						 __global int *cost
						)
{
	//[0....7] n =2
	//int i  = id / size;
	//int j  = id % size;
	//x + y * sizeX + z * sizeX * sizeY
	int id = get_global_id(0);
	int n = size * size;
	int start = n * id ;
	int i = id % size;
	int j = id / 4;
	//result[id]=j*10;
	cost[id]= j;
	for(int k=0; k< n; k++){
		result[start+k] = current[k];
	}
	switch(id % 4){
		case 0:
			rotate_up	(size,i,j,start,current, result);
			break;
		case 1:
			 rotate_down (size,i,j,start,current, result );
			break;
		case 2:
			rotate_left (size,i,j,start,current, result );
			break;
		case 3:
			 rotate_right(size,i,j,start,current, result );
			break;
	}


}
__kernel void search(const int size, __constant int * source, __constant int * target)
{
	//Get our global thread ID  
	int id = get_global_id(0);
	
}