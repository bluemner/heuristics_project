int heuristic_function(__constant int size, __constant int * goal, int *matrix){
	int result=0;	
	for( int i=0; i < size * size; i++){
		result += abs_diff( goal[i] , matrix[i]);
	}
	return result;
}
void rotate_up(__constant int size,int col, int value, int *matrix, int *result){
	for( int i = 0; i < size; i++ ){
		shift = (i + value) % size;
		result[size * i + col]=matrix[ size * shift + col];
	}
}
void rotate_down(__constant int size,int col, int value, int *matrix, int *result){
	for( int i = 0; i < size; i++ ){
		shift = (i-value <0)? i - value + size: (i-value);
		result[i * size + col]=matrix[size * shift +col];
	}
}
void rotate_left(__constant int size,int row, int value , int *matrix,  int *result){
	int shift;
	for( int i = 0; i < size; i++ ){
		shift = (i + value) % size;
		result[row * size + i ]=matrix[ row * size + shift ];
	}
}
void rotate_right(__constant int size,int row, int value,int *matrix, int *result){
	int shift;
	for( int i = 0; i <  size; i++ ){
		shift = (i-value <0)? i-value+ size : (i-value);
		result[row*size + i]=matrix[row * size + shift];
	}
}

void next_nodes(__constant int size int *current ,__constant int * goal)
{
	
}
__kernel void search(__constant int size, __constant int * source, __constant int * target)
{
	//Get our global thread ID
	int id = get_global_id(0);

}