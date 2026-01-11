#ifndef BRICKSCLASS_H
#define BRICKSCLASS_H

#include <string>
#include <unordered_map>
#include <stdexcept>

class Bricks {
private:
    std::string id;

    // Physical size (reference only, not used for counting)
    double length{0.0};    // metres
    double height{0.0};    // metres
    double width{0.0};     // metres

    // Industry rates
    std::unordered_map<double, double> bricks_per_m2; 
    // key   = wall thickness (m)
    // value = bricks per square metre

public:
    double unit_cost{0.0};                     // cost per brick
    double mortar_m3_per_1000_bricks{0.0};     // mortar demand

    Bricks() = default;

    Bricks(const std::string &i_id,
           double i_length,
           double i_height,
           double i_width)
        : id(i_id),
          length(i_length),
          height(i_height),
          width(i_width) {}

    const std::string &get_id() const { return id; }

    // Add rate per wall thickness
    void add_bricks_per_m2(double wall_thickness, double rate) {
        bricks_per_m2[wall_thickness] = rate;
    }

    // Lookup rate (throws if undefined – intentional safety)
    double get_bricks_per_m2(double wall_thickness) const {
        auto it = bricks_per_m2.find(wall_thickness);
        if (it == bricks_per_m2.end()) {
            throw std::runtime_error(
                "No bricks_per_m2 defined for thickness " +
                std::to_string(wall_thickness));
        }
        return it->second;
    }

    // Reference dimensions
    double get_length() const { return length; }
    double get_height() const { return height; }
    double get_width() const { return width; }
};

#endif
