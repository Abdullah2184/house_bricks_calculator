class Windows {
    private:
        string identifier;
        double height;
        double width;
        string associated_wall;

    public:
    string get_identifier() {
        return identifier;
    }
    double get_width() {
        return height;
    }
    double get_height() {
        return height;
    }
    string get_asc_wall() {
        return associated_wall;
    }
    string get_area() {
        return height * width;
    }
};