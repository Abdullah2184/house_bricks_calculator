#include <iostream>


int main() {
    std::cout << "This is a program that analyzes your house plan and tells you what you need.\n"; 
    House userHouse;
    std::cout << "Enter the name of the file you want to read: ";
    std::string filename;
    std::cin >> filename;
    userHouse.readHouseData(filename);
}