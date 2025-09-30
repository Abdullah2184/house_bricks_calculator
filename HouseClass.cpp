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
    void append_Wall(Walls new_wall);
    void append_Window(Windows new_window);
    void append_Door(Doors new_door);
    void append_Brick(Bricks new_brick);
    void displayHouseData();
    void writeHouseData();
    void set_name();
    void calculate_bricks();
};

House::House() {
    owner_name = "";
    bricks_arr = nullptr;
    num_bricks_types = 0;
    //num_bricks_req = 0;
    walls_arr = nullptr;
    num_walls = 0;
    windows_arr = nullptr;
    num_windows = 0;
    doors_arr = nullptr;
    num_doors = 0;
    house_data = "";
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
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::cout << "File opened successfully"; //DEBUGGING
    //Write the code to read the formatted data from the file
    //Reads word by word for each line, then based on the
    //first word, it assigns the rest of the line to the appropriate variable
    std::string header;
    std::string obj_type;
    getline(infile, header); //removes the column lines from the filestream buffer
    int runs = 0; //DEBUGGING
    while(infile >> obj_type) {
        //infile >> obj_type; //Takes the first column to identify the type of object
        std::cout << "Object type read: " << obj_type << "\n"; //DEBUGGING
        std::cout << ++runs << "\n"; //DEBUGGING
        //Performs specific instructions as per object type
        if (obj_type == "Wall") {
            
            //Read all the attributes of the object
            std::string identifier, brick_type; 
            double height, width, thickness;
            int has_window, has_door;
            infile >> identifier >> height >> width >> thickness
            >> brick_type >> has_window >> has_door;
            
            //Construct the window object using class constructor
            Walls wall_input(identifier, height, width, thickness, brick_type, has_window, has_door);
            
            //Append the new object to the dynamic object array
            append_Wall(wall_input);

        } else if (obj_type == "Window") {

            std::string identifier, asc_wall; double height, width;
            infile >> identifier >> height >> width >> asc_wall;
            
            Windows window_input(identifier, height, width, asc_wall);

            append_Window(window_input);

        } else if (obj_type == "Door") {

            std::string identifier, asc_wall; double height, width;
            infile >> identifier >> height >> width >> asc_wall;
            
            Doors door_input(identifier, height, width, asc_wall);

            append_Door(door_input);

        } else if (obj_type == "Brick") {

            std::string identifier; double height, width, thickness;
            infile >> identifier >> height >> width >> thickness;
            
            Bricks brick_input(identifier, height, width, thickness);

            append_Brick(brick_input);

        } else {    
            std::string rm_line; //Invalid object. Discarded
            getline(infile, rm_line);
        }
        std::cout << "Stream good? " << infile.good() << "\n";
        std::cout << "Stream fail? " << infile.fail() << "\n";
        std::cout << "Stream eof? " << infile.eof() << "\n";
    }

    infile.close();
}

void House::append_Wall(Walls new_wall) {
    //Takes in the current walls array, resizes it by dynamically allocating
    //a new array of size num_walls + 1, copies the old array to the new one,
    //appends the new wall to the end, deletes the old array, and updates
    //the pointer and num_walls variable
    
    //If the pointer array hasn't been created (first append call): 
    if (!walls_arr) {
        walls_arr = new Walls[1];
        walls_arr[0] = new_wall;
        num_walls = 1;
        return;
    }


    Walls* new_arr = new Walls[num_walls+1];
    for (int i=0; i<num_walls; i++) {
        new_arr[i] = walls_arr[i];
    }

    //works
    new_arr[num_walls] = new_wall;

    //works
    delete[] walls_arr;
    walls_arr = new_arr;
    num_walls++;
    std::cout << "Appending Wall, identifier\n"; //DEBUGGING

}

void House::append_Window(Windows new_window) {
    //Same as append_Wall but for windows
    if (!windows_arr) {
        windows_arr = new Windows[1];
        windows_arr[0] = new_window;
        num_windows = 1;
        return;
    }

    Windows* new_arr = new Windows[num_windows+1];
    for (int i=0; i<num_windows; i++) {
        new_arr[i] = windows_arr[i];
    }

    //works
    new_arr[num_windows] = new_window;

    //works
    delete[] windows_arr;
    windows_arr = new_arr;
    num_windows++;
    std::cout << "Appending Window, identifier\n"; //DEBUGGING
}

void House::append_Door(Doors new_door) {
    if (!doors_arr) {
        doors_arr = new Doors[1];
        doors_arr[0] = new_door;
        num_doors = 1;
        return;
    }
    
    //Same as append_Wall but for doors
    Doors* new_arr = new Doors[num_doors+1];
    for (int i=0; i<num_doors; i++) {
        new_arr[i] = doors_arr[i];
    }

    //works
    new_arr[num_doors] = new_door;

    //works
    delete[] doors_arr;
    doors_arr = new_arr;
    num_doors++;
    std::cout << "Appending Door, identifier\n"; //DEBUGGING
}

void House::append_Brick(Bricks new_brick) {
    //Same as append_Wall but for bricks

    if (!doors_arr) {
        bricks_arr = new Bricks[1];
        bricks_arr[0] = new_brick;
        num_windows = 1;
        return;
    }

    Bricks* new_arr = new Bricks[num_bricks_types+1];
    for (int i=0; i<num_bricks_types; i++) {
        new_arr[i] = bricks_arr[i];
    }

    //works
    new_arr[num_bricks_types] = new_brick;

    //works
    delete[] bricks_arr;
    bricks_arr = new_arr;
    std::cout << ++num_bricks_types;

    std::cout << "Appending Brick, identifier\n"; //DEBUGGING
}

void House::displayHouseData() {
    //Displays a summary of the house plan, and a final verdict on the bricks required
    std::cout << "Going to print the data now:\n"; //DEBUGGING
    std::cout << house_data << "\n"; 
}

void House::writeHouseData() {
    //Take the filename as input
    std::string filename;
    std::cout << "Enter the filename: ";
    std::cin >> filename;
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

//pseudo-code to implement logic, then will name everything properly 
void House::calculate_bricks() {
    std::cout << "Entered the function: "<< num_bricks_types << "\n"; //DEBUGGING
    //Loops through the bricks
    for (int i=0; i<num_bricks_types; i++) {
        //Loops through the walls
        int num_bricks = 0;
        std::cout << "Calculating for brick type: " << bricks_arr[i].get_type() << "\n"; //DEBUGGING
        double total_volume = 0;
        std::cout << "Number of walls:  " << num_walls << "\n"; //DEBUGGING 
        for (int j=0; j<num_walls;j++) {
            //If the types match, add to the number of bricks of this type required
            std::cout << "Calculating for wall: " << walls_arr[j].get_identifier() << " | " << walls_arr[j].get_brick_type() << "\n";
            if (bricks_arr[i].get_type() != walls_arr[j].get_brick_type()) {
                std::cout << "Number of bricks for this: " << num_bricks << "\n"; //DEBUGGING
                continue;
            }
            
            std::cout << "Wall matched: " << walls_arr[j].get_identifier() << "\n"; //DEBUGGING
            //have to round upwards, not downwards (default)
            //Calculate the volume of the wall and add it to the total volume
            total_volume += walls_arr[j].get_volume();
            std::cout << "Volume now from the wall: " << total_volume << "\n"; //DEBUGGING

            //If the wall has a window or door, find it by looping,
            // through the array and calculate dimensions
            //and subtract that volume frrom the volume
            if (walls_arr[j].has_window) {
                std::cout << "Wall has window(s): " << walls_arr[j].get_identifier() << "\n"; //DEBUGGING
                for (int k=0; k < num_windows; k++) {
                    if (walls_arr[j].get_identifier() == windows_arr[k].get_asc_wall()) {
                        //multiples the area of the window with the thickness of the wall
                        //to get a proper estimate of the volume subtracted by the window
                        total_volume -= ( (windows_arr[k].get_area())*(walls_arr[j].get_thickness()) );
                        std::cout << "Volume now after subtracting windows: " << total_volume << "\n"; //DEBUGGING
                    }
                }
            }
            if (walls_arr[j].has_door) {
                for (int k=0; k<num_doors; k++) {
                    if (walls_arr[j].get_identifier() == doors_arr[k].get_asc_wall()) {
                        //multiples the area of the window with the thickness of the wall
                        //to get a proper estimate of the volume subtracted by the window
                        total_volume -= ( (doors_arr[k].get_area())*(walls_arr[j].get_thickness()) );
                        std::cout << "Volume now after subtracting doors: " << total_volume << "\n"; //DEBUGGING
                    }
                }
            }
            //Now calculates the number of this type of bricks required
            //based on the total volume of wall and volume of brick
            //and passes it to the specific brick class
            std::cout << "Total volume: " << total_volume << " | " << "Brick volume: " << bricks_arr[i].get_volume() << "\n";
            num_bricks += static_cast<int>(total_volume / bricks_arr[i].get_volume());
            std::cout << "Number of bricks for this: " << num_bricks << "\n"; //DEBUGGING
            bricks_arr[i].update_num_req(num_bricks);
        }
        house_data += "The number of " + bricks_arr[i].get_type() + " bricks required is : "; 
        house_data += std::to_string(bricks_arr[i].get_num_req()) + "\n";
        std::cout << "House info now: " << house_data << "\n";
    }
    std::cout << "FINISHED CALCULATING\n"; //DEBUGGING
}

