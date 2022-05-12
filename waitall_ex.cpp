#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include <Kokkos_Core.hpp>
#ifdef USE_CUDA
#define myDeviceMemorySpace Kokkos::CudaSpace
#else
#define myDeviceMemorySpace Kokkos::Experimental::SYCLDeviceUSMSpace
#endif
#define myViewInt Kokkos::View<int *, myDeviceMemorySpace>
#define myMirrorViewInt Kokkos::View<int *, myDeviceMemorySpace>::HostMirror

/**
 * @brief Illustrates how to wait for the completion of multiple non-blocking
 * operations.
 * @details This program is meant to be run with even number of processes.
 **/
int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    // Get the number of processes and check if even number of processes are used
    int size;
	
	
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(size%2!=0)
    {
	printf("The number of ranks must be even.\n");
	exit(-1);
    }

    // Get my rank
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    Kokkos::initialize(argc, argv);
    {

        myViewInt bufferSend,bufferRecv;
	myMirrorViewInt Send,Recv;
	
	bufferSend=myViewInt("bufferSend",1);
	bufferRecv=myViewInt("bufferRecv",2);

	Send=Kokkos::create_mirror_view(bufferSend);
	Recv=Kokkos::create_mirror_view(bufferRecv);
	
	Send(0)=my_rank;
	Kokkos::deep_copy(bufferSend,Send);

	Kokkos::fence();

	printf("MPI process %d sends the value %d.\n", my_rank, Send(0));

	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Request requestsSend[2];
	MPI_Request requestsRecv[2];

    	if(my_rank <size/2)
    	{
	    // The MPI process sends the message.      
        
            MPI_Isend(bufferSend.data(), 1, MPI_INT, my_rank+size/2, 0, MPI_COMM_WORLD, &requestsSend[0]);
	    MPI_Isend(bufferSend.data(), 1, MPI_INT, size-1-my_rank, 0, MPI_COMM_WORLD, &requestsSend[1]);


	    // The MPI processes receive the message.       	
	    MPI_Request status[2];
            MPI_Irecv(bufferRecv.data(), 1, MPI_INT, my_rank+size/2, 0, MPI_COMM_WORLD, &requestsRecv[0]);
	    MPI_Irecv(bufferRecv.data()+1, 1, MPI_INT, size-1-my_rank, 0, MPI_COMM_WORLD, &requestsRecv[1]);	

	    Kokkos::deep_copy(Recv,bufferRecv);
            printf("Process %d received value %d from %d and value %d from %d.\n", my_rank, Recv(0),my_rank+size/2,Recv(1),size-1-my_rank);    
    	}
    	else
    	{

	    // The MPI process sends the message.

            MPI_Isend(bufferSend.data(), 1, MPI_INT, my_rank-size/2, 0, MPI_COMM_WORLD, &requestsSend[0]);
	    MPI_Isend(bufferSend.data(), 1, MPI_INT, size-1-my_rank, 0, MPI_COMM_WORLD, &requestsSend[1]);

            // The MPI processes receive the message.       	
            MPI_Irecv(bufferRecv.data(), 1, MPI_INT, my_rank-size/2, 0, MPI_COMM_WORLD, &requestsRecv[0]);
	    MPI_Irecv(bufferRecv.data()+1, 1, MPI_INT, size-1-my_rank, 0, MPI_COMM_WORLD, &requestsRecv[1]);

	    Kokkos::deep_copy(Recv,bufferRecv);
            printf("Process %d received value %d from %d and value %d from %d.\n", my_rank, Recv(0),my_rank-size/2,Recv(1),size-1-my_rank);
    	}
	
	MPI_Barrier(MPI_COMM_WORLD);

	// Wait for both routines to complete
        MPI_Waitall(2, requestsSend, MPI_STATUSES_IGNORE);
        printf("Process %d: both messages have been sent.\n", my_rank);

	MPI_Waitall(2, requestsRecv, MPI_STATUSES_IGNORE);
        printf("Process %d: both messages have been received.\n", my_rank);

    }
    Kokkos::finalize();


    MPI_Finalize();
    return EXIT_SUCCESS;
}
