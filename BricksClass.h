#include <string>

class Bricks {
private:
    std::string type;
    int num_bricks_req{0};
    double height{0.0};
    double width{0.0};
    double thickness{0.0};

public:
    Bricks() = default;
    Bricks(const std::string &i_identifier, double i_height, double i_width, double i_thickness)
        : type(i_identifier), num_bricks_req(0), height(i_height), width(i_width), thickness(i_thickness) {}

    const std::string &get_type() const { return type; }
    void set_num_req(int num_req) { num_bricks_req = num_req; }
    int get_num_req() const { return num_bricks_req; }
    double get_height() const { return height; }
    double get_width() const { return width; }
    double get_thickness() const { return thickness; }
    double get_volume() const { return thickness * width * height; }
};