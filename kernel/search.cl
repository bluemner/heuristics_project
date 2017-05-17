// Open cl - kernel 
// 
#define PQ_START	0
#define PQ_END		1
#define PQ_MAX		2

// Queue item with item and value  
typedef struct queue_item_struct{
	int id;
	int value;
} queue_item_struct;

// inserts element basied on priority 
void pq_insert( const int value, const int id ,__global  queue_item_struct * pq, __global int *pq_info )
{
	int position =pq_info[PQ_END]+1;
	for(int i=pq_info[PQ_START]; i< pq_info[PQ_END];i++ )
	{
		if(pq[i].value > value){
			position = i;
			break;
		}
	}
	queue_item_struct temp={ .id = id, .value =value};
	if(position+1>pq_info[PQ_MAX] && pq_info[PQ_START] > 0)
	{
		// TODO SHIFT LEFT ADD TO END
		for(int i=pq_info[PQ_START]-1; i<position; i++ ){
			pq[i]=pq[i+1];
		}
		pq_info[PQ_START] = pq_info[PQ_START] -1;

	}else if(pq_info[PQ_START] > 0){
		// TODO SHIFT LEFT ADD AT POSITION
		for(int i=pq_info[PQ_START]-1; i<position; i++ ){
			pq[i]=pq[i+1];
		}
		pq_info[PQ_START] = pq_info[PQ_START] -1;

	}else if(pq_info[PQ_START] ==0 && pq_info[PQ_END] < pq_info[PQ_MAX] ){
		// SHIFT RIGHT
		for(int i= pq_info[PQ_END]; i>position; i--){
			pq[i]=pq[i-1];
		}
		pq_info[PQ_END] = pq_info[PQ_END]+1;
	}
	pq[position] = temp;

}
// returns the id of the top element in the queue
int pq_peek(__global  queue_item_struct * pq, __global int *pq_info){
	return pq[pq_info[PQ_START]].id;
}
// Removes one element form the queue
void pq_pop( __global int *pq_info){
	pq_info[PQ_START]= pq_info[PQ_START]+1;
}


int heuristic_function(const int size, __constant int * goal, __global int * matrix, const int start){
	int result=0;	
	for( int i=0; i < size * size; i++){
		for(int j=0; j < size * size; j++){
			if(goal[i]== matrix[start+j]){
				result += abs_diff( i, j);
			}
		}
	}
	return result;
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

	cost[(4*size)+ id] = heuristic_function(size, goal, result, start );
}
__kernel void search(const int size, __constant int * source, __constant int * target)
{
	//Get our global thread ID  
	int id = get_global_id(0);
	int lid= get_local_id(0);
}