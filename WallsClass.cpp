#include <string>

class Walls {
    private:
    std::string identifier;
    double height;
    double width;
    double thickness;
    std::string brick_type;

    public:

    //public because not an issue
    bool has_window; //have to include in the constructor and any other required functions
    bool has_door;
    Walls() {
        //default constructor
    }
    Walls(std::string id, double h, double w, double thick, std::string b_type, bool window, bool door) {
        identifier = id;
        height = h;
        width = w;
        thickness = thick;
        brick_type = b_type;
        has_window = window;
        has_door = door;
    }

    std::string get_identifier() {
        return identifier;
    }
    double get_width() {
        return height;
    }
    double get_thickness() {
        return height;
    }
    double get_height() {
        return height;
    }
    double get_volume() {
        return height*width*thickness;
    }
    std::string get_brick_type() {
        return brick_type;
    }
};