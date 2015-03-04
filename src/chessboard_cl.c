#include "chessboard.h"




int err;                            // error code returned from api calls

size_t global;                      // global domain size for our calculation
size_t local;                       // local domain size for our calculation

cl_int  queens_buf[MAX_QUEENS];
cl_int  queens_result;

cl_device_id device_id;             // compute device id 
cl_context context;                 // compute context
cl_command_queue commands;          // compute command queue
cl_program program;                 // compute program
cl_kernel kernel;                   // compute kernel

cl_mem input;                       // device memory used for the input array
cl_mem output;                      // device memory used for the output array

const char *kernelSource = "\n"\
"#pragma OPENCL EXTENSION cl_khr_global_int32_base_atomics : enable\n"\
"#pragma OPENCL EXTENSION cl_khr_local_int32_base_atomics : enable\n"\
"#pragma OPENCL EXTENSION cl_khr_global_int32_extended_atomics : enable\n"\
"#pragma OPENCL EXTENSION cl_khr_local_int32_extended_atomics : enable\n"\
"__kernel void validate(                \n"\
"       __global uint* queens,          \n"\  
"       __global uint* output,          \n"\
"       const uint col,                 \n"\
"       const uint size)                \n"\
"{                                      \n"\
"       uint i = get_global_id(0);      \n"\
"       atom_or(output, i < size &&     \n"\
"               i != col &&             \n"\
"               abs_diff(col, i) == abs_diff(queens[col], queens[i]));\n"\
"}";


int    cl_init() {
        int gpu = 1;
        err = clGetDeviceIDs(NULL, gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
        if (err != CL_SUCCESS)
        {
                printf("Error: Failed to create a device group!\n");
                return EXIT_FAILURE;
        }

        // Create a compute context 
        //
        context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
        if (!context)
        {
                printf("Error: Failed to create a compute context!\n");
                return EXIT_FAILURE;
        }

        // Create a command commands
        //
        commands = clCreateCommandQueue(context, device_id, 0, &err);
        if (!commands)
        {
                printf("Error: Failed to create a command commands!\n");
                return EXIT_FAILURE;
        }

        // Create the compute program from the source buffer
        //
        program = clCreateProgramWithSource(context, 1, (const char **) & kernelSource, NULL, &err);
        if (!program)
        {
                printf("Error: Failed to create compute program!\n");
                return EXIT_FAILURE;
        }

        // Build the program executable
        //
        err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
        if (err != CL_SUCCESS)
        {
                size_t len;
                char buffer[4096];

                printf("Error: Failed to build program executable!\n");
                clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
                printf("%s\n%d\n", buffer, err);
                exit(1);
        }

        // Create the compute kernel in the program we wish to run
        //
        kernel = clCreateKernel(program, "validate", &err);
        if (!kernel || err != CL_SUCCESS)
        {
                printf("Error: Failed to create compute kernel!\n");
                exit(1);
        } 

        // Create the input and output arrays in device memory for our calculation
        //
        input = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(cl_int) * MAX_QUEENS, NULL, NULL);
        output = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_int), NULL, NULL);
        if (!input || !output)
        {
                printf("Error: Failed to allocate device memory!\n");
                exit(1);
        } 

        // Get the maximum work group size for executing the kernel on the device
        //
        err = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
        if (err != CL_SUCCESS)
        {
                printf("Error: Failed to retrieve kernel work group info! %d\n", err);
                exit(1);
        }
}

int     cb_validates_cl(const cb_t* cb, size_t row, size_t col) {
        // Write our data set into the input array in device memory 
        //
        err = clEnqueueWriteBuffer(commands, input, CL_TRUE, 0, sizeof(cl_int) * MAX_QUEENS, cb->queens, 0, NULL, NULL);
        if (err != CL_SUCCESS)
        {
                printf("Error: Failed to write to source array!\n");
                exit(1);
        }
        queens_result = 0;
        clEnqueueWriteBuffer(commands, output, CL_TRUE, 0, sizeof(cl_int), &queens_result, 0, NULL, NULL);
        if (err != CL_SUCCESS)
        {
                printf("Error: Failed to write to source array!\n");
                exit(1);
        }
        // Set the arguments to our compute kernel
        //
        err = 0;
        err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
        err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
        err |= clSetKernelArg(kernel, 2, sizeof(size_t), &col);
        err |= clSetKernelArg(kernel, 3, sizeof(size_t), &cb->size);
        if (err != CL_SUCCESS)
        {
                printf("Error: Failed to set kernel arguments! %d\n", err);
                exit(1);
        }

        global = MAX_QUEENS;
        err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);
        if (err)
        {
                printf("Error: Failed to execute kernel!\n%d\n", err);
                return EXIT_FAILURE;
        }

        clFinish(commands);


        err = clEnqueueReadBuffer( commands, output, CL_TRUE, 0, sizeof(cl_int), &queens_result, 0, NULL, NULL );  
        if (err != CL_SUCCESS)
        {
                printf("Error: Failed to read output array! %d\n", err);
                exit(1);
        }

        // printf("%d\n", queens_result);

        return queens_result;
}