CC = gcc
OBJS = KDArray_unit_test.o KDArray.o SPPoint.o SPLogger.o
EXEC = KDArray_unit_test
TESTS_DIR = ./unit_tests
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors -g

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -lm -o $@
KDArray_unit_test.o: $(TESTS_DIR)/KDArray_unit_test.c $(TESTS_DIR)/unit_test_util.h KDArray.h SPPoint.h SPLogger.h
	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c
KDArray.o: KDArray.c KDArray.h SPLogger.h SPPoint.h
	$(CC) $(COMP_FLAG) -c $*.c
SPPoint.o: SPPoint.c SPPoint.h
	$(CC) $(COMP_FLAG) -c $*.c
SPLogger.o: SPLogger.c SPLogger.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)