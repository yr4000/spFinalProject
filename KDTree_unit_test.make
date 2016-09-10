CC = gcc
OBJS = KDTree_unit_test.o KDTree.o KDArray.o SPBPriorityQueue.o SPList.o SPListElement.o SPPoint.o SPLogger.o SPConfig.o DSBuilders.o
EXEC = KDTree_test
TESTS_DIR = ./unit_tests
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -lm -o $@
KDTree_unit_test.o: $(TESTS_DIR)/KDTree_unit_test.c $(TESTS_DIR)/unit_test_util.h $(TESTS_DIR)/DSBuilders.h KDTree.h
	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c
KDArray_unit_test.o: $(TESTS_DIR)/KDArray_unit_test.c $(TESTS_DIR)/unit_test_util.h KDArray.h SPPoint.h
	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c
SPPoint.o: SPPoint.c SPPoint.h
	$(CC) $(COMP_FLAG) -c $*.c
SPLogger.o: SPLogger.c SPLogger.h
	$(CC) $(COMP_FLAG) -c $*.c
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
	
SPConfig.o: SPConfig.c SPConfig.h SPLogger.h
	$(CC) $(C_COMP_FLAG) -c $*.c
	
DSBuilders.o: $(TESTS_DIR)/DSBuilders.c $(TESTS_DIR)/DSBuilders.h KDArray.h SPPoint.h
	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c
clean:
	rm -f $(OBJS) $(EXEC)