#include <iostream>
#include <vector>
#include <string>

using namespace std;

class StringVectorHolder {
private:
    vector<string> data; // Class member variable to hold the vector of strings

public:
    // Constructor that initializes the class member variable with the input vector
    StringVectorHolder(const vector<string>& inputVector) : data(inputVector) {}

    // Method to print the contents of the vector
    void printData() const {
        for (const auto& str : data) {
            cout << str << endl;
        }
    }
};

int main() {
    // Example vector of strings
    vector<string> myVector = {
        "Hello, world!",
        "This is a test.",
        "C++ is great!"
    };

    // Create an instance of StringVectorHolder and initialize it with myVector
    StringVectorHolder holder(myVector);

    // Print the contents of the vector stored in the class
    holder.printData();

    return 0;
}
