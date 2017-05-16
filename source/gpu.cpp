#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.hpp>



#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

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
      data+= line;
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

	int * start = new int[_size_];
	get_start(_size_, start);
	int * goal = new int [_size_];
	get_goal(_size_, goal);

	// OPEN CL INI SECTION
	cl_device_id device_id = NULL;
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;
	cl_mem memobj = NULL;
	cl_program program = NULL;
	cl_kernel kernel = NULL;
	cl_platform_id platform_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_int command_result;
	char string[MEM_SIZE];
	
	FILE *fp;
	char *source_str;
	size_t source_size;

	/* Load the source code containing the kernel*/
	fp = fopen(argv[2], "r");
	if (!fp)
	{
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}

	source_str = (char *)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	/* Get Platform and Device Info */
	command_result = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	command_result = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

	/* Create OpenCL context */
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &command_result);

	/* Create Command Queue */
	command_queue = clCreateCommandQueue(context, device_id, 0, &command_result);

	/* Create Memory Buffer */
	memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, MEM_SIZE * sizeof(char), NULL, &command_result);

	/* Create Kernel Program from the source */
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
										(const size_t *)&source_size, &command_result);

	/* Build Kernel Program */
	command_result = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	/* Create OpenCL Kernel */
	kernel = clCreateKernel(program, "hello", &command_result);

	/* Set OpenCL Kernel Parameters */
	command_result = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);

	/* Execute OpenCL Kernel */
	command_result = clEnqueueTask(command_queue, kernel, 0, NULL, NULL);

	/* Copy results from the memory buffer */
	command_result = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0,
		MEM_SIZE * sizeof(char), string, 0, NULL, NULL);
		
	/* Clean Up */
	command_result = clFlush(command_queue);
	command_result = clFinish(command_queue);
	command_result = clReleaseKernel(kernel);
	command_result = clReleaseProgram(program);
	command_result = clReleaseMemObject(memobj);
	command_result = clReleaseCommandQueue(command_queue);
	command_result = clReleaseContext(context);
	free(source_str);

	delete start;
	start = nullptr;
	delete goal;
	goal = nullptr;
}