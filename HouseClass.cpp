#include <iostream>
#include <string>
#include <fstream>
#include "WallsClass.cpp"
#include "WindowsClass.cpp"
#include "BricksClass.cpp"
#include "DoorsClass.cpp"

class House {
    private:
    std::string owner_name;
    Bricks* bricks_arr;
    int num_bricks_types;
    //int num_bricks_req; //Since the bricks will be of different types
    Walls* walls_arr;
    int num_walls;
    Windows* windows_arr;
    int num_windows;
    Doors* doors_arr;
    int num_doors;
    std::string house_data; //holds all the data on the required bricks

    public:
    House();
    House(std::string filename);
    ~House();
    void readHouseData(std::string filename);
    void append_Wall(Walls*& walls_arr, Walls wall, int& num_walls);
    void append_Window(Windows* windows_arr, Windows window, int& num_windows);
    void append_Door(Doors* door_arr, Doors door, int& num_doors);
    void append_Brick(Bricks* bricks_arr, Bricks brick, int& num_bricks_types);
    void displayHouseData(std::string house_data);
    void writeHouseData(std::string filename, std::string house_data);
    void set_name();
};

House::House() {
    owner_name = "";
    bricks_arr = nullptr;
    //num_bricks_req = 0;
    walls_arr = nullptr;
    num_walls = 0;
    windows_arr = nullptr;
    num_windows = 0;
    doors_arr = nullptr;
    num_doors = 0;
}

House::~House() {
    delete[] bricks_arr;
    delete[] walls_arr;
    delete[] windows_arr;
    delete[] doors_arr;
}

void House::readHouseData(std::string filename) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error opening file: " << filename << endl;
        return;
    }

    //Write the code to read the formatted data from the file
    //Reads word by word for each line, then based on the
    //first word, it assigns the rest of the line to the appropriate variable
    std::string header;
    std::string obj_type;
    getline(infile, header); //removes the column lines from the filestream buffer
    while(infile) {
        infile >> obj_type; //Takes the first column to identify the type of object

        //Performs specific instructions as per object type
        if (obj_type == "Wall") {
            
            //Read all the attributes of the object
            std::string identifier, brick_type; double height, width, thickness;
            infile >> identifier >> height >> width >> thickness >> brick_type;
            
            //Construct the window object using class constructor
            Walls wall_input(identifier, height, width, thickness, brick_type);
            
            //Append the new object to the dynamic object array
            append_Wall(walls_arr, wall_input, num_walls);

        } else if (obj_type == "Window") {

            std::string identifier, asc_wall; double height, width;
            infile >> identifier >> height >> width >> asc_wall;
            
            Windows window_input(identifier, height, width, asc_wall);

            append_Window(windows_arr, window_input, num_windows);

        } else if (obj_type == "Door") {

            std::string identifier, asc_wall; double height, width;
            infile >> identifier >> height >> width >> asc_wall;
            
            Doors door_input(identifier, height, width, asc_wall);

            append_Door(doors_arr, door_input, num_doors);

        } else if (obj_type == "Brick") {

            std::string identifier; double height, width, thickness;
            infile >> identifier >> height >> width >> thickness;
            
            Bricks brick_input(identifier, height, width, thickness);

            append_Door(doors_arr, brick_input, num_bricks_types);

        } else {    
            std::string rm_line; //Invalid object. Discarded
            getline(infile, rm_line);
        }
    }

    infile.close();
}

void House::append_Wall(Walls*& walls_arr, Walls new_wall, int& num_walls) {
    //Takes in the current walls array, resizes it by dynamically allocating
    //a new array of size num_walls + 1, copies the old array to the new one,
    //appends the new wall to the end, deletes the old array, and updates
    //the pointer and num_walls variable
    
    //Fix the the other functions so they match this

    //Have to fix the initial assigning of the walls here, otherwise 
    //there would be a segmentation fault, because the walls_arr 
    //doesn't have a size
    if (!walls_arr) return; //do it so it assigns it an address and pushes the wall in it
    //no need to create a new array or any resizing so return after this


    Walls* new_arr = new Walls[num_walls+1];
    for (int i=0; i<num_walls; i++) {
        new_arr[i] = walls_arr[i]
    }

    //works
    new_arr[num_walls] = new_wall;

    //works
    delete[] walls_arr;
    walls_arr = new_arr;
    num_walls++;
}

void House::append_Window(Windows*& windows_arr, Windows new_window, int& num_windows) {
    //Same as append_Wall but for windows
    Windows* new_arr = new Windows[num_windows+1];
    for (int i=0; i<num_windows; i++) {
        new_arr[i] = windows_arr[i]
    }

    //works
    new_arr[num_windows] = new_window;

    //works
    delete[] windows_arr;
    windows_arr = new_arr;
    num_windows++;
}

void House::append_Door(Doors* doors_arr, Doors new_door, int& num_doors) {
    //Same as append_Wall but for doors
    Doors* new_arr = new Doors[num_doors+1];
    for (int i=0; i<num_doors; i++) {
        new_arr[i] = doors_arr[i]
    }

    //have to see if this works
    new_arr[num_doors] = new_door;

    //have to see if this works
    doors_arr = new_arr
    delete[] new_arr;
}

void House::append_Brick(Bricks* bricks_arr, Bricks new_brick, int& num_bricks_types) {
    //Same as append_Wall but for bricks
    Bricks* new_arr = new Bricks[num_bricks_types+1];
    for (int i=0; i<num_bricks_types; i++) {
        new_arr[i] = bricks_arr[i]
    }

    //have to see if this works
    new_arr[num_bricks_types] = new_brick;

    //have to see if this works
    bricks_arr = new_arr
    delete[] new_arr;
}

void House::displayHouseData() {
    //Displays a summary of the house plan, and a final verdict on the bricks required
}

void House::writeHouseData(std::string filename) {
    std::ofstream outfile(filename);
    if (!outfile) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    //Writes the formatted data to the file
    //Writes each attribute of the house in a structured format

    outfile.close();
}

void House::set_name() {
    std::cout << "Enter the owner's name: "; std::cin >> owner_name;
}
