#include "Knn.h"
#include "Distances.h"
#include "Knn_subClasses.h"
#define P 2


        Knn_AUC::Knn_AUC(int k, std::multimap< std::vector<double> , std::string> data) {
            this->add_data(k, data);
        }
    

        double Knn_AUC::distance(std::vector<double> vect1, std::vector<double> vect2) {
            return Oclidic(vect1, vect2);
        }

        Knn_MAN::Knn_MAN(int k, std::multimap< std::vector<double> , std::string> data) {
            this->add_data(k, data);
        }
    
        double Knn_MAN::distance(std::vector<double> vect1, std::vector<double> vect2) {
            return Manhattan(vect1, vect2);
        }

        Knn_CHB::Knn_CHB(int k, std::multimap< std::vector<double> , std::string> data) {
            this->add_data(k, data);
        }
    

        double Knn_CHB::distance(std::vector<double> vect1, std::vector<double> vect2) {
            return Chebyshev(vect1, vect2);
        }

        Knn_CAN::Knn_CAN(int k, std::multimap< std::vector<double> , std::string> data) {
            this->add_data(k, data);
        }
    

        double Knn_CAN::distance(std::vector<double> vect1, std::vector<double> vect2) {
            return Canberra(vect1, vect2);
        }

        Knn_MIN::Knn_MIN(int k, std::multimap< std::vector<double> , std::string> data) {
            this->add_data(k, data);
        }
    

        double Knn_MIN::distance(std::vector<double> vect1, std::vector<double> vect2) {
            return Minkowski(vect1, vect2, P);
        }



