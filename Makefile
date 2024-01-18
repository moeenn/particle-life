PROJECT = particle-life

CC = clang++
SRC_DIR =./src
SRC = $(wildcard ${SRC_DIR}/*.cpp)
OUT_DIR = bin
OBJ = ${SRC:.cpp=.o}
LIBS = -lm -lraylib
CFLAGS = -Wextra -Werror -Wall -std=c++20 -O3
BINARY = ${OUT_DIR}/${PROJECT}

build: ${OBJ}
	${CC} -o ${BINARY} ${OUT_DIR}/*.o ${LIBS}

.cpp.o:
	${CC} -c ${CFLAGS} $<
	@mv ./*.o ${OUT_DIR}/

fmt:
	clang-format -i -style=Chromium ${SRC_DIR}/*.cpp ${SRC_DIR}/*.hpp

run:
	@${BINARY}

clean:
	@rm -v ${OUT_DIR}/*.o ${BINARY}
