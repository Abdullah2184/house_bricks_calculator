class House {
    private:
    string owner_name;
    Bricks* brick_arr;
    int num_bricks_types;
    int num_bricks_req;
    Walls* walls_arr;
    int num_walls;
    Windows* windows_arr;
    int num_windows;
    Doors* doors_arr;
    int num_doors;

    public:
    House();
    ~House();
    void readHouseData(string filename);
    void append_Wall(Walls* walls_arr, Walls wall, int& num_walls);
    void append_Window(Windows* windows_arr, Windows window, int& num_windows);
    void append_Door(Doors* door_arr, Doors door, int& num_doors);
    void append_Brick(Bricks* bricks_arr, Bricks brick, int& num_bricks_types);
    void displayHouseData();
    void writeHouseData(string filename);
}

House::House() {
    owner_name = "";
    brick_arr = nullptr;
    num_bricks_req = 0;
    walls_arr = nullptr;
    num_walls = 0;
    windows_arr = nullptr;
    num_windows = 0;
    doors_arr = nullptr;
    num_doors = 0;
}

House::~House() {
    delete[] brick_arr;
    delete[] walls_arr;
    delete[] windows_arr;
    delete[] doors_arr;
}

void House::readHouseData(string filename) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    //Write the code to read the formatted data from the file
    //Reads word by word for each line, then based on the
    //first word, it assigns the rest of the line to the appropriate variable

    infile.close();
}

void append_Wall(Walls* walls_arr, Walls wall, int& num_walls) {
    //Takes in the current walls array, resizes it by dynamically allocating
    //a new array of size num_walls + 1, copies the old array to the new one,
    //appends the new wall to the end, deletes the old array, and updates
    //the pointer and num_walls variable
}

void append_Window(Windows* windows_arr, Windows window, int& num_windows) {
    //Same as append_Wall but for windows
}

void append_Door(Doors* door_arr, Doors door, int& num_doors) {
    //Same as append_Wall but for doors
}

void append_Brick(Bricks* bricks_arr, Bricks brick, int& num_bricks_types) {
    //Same as append_Wall but for bricks
}

void displayHouseData() {
    //Displays a summary of the house plan, and a final verdict on the bricks required
}

void writeHouseData(string filename) {
    ofstream outfile(filename);
    if (!outfile) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    //Writes the formatted data to the file
    //Writes each attribute of the house in a structured format

    outfile.close();
}
