CC = gcc
OBJS = config_unit_test.o SPConfig.o SPLogger.o
EXEC = config_unit_test
TESTS_DIR = ./unit_tests
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
config_unit_test.o: $(TESTS_DIR)/config_unit_test.c $(TESTS_DIR)/unit_test_util.h SPConfig.h
	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c
SPConfig.o: SPConfig.c SPConfig.h SPLogger.h
	$(CC) $(COMP_FLAG) -c $*.c	
SPLogger.o: SPLogger.c SPLogger.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
