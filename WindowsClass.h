#ifndef WINDOWSCLASS_H // Header Guard
#define WINDOWSCLASS_H
#include <string>

class Windows {
private:
    std::string identifier;
    double height{0.0};
    double width{0.0};
    std::string associated_wall;

public:
    Windows() = default;
    Windows(const std::string &id, double h, double w, const std::string &asc_wall)
        : identifier(id), height(h), width(w), associated_wall(asc_wall) {}

    const std::string &get_identifier() const { return identifier; }
    double get_width() const { return width; }
    double get_height() const { return height; }
    const std::string &get_asc_wall() const { return associated_wall; }
    double get_area() const { return height * width; }
};

#endif