all: master palin

master: main.cpp
	g++ -o master main.cpp

palin: helper.cpp
	g++ -o palin helper.cpp
clean:
	rm master
	rm palin
	rm log.out
	rm palin.out
	rm nopalin.out
