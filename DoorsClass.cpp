class Doors {
    private:
        std::string identifier;
        double height;
        double width;
        std::string associated_wall;

    public:
    Doors() = default;
    Doors(std::string i_identifier, double i_height, double i_width, std::string i_asc_wall) {
        identifier = i_identifier;
        height = i_height;
        width = i_width;
        associated_wall = i_asc_wall;
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