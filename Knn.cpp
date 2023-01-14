#include <iterator>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "Distances.h"
#include "Knn.h"


        void Knn::add_data(int k, std::multimap< std::vector<double> , std::string> data) {
            this->k = k;
            this->data = data;
            std::multimap<double , std::string> newMap;
            this->nearest_map = newMap;
        }

        void Knn::fill_nearest_map(std::vector<double> unclassified) {
            if (this->data.empty()) {   // Checks if the data is filled
                std::cout << "Error! there is no data assigned" << endl;
                return;
            }

            std::multimap<std::vector<double>, std::string>::iterator it;
            double distance;
            for (it = this->data.begin(); it != this->data.end(); it++) {   //Loops through the data and makes a distances map
                distance = this->distance(unclassified, it->first);
                std::string name = it->second;
                this->nearest_map.insert(std::pair<double, std::string>(distance, name));
            }
        }

        std::string Knn::max_used_name() {
            if (this->data.empty()) {   // Checks if the data is filled
                std::cout << "Error! there is no data assigned" << endl;
                return nullptr;
            }

            std::map<std::string, int> count_map;
            std::multimap<double, std::string>::iterator it = this->nearest_map.begin();
            for (int i = 0; i < this->k; i++) {     //Loops on the k first values and creates a map that counts the names
                std::string name = it->second;
                if (count_map.count(name) == 0) {   // in case its the first time
                    count_map[name] = 1;
                } else {
                    count_map[name]++;
                }
                it++;
            }
            return this->max_counter(count_map);
        }

        std::string Knn::max_counter(std::map<std::string, int> count_map) {
            if (count_map.empty()) {   // Checks if the map is not empty
                std::cout << "Error! there is no data assigned" << endl;
                return nullptr;
            }

            std::string name = count_map.begin()->first;
            int max = count_map.begin()->second;
            std::map<std::string, int>::iterator it;
            for (it = count_map.begin(); it != count_map.end(); it++) {     // loops through the map
                if (it->second > max) {     // in case the current is bigger than temp
                    name = it->first;
                    max = it->second;
                }
                 //cout << "max name: " << name << ", quantity: " << max << endl;
            }
            return name;
        }
        

  
        std::string Knn::get_type(std::vector<double> unclassified) {
            this->fill_nearest_map(unclassified);
            std::string max_name = this->max_used_name();
            this->nearest_map.clear();
            return max_name;
        }
        
        
    