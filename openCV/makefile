CC = gcc
CPP = g++
OBJS = main.o SPImageProc.o SPPoint.o SPLogger.o SPConfig.o SPList.o SPKDArrays.o SPListElement.o SPFeatures.o SPBPriorityQueue.o KDTreeBuiler.o
EXEC = SPCBIR
INCLUDEPATH=/usr/local/lib/opencv-3.1.0/include/
LIBPATH=/usr/local/lib/opencv-3.1.0/lib/
LIBS=-lopencv_xfeatures2d -lopencv_features2d \
-lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_core


CPP_COMP_FLAG = -std=c++11 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

C_COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(CPP) $(OBJS) -L$(LIBPATH) $(LIBS) -o $@
main.o: main.cpp SPImageProc.h SPConfig.h KDTreeBuiler.h SPKDArrays.h SPLogger.h SPFeatures.h SPListElement.h SPBPriorityQueue.h
	$(CPP) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
SPImageProc.o: SPImageProc.cpp SPImageProc.h SPConfig.h SPPoint.h SPLogger.h
	$(CPP) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
	
SPLogger.o: SPLogger.c SPLogger.h 
	$(CC) $(C_COMP_FLAG) -c $*.c
SPPoint.o: SPPoint.c SPPoint.h SPLogger.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPConfig.o: SPConfig.c SPConfig.h 
	$(CC) $(C_COMP_FLAG) -c $*.c	
SPList.o: SPList.c SPList.h 
	$(CC) $(C_COMP_FLAG) -c $*.c	
SPKDArrays.o: SPKDArrays.c SPKDArrays.h SPPoint.h SPLogger.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPListElement.o: SPListElement.c SPListElement.h SPLogger.h
	$(CC) $(C_COMP_FLAG) -c $*.c	
SPFeatures.o: SPFeatures.c SPFeatures.h SPPoint.h SPLogger.h
	$(CC) $(C_COMP_FLAG) -c $*.c	
SPBPriorityQueue.o: SPBPriorityQueue.c SPBPriorityQueue.h SPListElement.h SPList.h SPLogger.h
	$(CC) $(C_COMP_FLAG) -c $*.c
KDTreeBuiler.o: KDTreeBuiler.c KDTreeBuiler.h SPKDArrays.h SPPoint.h SPConfig.h SPListElement.h SPLogger.h SPBPriorityQueue.h
	$(CC) $(C_COMP_FLAG) -c $*.c

clean:
	rm -f $(OBJS) $(EXEC)
