#include <iostream>
#include "HouseClass.cpp"


int main() {
    std::cout << "This is a program that analyzes your house plan and tells you what you need.\n"; 
    House userHouse;
    std::string filename = "HouseDimension_data.txt";
    //std::cout << "Enter the name of the file you want to read: ";
    //std::cin >> filename;
    userHouse.readHouseData(filename);
    std::cout << "Finished first function\n"; //FOR DEBUGGING
    userHouse.calculate_bricks();
    std::cout << "Finished second function\n"; //FOR DEBUGGING
    userHouse.displayHouseData();
    std::cout << "Finished third function\n"; //FOR DEBUGGING
}