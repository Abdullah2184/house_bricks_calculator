#ifndef DOORSCLASS_H
#define DOORSCLASS_H

#include <string>

class Doors {
private:
    std::string id;
    double height{0.0};   // metres
    double width{0.0};    // metres
    std::string wall_id;  // associated wall identifier

public:
    Doors() = default;

    Doors(const std::string &i_id,
          double i_height,
          double i_width,
          const std::string &i_wall_id)
        : id(i_id),
          height(i_height),
          width(i_width),
          wall_id(i_wall_id) {}

    const std::string &get_id() const { return id; }
    const std::string &get_wall_id() const { return wall_id; }

    double get_height() const { return height; }
    double get_width() const { return width; }

    double get_area() const { return height * width; }
};

#endif
