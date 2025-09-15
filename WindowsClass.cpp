#include <string>

class Windows {
    private:
        std::string identifier;
        double height;
        double width;
        std::string associated_wall;

    public:
    Windows() {
        identifier = "";
        height = 0;
        width = 0;
        associated_wall = "";
    }
    Windows(std::string id, double h, double w, std::string asc_wall) {
        identifier = id;
        height = h;
        width = w;
        associated_wall = asc_wall;
    }
    std::string get_identifier() {
        return identifier;
    }
    double get_width() {
        return height;
    }
    double get_height() {
        return height;
    }
    std::string get_asc_wall() {
        return associated_wall;
    }
    double get_area() {
        return height * width;
    }
};