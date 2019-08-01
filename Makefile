TESTS=$(wildcard test/*.c)
HDRS=$(wildcard src/*.h)
SRCS=$(wildcard src/*.c)
OBJS=$(patsubst src/%.c,build/%.o,${SRCS})
BINARIES=$(patsubst test/%.c,build/%,${TESTS})

$(info $(TESTS))
$(info $(HDRS))
$(info $(BINARIES))

CC = clang
CFLAGS  = -c -g -std=gnu99
LDFLAGS = -lm

# $^        : toutes les dépendences
# $@        : sortie générée
# CC       : variable d'environnement (compilateur)
# CCFLAGS+ :

all: ${BINARIES}

build/%: build/%.o ${OBJS}
	${CC} ${LDFLAGS} $^ -o $@

build/%.o: src/%.c ${HDRS}
	${CC} ${CFLAGS} -c $< -o $@

build/%.o: test/%.c ${HDRS}
	${CC} ${CFLAGS} -I./src -c $< -o $@

clean:
	${RM} build/*

check: ${BINARIES}
	./build/testLab1
	./build/testLab2

