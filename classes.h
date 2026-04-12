// Dummy classes for the motors and the breaks
#include <iostream>
using namespace std;

// Read this to see what the classes can do
class Motor {
public:
    void reverse();
    void forward();
    void stop();
};

class Break {
public:
    void open();
    void close();
};

// Implementations here. No need to really look at these
void Motor::reverse() {
    cout << "Motors moving in reverse\n";
}

void Motor::forward() {
    cout << "Motors moving forward\n";
}

void Motor::stop() {
    cout << "Motors off\n";
}

void Break::open() {
    cout << "Breaks open\n";
}

void Break::close() {
    cout << "Breaks closed\n";
}