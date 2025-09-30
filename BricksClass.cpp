class Bricks {
    private:
        std::string type;
        int num_bricks_req = 0;
        double height;
        double width;
        double thickness;

    public:
        Bricks() = default;
        Bricks(std::string i_identifier, double i_height, double i_width, double i_thickness) {
            type = i_identifier;
            height = i_height;
            width = i_width;
            thickness = i_thickness;
        }
        std::string get_type() {
            return type;
        }
        void set_num_req(int num_req) {
            num_bricks_req = num_req;
        }
        int get_num_req() {
            return num_bricks_req;
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
        double get_volume() {
            return thickness * width * height;
        }

};