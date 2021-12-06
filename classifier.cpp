#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cassert>
#include <cmath>

std::vector<int> dataReader(std::istream& in) {
    std::string curLine;
    char delimiter = '\t';
    std::vector<int> dataTuple;

    std::getline(in, curLine);

    std::stringstream unsplitLine(curLine);
    std::string data;

    dataTuple.push_back(1); // x0 is always 1

    while(std::getline(unsplitLine, data, delimiter)) {
        dataTuple.push_back(std::stoi(data));
    }

    return dataTuple;
}

template <typename T>
double sig(const T z) {
    return 1 / (1 + exp(-z));
}

double getDelta(const std::vector<double>& weights, const std::vector<int>& data) {
    double ycap;
    double sum = 0;
    double delta;

    for(int i = 0; i < weights.size(); ++i) {
        sum += weights[i] * data[i];
    }
    ycap = sig(sum);

    delta = data[data.size() - 1] - ycap; // y(e) - ycap
    return delta;
} 

int main(int argc, char **argv) {
    const int NUM_WEIGHTS = 3;
    const int NUM_ITERATIONS = 100;
    srand(1);

    if(argc != 2) {
        exit(1);
    }

    // open connection to file
    std::string filename = argv[1];
    std::ifstream fin(filename);

    if(!fin.good()) {
        exit(1);
    }

    // initialize weights
    std::vector<float> weights(NUM_WEIGHTS);

    // randomize weights
    for(auto& weight : weights) {
        weight = static_cast<float>(rand());
        std::cout << weight << ", ";
    }
    std::cout << "\n";

    // repeat for desired number of iterations
    for(int j = 0; j < NUM_ITERATIONS; ++j) {
        std::vector<int> tuple;
        // reads each datapoint from the file until it reaches eof
        while(!fin.eof()) {
            // gets the data from the file
            tuple = dataReader(fin);
            for(std::size_t i = 0; i < tuple.size(); ++i) {
                std::cout << tuple[i] << ", ";
            }
            std::cout << std::endl;
        }

        // reset fin to the top of file
        fin.clear();
        fin.seekg(0);
    }

    fin.close();

    return 0;
}