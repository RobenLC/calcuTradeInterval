CC = gcc
CFLAGS = -Wall -Wno-pointer-to-int-cast

main: main.o helper.o calcuTradeInterval.o
	${CC} main.o helper.o calcuTradeInterval.o ${CFLAGS} -o out/main.bin
main.o: src/main.c src/helper.c src/calcuTradeInterval.c
	${CC} -c src/main.c ${CFLAGS}
helper.o: src/helper.c
	${CC} -c src/helper.c ${CFLAGS}
calcuTradeInterval.o: src/calcuTradeInterval.c
	${CC} -c src/calcuTradeInterval.c ${CFLAGS}
clean:
	@rm -fr *.o ./out/*.bin
