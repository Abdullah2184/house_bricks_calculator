#ifndef WALLSCLASS_H
#define WALLSCLASS_H
#include <string>

class Walls {
private:
    std::string identifier;
    double height{0.0};
    double width{0.0};
    double thickness{0.0};
    std::string brick_type;

public:
    // exposed flags used by calculation logic
    bool has_window{false};
    bool has_door{false};

    Walls() = default;
    Walls(const std::string &id, double h, double w, double thick, const std::string &b_type, bool window, bool door)
        : identifier(id), height(h), width(w), thickness(thick), brick_type(b_type), has_window(window), has_door(door) {}

    const std::string &get_identifier() const { return identifier; }
    double get_width() const { return width; }
    double get_thickness() const { return thickness; }
    double get_height() const { return height; }
    double get_volume() const { return height * width * thickness; }
    const std::string &get_brick_type() const { return brick_type; }
};

#endif