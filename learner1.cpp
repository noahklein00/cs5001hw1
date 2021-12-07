#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <ctime>

std::vector<std::vector<int>> dataReader(std::istream& in) {
    constexpr char DELIMITER = '\t';
    std::string curLine;
    std::vector<std::vector<int>> data;
    std::size_t index = 0;

    while(std::getline(in, curLine) && curLine.size() > 0) {
        std::stringstream unsplitLine(curLine);
        std::string catchData;
        data.push_back({});
        data[index].push_back(1); // x0 is always 1

        while(std::getline(unsplitLine, catchData, DELIMITER)) {
            data[index].push_back(std::stoi(catchData));
        }
        ++index;
    }

    return data;
}

template <typename T>
double sig(const T z) {
    return 1 / (1 + exp(-z));
}

double getDelta(const std::vector<double>& weights, const std::vector<int>& dataTuple) {
    double ycap;
    double sum = 0;
    double delta;

    for(std::size_t i = 0; i < weights.size(); ++i) {
        sum += weights[i] * dataTuple[i];
    }
    ycap = sig(sum);

    delta = (dataTuple[dataTuple.size() - 1] - ycap) * ycap * (1 - ycap);
    return delta;
}

void updateWeights(std::vector<double>& weights, const std::vector<int>& dataTuple, const double eta) {
    double delta = getDelta(weights, dataTuple);

    for(std::size_t i = 0; i < weights.size(); ++i) {
        weights[i] = weights[i] + (eta * delta * static_cast<double>(dataTuple[i]));
    }
    return;
}

double SSE(const std::vector<double>& weights, const std::vector<std::vector<int>>& data) {
    double ycap;
    double sum;
    double sse = 0;

    for(std::size_t k = 0; k < data.size(); ++k) {
        sum = 0;
        for(std::size_t i = 0; i < weights.size(); ++i) {
            sum += weights[i] * data[k][i];
        }

        ycap = sig(sum);
        sse += std::pow(data[k][data[k].size() - 1] - ycap, 2);
    }

    return sse;
}

int main(int argc, char **argv) {
    const int NUM_WEIGHTS = 3;
    const double ETA = 0.001;
    const int NUM_ITERATIONS = 100000;
    srand(time(0));

    if(argc != 4) {
        exit(1);
    }

    // open connection to file
    std::string filename = argv[1];
    std::ifstream fin(filename);
    std::vector<std::vector<int>> data; // Stores training data

    if(fin.good()) {
        data = dataReader(fin);        
    } else {
        std::cout << "Could not open connection to " << argv[1] << std::endl;
        exit(1);
    }

    fin.close();

    // ---------------- generate weights from training data ---------------

    // initialize weights
    std::vector<double> weights(NUM_WEIGHTS);

    // randomize weights
    for(auto& weight : weights) {
        weight = static_cast<double>((rand() % 4000 - 2000) / 1000.0); // [-2, 2]
    }

    // repeat for desired number of iterations
    for(int k = 0; k < NUM_ITERATIONS; ++k) {
        for(std::size_t i = 0; i < data.size(); ++i) {
            updateWeights(weights, data[i], ETA);
        }
    }

    // --------------- check weights against validation data -----------------

    filename = argv[2];
    fin.open(filename);

    std::vector<std::vector<int>> validData; // stores validation data

    if(fin.good()) {
        validData = dataReader(fin);
    } else {
        std::cout << "Could not open connection to " << argv[2] << std::endl;
        exit(1);
    }

    fin.close();

    double sumOfSquaresError = SSE(weights, validData);

    // --------------- output to txt file -----------------

    // open connection to file
    filename = argv[3];
    std::ofstream fout(filename);

    if(fout.good()) {
        // output final weights
        for(std::size_t i = 0; i < weights.size(); ++i) {
            fout << weights[i] << " ";
        }
        fout << "\n";

        // output sum-of-squares error
        fout << sumOfSquaresError << "\n\n";

        // output name and class
        fout << "CS-5001: HW#1\n"
             << "Programmer: Noah E. Klein\n\n";

        // output parameters
        fout << "TRAINING:\n"
             << "Using learning rate eta = " << ETA << "\n"
             << "Using " << NUM_ITERATIONS << " iterations.\n\n";

        fout << "LEARNED:\n";
        for(std::size_t i = 0; i < weights.size(); ++i) {
            fout << "w" << i << " = " << weights[i] << "\n";
        }
        fout << "\n";

        fout << "VALIDATION\n"
             << "Sum-of-Squares Error = " << sumOfSquaresError << std::endl;
    } else {
        std::cout << "Could not open connection to " << argv[3] << std::endl;
        exit(1);
    }

    fout.close();

    return 0;
}