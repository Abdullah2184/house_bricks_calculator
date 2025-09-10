class Bricks {
    private:
        string type;
        double height;
        double width;
        double thickness

    public:
        string get_type() {
            return type;
        }
        double get_height() {
            return height;
        }
        double get_width() {
            return width;
        }
        double get_thickness() {
            return thickness;
        }
        double get_dimensions() {
            return thickness * width * height;
        }

};