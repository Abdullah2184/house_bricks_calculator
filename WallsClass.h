#ifndef WALLSCLASS_H
#define WALLSCLASS_H

#include <string>

class Walls {
private:
    std::string id;
    double height{0.0};     // metres
    double width{0.0};      // metres
    double thickness{0.0};  // metres (e.g. 0.1025, 0.20)
    std::string brick_type; // references Bricks::id

public:
    Walls() = default;

    Walls(const std::string &i_id,
          double i_height,
          double i_width,
          double i_thickness,
          const std::string &i_brick_type)
        : id(i_id),
          height(i_height),
          width(i_width),
          thickness(i_thickness),
          brick_type(i_brick_type) {}

    const std::string &get_id() const { return id; }
    const std::string &get_brick_type() const { return brick_type; }

    double get_height() const { return height; }
    double get_width() const { return width; }
    double get_thickness() const { return thickness; }

    // Masonry is area-based
    double get_area() const { return height * width; }
};

#endif
