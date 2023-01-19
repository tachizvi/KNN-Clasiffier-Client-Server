all : ex2.out

some: clean Server.out Client.out 

Server.out:
	g++ -o Server.out Server.cpp input.cpp ex2.cpp Knn.cpp Knn_subClasses.cpp Distances.cpp HndaleClient.cpp CLI.cpp SocketIO.cpp Command.cpp -g -std=c++11



Client.out: 
	g++ -o Client.out Client.cpp input.cpp ex2.cpp Knn.cpp Knn_subClasses.cpp Distances.cpp HndaleClient.cpp CLI.cpp SocketIO.cpp Command.cpp -g -std=c++11

clean :
	rm -f *.o *.gch *.out