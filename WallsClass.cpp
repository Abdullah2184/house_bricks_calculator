class Walls {
    private:
    string identifier;
    double height;
    double width;
    double thickness;
    int brick_type;

    public:

    string get_identifier() {
        
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