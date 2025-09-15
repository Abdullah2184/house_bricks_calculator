class Bricks {
    private:
        std::string type;
        int num_bricks_req;
        double height;
        double width;
        double thickness;

    public:
        std::string get_type() {
            return type;
        }
        void set_num_req(int num_req) {
            num_bricks_req = num_req
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