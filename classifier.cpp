#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cassert>

std::vector<int> dataReader(std::istream& in) {
    std::string curLine;
    char delimiter = '\t';
    std::vector<int> dataTuple;

    std::getline(in, curLine);

    std::stringstream unsplitLine(curLine);
    std::string data;

    while(std::getline(unsplitLine, data, delimiter)) {
        dataTuple.push_back(std::stoi(data));
    }

    in.ignore();

    return dataTuple;
}

int main(int argc, char **argv) {
    const int NUM_WEIGHTS = 5;
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

            assert(tuple.size() == 3);

            // clear the data for next iteration
            tuple.clear();
        }

        // reset fin to the top of file
        fin.clear();
        fin.seekg(0);
    }

    fin.close();

    return 0;
}