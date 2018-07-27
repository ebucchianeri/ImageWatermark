FF_ROOT		 = /usr/local/

CXX 		   = g++
CXX_FLAGS 	= -std=c++17 -O3
LD_FLAGS	   = -lstdc++fs

INCLUDES	= -I $(FF_ROOT)


.PHONY: clean clean_immagini

all: clean seq.esteso.bin main.seq.bin main.thrnorr.bin main.thr.bin main.ff.bin

seq.esteso.bin: src/sequenziale.cpp
	   $(CXX) $^ $(CXX_FLAGS) -o $@ $(LD_FLAGS) 

main.seq.bin: src/main_seq.cpp
		$(CXX) $^ $(CXX_FLAGS) -o $@ $(LD_FLAGS) 
		
main.thrnorr.bin: src/main_thrnorr.cpp
		$(CXX) $^ $(CXX_FLAGS) -o $@ $(LD_FLAGS) -pthread
		
main.thr.bin: src/main_thr.cpp
		$(CXX) $^ $(CXX_FLAGS) -o $@ $(LD_FLAGS) -pthread

main.ff.bin: src/main_ff.cpp
	  $(CXX) $^ $(CXX_FLAGS) $(INCLUDES) -o $@ $(LD_FLAGS) -pthread
	  
clean:
	-rm *.bin

