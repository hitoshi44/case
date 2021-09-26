CC	= gcc
CFLAGS	= -g -Wall

TOBJS	= \
	test/hash.o

UNT	= test/unity/unity.c




clean:
	@rm -f ./*.o
	@echo "cleaned dirs:  ./ "
fclean:
	@rm -f ./*.o
	@rm -f ./test/*.o
	@rm -f ./src/*.o
	@echo "cleaned dirs:  ./  ./test/  ./src/"

test:	$(TOBJS)
	@echo "==== Test With Unity ===="
	@./test/hash.o

test/hash.o:
	@$(CC) $(CFLAGS) $(UNT) src/hash.c test/hash.c -o test/hash.o
