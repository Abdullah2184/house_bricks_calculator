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

    Walls(std::string id, double h, double w, double thick, std::string b_type) {
        identifier = id;
        height = h;
        width = w;
        thickness = thick;
        brick_type = b_type;
    }

    std::string get_identifier() {
        
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
};