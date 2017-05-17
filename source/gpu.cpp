#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.hpp>
#include <exception>

void print(int size, int *matrix){
	for(int i = 0 ; i <size; i++){
		for(int j = 0 ; j < size; j++){
			std::cout << matrix[i*size+j] <<"\t";
		}
		std::cout << std::endl;
	}
}

void get_start(int size, int *result){
	int value = size * size;
	for(int i=0; i<size * size ; ++i){
		result[i] = value--;
	}
}
void get_goal(int size, int *result){
	int value = 1;
	for(int i=0; i<size * size ; ++i){
		result[i] = value++;
	}
}

int load_file(char* file_name, std::string &data){
  std::string line;
  std::ifstream my_file (file_name);
  if (my_file.is_open())
  {
    while ( getline (my_file,line) )
    {
		//if(line.find("//") == std::string::npos ){
			 data+= line +"\n";
		//}
    }
    my_file.close();
	
	return 0;
  }
  else {
	std::cout << "Unable to open file" <<std::endl;
    return 1;
	}
}
/*
 *
 */
int main(int argc, char * argv[]){
	
	int _size_ =2;
	if(argc<3){
		std::cout<< "Usage: n kernel_locatoin" <<std::endl;
		return -1;
	}
	_size_ = std::stoi(argv[1]);
	int n  = _size_ * _size_;
	int * start= new int[n];

	get_start(_size_, start);
	int * goal = new int [n];
	get_goal(_size_, goal);

	print(_size_, start);
	print(_size_, goal);
	std::vector<cl::Platform> all_platforms;
	cl::Platform::get(&all_platforms);

	if (all_platforms.size()==0) {
        std::cout<<" No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
	
	
	cl::Platform default_platform=all_platforms[0];
    std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    // get default device (CPUs, GPUs) of the default platform
    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size()==0){
        std::cout<<" No devices found. Check OpenCL installation!\n";
        exit(1);
    }
	
    // use device[0] because that's a GPU; device[1] is the CPU
    cl::Device default_device=all_devices[0];

    std::cout<< "Using device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<"\n";

	// a context is like a "runtime link" to the device and platform;
    // i.e. communication is possible
    cl::Context context({default_device});
    // create the program that we want to execute on the device
    cl::Program::Sources sources;

	std::string kernel_code="";
	if(load_file(argv[2],kernel_code)){
		std::cout<<"Problem Loading kernel file"<<std::endl;
		exit(1);
	}
	//std::cout<<kernel_code<<std::endl;
	sources.push_back({kernel_code.c_str(), kernel_code.length()});

    cl::Program program(context, sources);
    if (program.build({default_device}) != CL_SUCCESS) {
        std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << std::endl;
        exit(1);
    }
	// [4 *_size_][n] => [4 *2][4]=>[32]
	int depth_size = 4*_size_;
	int result_size = _size_*4*n;

	//cl::Buffer 		A(context, CL_MEM_READ_ONLY , sizeof(int), NULL, &command_result); //__constant int size
	cl::Buffer buffer_goal(context, CL_MEM_READ_ONLY ,   sizeof(int) * n);//__constant int * goal,
	cl::Buffer buffer_current(context, CL_MEM_READ_WRITE , sizeof(int) *n ); //__global int *current,
	cl::Buffer buffer_result(context, CL_MEM_READ_WRITE ,  sizeof(int) * result_size); //__global int **result
	cl::Buffer buffer_cost(context, CL_MEM_READ_WRITE ,  sizeof(int) * depth_size *2); 

	int * result= new int[result_size];
	int * cost  = new int[depth_size *2];

	cl::CommandQueue queue(context, default_device);
    // push write commands to queue
    queue.enqueueWriteBuffer(buffer_goal   , CL_TRUE, 0, sizeof(int)*n, goal);
    queue.enqueueWriteBuffer(buffer_current, CL_TRUE, 0, sizeof(int)*n, start);
  	queue.enqueueWriteBuffer(buffer_result , CL_TRUE, 0, sizeof(int) *result_size ,result );
 	queue.enqueueWriteBuffer(buffer_cost , CL_TRUE, 0, sizeof(int) *depth_size *2 , cost  );
	//GSD
	cl::Kernel next_nodes(program, "next_nodes");
	next_nodes.setArg(0,_size_);
	next_nodes.setArg(1,buffer_goal);
	next_nodes.setArg(2,buffer_current);
	next_nodes.setArg(3, buffer_result );
	next_nodes.setArg(4, buffer_cost );
	queue.enqueueNDRangeKernel(next_nodes, cl::NullRange ,cl::NDRange( depth_size  ),cl::NDRange(1));
	queue.enqueueReadBuffer(buffer_result, CL_TRUE, 0, sizeof(int) * result_size, result);
	queue.enqueueReadBuffer(buffer_cost, CL_TRUE, 0, sizeof(int) * depth_size*2, cost); 
	 try{
		for(int i=0; i<result_size; ++i){

			std::cout<< result[i] <<"\t";
			if( (i+1) % n ==0){
				std::cout<<std::endl;
				std::cout<<"cost["<<(i) / n<<"]:"<<cost[(i) / n] <<std::endl;
				std::cout<<"h["<<(i) / n<<"]:"<<cost[depth_size + ((i) / n)] <<std::endl;
			}		
			if((i+1)  % _size_ ==0){
				std::cout<<std::endl;
			}
		}
	 } catch(std::exception& e){
	 }

	delete start;
	start = nullptr;
	delete goal;
	goal = nullptr;
	delete result;
	result = nullptr;
	delete cost ;
	cost  = nullptr;
	return 0;
}