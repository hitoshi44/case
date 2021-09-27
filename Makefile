CC	= gcc
CFLAGS	= -g -Wall

TOBJS	= \
	test/hash.o \
	test/header.o \
	test/body.o

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
	@echo "\n\n====[ test Hash ]===="
	@./test/hash.o
	@echo "\n\n====[ test Header ]===="
	@./test/header.o
	@echo "\\n\n===[ test Body ]===="
	@./test/body.o

test/hash.o:
	@$(CC) $(CFLAGS) $(UNT) src/hash.c test/hash.c -o test/hash.o
test/header.o:
	@$(CC) $(CFLAGS) $(UNT) src/header.c test/header.c -o test/header.o
test/body.o:
	@$(CC) $(CFLAGS) $(UNT) src/body.c test/body.c -o test/body.o
