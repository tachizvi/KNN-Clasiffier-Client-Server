
#ifndef KNN_SUBCLASSES_H
#define KNN_SUBCLASSES_H
#include "Distances.h"
#include "Knn.h"
#define P 2

class Knn_AUC : public Knn {

    public:
        Knn_AUC(int k, std::multimap< std::vector<double> , std::string> data) ;
    
    protected:

        double distance(std::vector<double> vect1, std::vector<double> vect2) ;

};

class Knn_MAN : public Knn {
    public:
        Knn_MAN(int k, std::multimap< std::vector<double> , std::string> data);
    protected:

        double distance(std::vector<double> vect1, std::vector<double> vect2);
};

class Knn_CHB : public Knn {
public:
        Knn_CHB(int k, std::multimap< std::vector<double> , std::string> data) ;
    
    protected:

        double distance(std::vector<double> vect1, std::vector<double> vect2) ;

};

class Knn_CAN : public Knn {
public:
        Knn_CAN(int k, std::multimap< std::vector<double> , std::string> data) ;
protected:
         double distance(std::vector<double> vect1, std::vector<double> vect2);

};

class Knn_MIN : public Knn {
public:
        Knn_MIN(int k, std::multimap< std::vector<double> , std::string> data) ;
    
    protected:

        double distance(std::vector<double> vect1, std::vector<double> vect2) ;
};

#endif