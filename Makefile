all: oss user

oss: main.cpp
	g++ -o oss main.cpp

user: helper.cpp
	g++ -o user helper.cpp
clean:
	rm oss
	rm user
	rm log.out
	
