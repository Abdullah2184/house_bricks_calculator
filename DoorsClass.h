#ifndef DOORSCLASS_H //Header Guard
#define DOORSCLASS_H

#include <string>

class Doors {
private:
    std::string identifier;
    double height{0.0};
    double width{0.0};
    std::string associated_wall;

public:
    Doors() = default;
    Doors(const std::string &i_identifier, double i_height, double i_width, const std::string &i_asc_wall)
        : identifier(i_identifier), height(i_height), width(i_width), associated_wall(i_asc_wall) {}

    const std::string &get_identifier() const { return identifier; }
    double get_width() const { return width; }
    double get_height() const { return height; }
    const std::string &get_asc_wall() const { return associated_wall; }
    double get_area() const { return height * width; }
};

#endif