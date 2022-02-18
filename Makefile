CC = gcc
CFLAGS = -Wall -Wno-pointer-to-int-cast

main: main.o helper.o calcuTradeInterval.o
	${CC} main.o helper.o calcuTradeInterval.o ${CFLAGS} -o out/main.bin
main.o: src/helper.h src/calcuTradeInterval.h
	${CC} -c src/main.c ${CFLAGS}
helper.o: src/helper.h
	${CC} -c src/helper.c ${CFLAGS}
calcuTradeInterval.o: src/calcuTradeInterval.h
	${CC} -c src/calcuTradeInterval.c ${CFLAGS}
clean:
	@rm -fr *.o ./out/*.bin
