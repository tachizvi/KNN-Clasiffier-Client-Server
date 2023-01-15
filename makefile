all : ex2.out

some: clean Server.out Client.out 

Server.out: Server.o input.o ex2.o Knn.o Knn_subClasses.o Distances.o HndaleClient.o
	g++ -o Server.out Server.o input.o ex2.o Knn.o Knn_subClasses.o Distances.o HndaleClient.o -g -std=c++11


Server.o:
	g++ -c Server.cpp input.cpp ex2.h HndaleClient.h -std=c++11

Client.out: Client.o input.o
	g++ -o Client.out Client.o input.o -g -std=c++11


Client.o:
	g++ -c Client.cpp input.cpp -std=c++11

ex2.out : ex2.o input.o Distances.o Knn.o Knn_subClasses.o
	g++ -o ex2.out ex2.o input.o Distances.o Knn.o Knn_subClasses.o -std=c++11

ex2.o : ex2.cpp input.h Distances.h Knn.h Knn_subClasses.h
	g++ -c ex2.cpp input.h Distances.h Knn.h Knn_subClasses.h -std=c++11

input.o : input.cpp
	g++ -c input.cpp -std=c++11

Distances.o : Distances.cpp
	g++ -c Distances.cpp -std=c++11

Knn.o : Knn.cpp Knn.h
	g++ -c Knn.cpp Knn.h -std=c++11

Knn_subClasses.o : Knn_subClasses.cpp Knn_subClasses.h Knn.h
	g++ -c Knn_subClasses.cpp Knn_subClasses.h Knn.h -std=c++11

HndaleClient.o: Knn.cpp Knn_subClasses.cpp ex2.cpp
	g++ -c HndaleClient.cpp Knn_subClasses.h Knn.h ex2.h -std=c++11


clean :
	rm -f *.o *.gch *.out