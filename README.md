# MPI-KOKKOS_SYCL-comm-test
This is a test for the communication problem of MPI+KOKKOS_SYCL

1. Access ThetaGPU compute node (Interactive).
2.
   ```
   cd /grand/catalyst/world-shared/spatel
   source set_DPCPP_env.sh
   ```
3. Setup OMPI
   ```
   module load openmpi  
   export OMPI_CXX=clang++
   ```
4. Download tests
   ```
   cd ~
   git clone https://github.com/lucaso19891019/MPI-KOKKOS_SYCL-comm-test
   cd  MPI-KOKKOS_SYCL-comm-test
   ```
7. Compile
   ```
   make
   ```
8. Run
   ```
   mpirun -n {even number of ranks} ./waitall_ex
   ```


To run with CUDA backend

1. Login to ThetaGPU Compute node (Interactive).

2. Download Kokkos
   ```
   cd ~  
   mkdir kokkos
   cd kokkos  
   git clone https://github.com/kokkos/kokkos.git
   ```
3. Building Kokkos on the ThetaGPU node
   ```
   mkdir build
   cd build  
   module load cmake  
   module load openmpi  
   cmake ../kokkos \  
     -D CMAKE_CXX_FLAGS=-fopenmp \    
     -D CMAKE_BUILD_TYPE=Release \    
     -D CMAKE_INSTALL_PREFIX="${PWD}"/install \    
     -D Kokkos_ENABLE_CUDA=On \    
     -D Kokkos_ENABLE_CUDA_LAMBDA=On \    
     -D Kokkos_ENABLE_SERIAL=On \    
     -D Kokkos_ENABLE_OPENMP=On \    
     -D CMAKE_CXX_STANDARD=17    
   make -j install  
   export CMAKE_PREFIX_PATH="${PWD}"/install:"${CMAKE_PREFIX_PATH}"  
   export OMPI_CXX=~/kokkos/kokkos/bin/nvcc_wrapper  
   export OMP_PLACES=threads  
   export OMP_PROC_BIND=spread
   ```
4. Download test
   ```
   cd ~  
   git clone https://github.com/lucaso19891019/MPI-KOKKOS_SYCL-comm-test
   cd  MPI-KOKKOS_SYCL-comm-test   
   vi Makefile
   ```
5. In Makefile replace KOKKOS_PATH, KOKKOS_DEVICES and USERFLAGS with the commented lines. Save and Exit vi.

6. Compile
   ```
   make
   ```
7. Run
   ```
   mpirun -n {even number of ranks} ./waitall_ex
   ```
