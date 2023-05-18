CXX      = clang++
# CXXFLAGS = -g3 -std=c++20 -Ofast -Wall -Wextra -Wpedantic -Wshadow -Werror
CXXFLAGS = -g3 -std=c++20 -Ofast
LDFLAGS  = -g3

SRC = src
INCLUDE = src/include
LIB = -lsfml-graphics -lsfml-window -lsfml-system

main: main.o openings.o game.o viewer.o bot.o board.o
	${CXX} ${LDFLAGS} -o main $^ $(LIB) && mv *.o bin/

main.o: $(SRC)/main.cpp openings.o game.o viewer.o
	$(CXX) $(CXXFLAGS) -c src/main.cpp

openings.o: $(SRC)/openings.cpp $(INCLUDE)/openings.h bot.o
	$(CXX) $(CXXFLAGS) -c $(SRC)/openings.cpp

game.o: $(SRC)/game.cpp $(INCLUDE)/game.h bot.o
	$(CXX) $(CXXFLAGS) -c $(SRC)/game.cpp

viewer.o: $(SRC)/viewer.cpp $(INCLUDE)/viewer.h bot.o
	$(CXX) $(CXXFLAGS) -c $(SRC)/viewer.cpp

bot.o: $(SRC)/bot.cpp $(INCLUDE)/bot.h board.o 
	$(CXX) $(CXXFLAGS) -c $(SRC)/bot.cpp

board.o: $(SRC)/board.cpp $(INCLUDE)/board.h $(INCLUDE)/constants.h
	$(CXX) $(CXXFLAGS) -c $(SRC)/board.cpp

run:
	./main

clean:
	-rm *.o
	-rm main
