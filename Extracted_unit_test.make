CC = gcc
OBJS = Extracted_unit_test.o Extracted.o SPPoint.o SPConfig.o SPLogger.o
EXEC = Extracted_unit_test
TESTS_DIR = ./unit_tests
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
Extracted_unit_test.o: $(TESTS_DIR)/Extracted_unit_test.c $(TESTS_DIR)/unit_test_util.h Extracted.h SPPoint.h SPConfig.h
	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c
Extracted.o: Extracted.c Extracted.h SPLogger.h SPConfig.h SPPoint.h
	$(CC) $(COMP_FLAG) -c $*.c	
SPConfig.o: SPConfig.c SPConfig.h SPLogger.h
	$(CC) $(COMP_FLAG) -c $*.c	
SPLogger.o: SPLogger.c SPLogger.h
	$(CC) $(COMP_FLAG) -c $*.c
SPPoint.o: SPPoint.c SPPoint.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
