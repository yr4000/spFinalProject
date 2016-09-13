CC = gcc
CPP = g++
#put your object files here
OBJS = main.o main_aux.o SPLogger.o SPConfig.o SPPoint.o SPList.o SPListElement.o SPBPriorityQueue.o Extracted.o KDArray.o KDTree.o SPImageProc.o 
#The executabel filename
EXEC = SPCBIR
INCLUDEPATH=/usr/local/lib/opencv-3.1.0/include/
LIBPATH=/usr/local/lib/opencv-3.1.0/lib
LIBS=-lopencv_xfeatures2d -lopencv_features2d \
-lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_core -lopencv_cudaarithm -lopencv_flann


CPP_COMP_FLAG = -std=c++11 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

C_COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(CPP) $(OBJS) -L$(LIBPATH) $(LIBS) -o $@

main.o: main.cpp SPLogger.h main_aux.h SPPoint.h SPConfig.h Extracted.h KDTree.h SPImageProc.h
	$(CPP) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp

SPImageProc.o: SPImageProc.cpp SPImageProc.h SPConfig.h SPPoint.h SPLogger.h
	$(CPP) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp

main_aux.o: main_aux.c main_aux.h SPLogger.h SPPoint.h SPBPriorityQueue.h SPConfig.h Extracted.h KDArray.h KDTree.h
	$(CC) $(C_COMP_FLAG) -c $*.c

SPConfig.o: SPConfig.c SPConfig.h SPLogger.h
	$(CC) $(C_COMP_FLAG) -c $*.c

SPLogger.o: SPLogger.c SPLogger.h
	$(CC) $(C_COMP_FLAG) -c $*.c

Extracted.o: Extracted.c Extracted.h SPLogger.h SPConfig.h SPPoint.h
	$(CC) $(C_COMP_FLAG) -c $*.c

SPPoint.o: SPPoint.c SPPoint.h 
	$(CC) $(C_COMP_FLAG) -c $*.c

SPListElement.o: SPListElement.c SPListElement.h
	$(CC) $(C_COMP_FLAG) -c $*.c

SPList.o: SPList.c SPList.h SPListElement.h
	$(CC) $(C_COMP_FLAG) -c $*.c

SPBPriorityQueue.o: SPBPriorityQueue.c SPBPriorityQueue.h SPListElement.h SPList.h
	$(CC) $(C_COMP_FLAG) -c $*.c

KDArray.o: KDArray.c KDArray.h SPLogger.h SPPoint.h
	$(CC) $(C_COMP_FLAG) -lm -c $*.c

KDTree.o: KDTree.c KDTree.h SPLogger.h SPBPriorityQueue.h SPPoint.h KDArray.h SPConfig.h
	$(CC) $(C_COMP_FLAG) -c $*.c

clean:
	rm -f $(OBJS) $(EXEC)
