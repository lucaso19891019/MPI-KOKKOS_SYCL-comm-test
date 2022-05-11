KOKKOS_PATH = /grand/catalyst/world-shared/spatel/kokkos/source
KOKKOS_DEVICES = "SYCL"
#KOKKOS_PATH = /home/lucaso/kokkos/kokkos
#KOKKOS_DEVICES = "Cuda,OpenMP"


KOKKOS_ARCH = "Ampere80"
KOKKOS_CUDA_OPTIONS = "enable_lambda"
KOKKOS_CXX_STANDARD=c++17

default: waitall_ex



USERFLAGS = -O3 -fsycl -fsycl-targets=nvptx64-nvidia-cuda-sycldevice -Wno-unknown-cuda-version
#USERFLAGS = -DUSE_CUDA -O3
USERFLAGS += -g -std=c++17 -I../   
CXXFLAGS = $(USERFLAGS)
LINKFLAGS = $(USERFLAGS)
CXX = mpicxx

LINK = $(CXX)

DEPFLAGS = -M
LIB =

include $(KOKKOS_PATH)/Makefile.kokkos

FOBJS = waitall_ex.o



waitall_ex: $(FOBJS) $(KOKKOS_LINK_DEPENDS)
	$(LINK) $(KOKKOS_LDFLAGS) $(LINKFLAGS) $(EXTRA_PATH) $(FOBJS) $(KOKKOS_LIBS) $(LIB) -o $@

clean: kokkos-clean
	rm -f *.o *.h *.tmp *.hpp *.a;  rm -f *.o *.cuda *.host *.dat

# Compilation rules

%.o:%.cpp $(KOKKOS_CPP_DEPENDS)
	$(CXX) $(KOKKOS_CPPFLAGS) $(KOKKOS_CXXFLAGS) $(CXXFLAGS) $(EXTRA_INC) -c $<

