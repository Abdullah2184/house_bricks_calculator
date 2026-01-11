#include "HouseClass.h"
#define VALIDATION_ERROR(msg) \
    do { std::cerr << "Validation error: " << msg << std::endl; return false; } while (0)

House::House() {
    owner_name = "";
    // vectors are default-initialized empty
    //num_bricks_req = 0;
    // vectors are default-initialized empty
    house_data = "";
}

House::~House() {
    // vectors cleanup automatically
}

/*
void House::readHouseData(std::string filename) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::cout << "File opened successfully"; //DEBUGGING
    //Write the code to read the formatted data from the file
    //Reads word by word for each line, then based on the
    //first word, it assigns the rest of the line to the appropriate variable
    std::string header;
    std::string obj_type;
    getline(infile, header); //removes the column lines from the filestream buffer
    int runs = 0; //DEBUGGING
    while(infile >> obj_type) {
        //infile >> obj_type; //Takes the first column to identify the type of object
        std::cout << "Object type read: " << obj_type << "\n"; //DEBUGGING
        std::cout << ++runs << "\n"; //DEBUGGING
        //Performs specific instructions as per object type
        if (obj_type == "Wall") {
            
            //Read all the attributes of the object
            std::string identifier, brick_type; 
            double height, width, thickness;
            int has_window, has_door;
            infile >> identifier >> height >> width >> thickness
            >> brick_type >> has_window >> has_door;
            
            //Construct the window object using class constructor
            Walls wall_input(identifier, height, width, thickness, brick_type, has_window, has_door);
            
            //Append the new object to the dynamic object array
            append_Wall(wall_input);

        } else if (obj_type == "Window") {

            std::string identifier, asc_wall; double height, width;
            infile >> identifier >> height >> width >> asc_wall;
            
            Windows window_input(identifier, height, width, asc_wall);

            append_Window(window_input);

        } else if (obj_type == "Door") {

            std::string identifier, asc_wall; double height, width;
            infile >> identifier >> height >> width >> asc_wall;
            
            Doors door_input(identifier, height, width, asc_wall);

            append_Door(door_input);

        } else if (obj_type == "Brick") {

            std::string identifier; double height, width, thickness;
            infile >> identifier >> height >> width >> thickness;
            
            Bricks brick_input(identifier, height, width, thickness);

            append_Brick(brick_input);

        } else {    
            std::string rm_line; //Invalid object. Discarded
            getline(infile, rm_line);
        }
        std::cout << "Stream good? " << infile.good() << "\n";
        std::cout << "Stream fail? " << infile.fail() << "\n";
        std::cout << "Stream eof? " << infile.eof() << "\n";
    }

    infile.close();
}
*/

// Read house description from a YAML file. Expected structure:
// owner: Name
// walls: [ {id, height, width, thickness, brick_type, windows, doors}, ... ]
// windows: [ {id, height, width, wall}, ... ]
// doors: [ {id, height, width, wall}, ... ]
// bricks: [ {id, height, width, thickness}, ... ]
bool House::readHouseYaml(const std::string &filename) {
    try {
        YAML::Node root = YAML::LoadFile(filename);

        if (!root["owner"])
            VALIDATION_ERROR("Missing owner field");

        owner_name = root["owner"].as<std::string>();

        walls_arr.clear();
        windows_arr.clear();
        doors_arr.clear();
        bricks_arr.clear();

        /* ---- Walls ---- */
        if (!root["walls"])
            VALIDATION_ERROR("No walls defined");

        for (const auto &node : root["walls"]) {
            std::string id = node["id"].as<std::string>();
            double h = node["height"].as<double>();
            double w = node["width"].as<double>();
            double t = node["thickness"].as<double>();
            std::string btype = node["brick_type"].as<std::string>();

            if (h <= 0 || w <= 0 || t <= 0)
                VALIDATION_ERROR("Wall " + id + " has non-positive dimensions");

            append_Wall(Walls(id, h, w, t, btype));
        }

        /* ---- Windows ---- */
        if (root["windows"]) {
            for (const auto &node : root["windows"]) {
                std::string id = node["id"].as<std::string>();
                double h = node["height"].as<double>();
                double w = node["width"].as<double>();
                std::string wall = node["wall"].as<std::string>();

                if (h <= 0 || w <= 0)
                    VALIDATION_ERROR("Window " + id + " has invalid dimensions");

                bool found = false;
                for (const auto &wl : walls_arr)
                    if (wl.get_id() == wall) found = true;

                if (!found)
                    VALIDATION_ERROR("Window " + id + " references unknown wall " + wall);

                append_Window(Windows(id, h, w, wall));
            }
        }

        /* ---- Doors ---- */
        if (root["doors"]) {
            for (const auto &node : root["doors"]) {
                std::string id = node["id"].as<std::string>();
                double h = node["height"].as<double>();
                double w = node["width"].as<double>();
                std::string wall = node["wall"].as<std::string>();

                if (h <= 0 || w <= 0)
                    VALIDATION_ERROR("Door " + id + " has invalid dimensions");

                bool found = false;
                for (const auto &wl : walls_arr)
                    if (wl.get_id() == wall) found = true;

                if (!found)
                    VALIDATION_ERROR("Door " + id + " references unknown wall " + wall);

                append_Door(Doors(id, h, w, wall));
            }
        }

    } catch (const YAML::Exception &e) {
        std::cerr << "YAML parse error: " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool House::readBricksYAML(const std::string &filename) {
    try {
        YAML::Node root = YAML::LoadFile(filename);
            if (!root["bricks"])
                VALIDATION_ERROR("No bricks defined");

            for (const auto &node : root["bricks"]) {
                std::string id = node["id"].as<std::string>();

                auto size = node["size"];
                double len = size["length"].as<double>();
                double h = size["height"].as<double>();
                double w = size["width"].as<double>();

                Bricks brick(id, len, h, w);

                brick.unit_cost = node["unit_cost"].as<double>();
                brick.mortar_m3_per_1000_bricks =
                    node["mortar_m3_per_1000_bricks"].as<double>();

                if (brick.mortar_m3_per_1000_bricks <= 0)
                    VALIDATION_ERROR("Brick " + id + " has invalid mortar rate");

                auto rates = node["bricks_per_m2"];
                for (auto it = rates.begin(); it != rates.end(); ++it) {
                    double thickness = std::stod(it->first.as<std::string>());
                    double rate = it->second.as<double>();

                    if (rate <= 0)
                        VALIDATION_ERROR("Invalid bricks_per_m2 rate for brick " + id);

                    brick.add_bricks_per_m2(thickness, rate);
                }
                append_Brick(brick);
            }
        } 
        catch (const YAML::Exception &e) {
            std::cerr << "YAML parse error: " << e.what() << std::endl;
            return false;
        }
    return true;
}

// Read materials and cost parameters from YAML.
// Expected fields: prices.brick_cost (per piece), prices.cement_cost_per_5kg, prices.water_cost_per_litre
// parameters.cement_kg_per_1000_bricks, parameters.water_l_per_kg_cement
bool House::readMaterialsYaml(const std::string &filename) { 
    try {
        YAML::Node root = YAML::LoadFile(filename);

        auto cement = root["materials"]["cement"];
        auto water  = root["materials"]["water"];
        auto sand   = root["materials"]["sand"];
        auto waste  = root["materials"]["waste"];

        // Cement
        materials.cement_bag_kg     = cement["bag_size_kg"].as<double>();
        materials.cement_cost_per_bag = cement["cost_per_bag"].as<double>();
        materials.cement_kg_per_m3_mortar = cement["kg_per_m3_mortar"].as<double>();

        // Water
        materials.water_l_per_kg_cement = water["litres_per_kg_cement"].as<double>();
        materials.water_cost_per_litre = water["cost_per_litre"].as<double>();

        // Sand
        materials.sand_m3_per_m3_mortar = sand["m3_per_m3_mortar"].as<double>();
        materials.sand_density_kg_per_m3 = sand["density_kg_per_m3"].as<double>();
        materials.sand_cost_per_tonne = sand["cost_per_tonne"].as<double>();

        // Waste factors
        double bricks_waste = waste["bricks"].as<double>();
        double cement_waste = waste["cement"].as<double>();
        double sand_waste   = waste["sand"].as<double>();
        double water_waste  = waste["water"].as<double>();

        // Validate ranges
        auto validate = [](double v, const char* name) {
            if (v < 0.0 || v > 0.25)
                throw std::runtime_error(std::string("Invalid waste factor: ") + name);
        };
        validate(bricks_waste, "bricks");
        validate(cement_waste, "cement");
        validate(sand_waste, "sand");
        validate(water_waste, "water");

        // Assign default / fallback if needed (could also store all individually)
        materials.waste_factor = cement_waste; // for backward compatibility

    } catch (const YAML::Exception &e) {
        std::cerr << "YAML parse error (materials): " << e.what() << std::endl;
        return false;
    } catch (const std::runtime_error &e) {
        std::cerr << "Validation error (materials): " << e.what() << std::endl;
        return false;
    }
    return true;
}



void House::append_Wall(Walls new_wall) {
    // Push the new wall into the container. Simple and exception-safe.
    walls_arr.push_back(new_wall);
    std::cout << "Appending Wall: " << new_wall.get_id() << "\n"; // DEBUG
}

void House::append_Window(Windows new_window) {
    windows_arr.push_back(new_window);
    std::cout << "Appending Window: " << new_window.get_id() << "\n"; // DEBUG
}

void House::append_Door(Doors new_door) {
    doors_arr.push_back(new_door);
    std::cout << "Appending Door: " << new_door.get_id() << "\n"; // DEBUG
}

void House::append_Brick(Bricks new_brick) {
    bricks_arr.push_back(new_brick);
    std::cout << "Appending Brick: " << new_brick.get_id() << "\n"; // DEBUG
}
    

// Calculate material requirements from previously calculated brick counts.
// This method fills `summary` with per-brick counts, total bricks and derived material quantities and costs.
void House::calculate_materials() {
    // Validate inputs
    if (materials.cement_kg_per_m3_mortar <= 0.0) {
        std::cerr << "Error: cement_kg_per_m3_mortar must be > 0\n";
        return;
    }
    if (materials.sand_m3_per_m3_mortar <= 0.0) {
        std::cerr << "Error: sand_m3_per_m3_mortar must be > 0\n";
        return;
    }
    if (materials.sand_density_kg_per_m3 <= 0.0) {
        std::cerr << "Error: sand_density_kg_per_m3 must be > 0\n";
        return;
    }
    if (materials.cement_bag_kg <= 0.0) { // add this field to Materials struct
        std::cerr << "Error: materials.cement_bag_kg must be > 0\n";
        return;
    }

    summary.mortar_m3_base = 0.0;

    for (const auto &brick_est : summary.bricks) {
        const Bricks *brick = find_brick_by_id(brick_est.type);
        if (!brick) {
            std::cerr << "Error: unknown brick type '" << brick_est.type << "'\n";
            continue; // or return / throw
        }
        double mortar_m3 = (static_cast<double>(brick_est.num) / 1000.0) * brick->mortar_m3_per_1000_bricks;
        summary.mortar_m3_base += mortar_m3;
    }

    summary.mortar_m3_with_waste = summary.mortar_m3_base * (1.0 + materials.waste_factor);

    // Cement
    summary.cement_kg_base = summary.mortar_m3_base * materials.cement_kg_per_m3_mortar;
    summary.cement_kg_final = summary.cement_kg_base * (1.0 + materials.waste_factor);

    summary.cement_bags = static_cast<int>(std::ceil(summary.cement_kg_final / materials.cement_bag_kg));
    summary.cement_cost = summary.cement_bags * materials.cement_cost_per_bag; // materials fields: cement_bag_kg, cement_cost_per_bag

    // Water
    summary.water_l = summary.cement_kg_final * materials.water_l_per_kg_cement;
    summary.water_cost = summary.water_l * materials.water_cost_per_litre;

    // Sand
    summary.sand_m3 = summary.mortar_m3_with_waste * materials.sand_m3_per_m3_mortar;
    summary.sand_kg = summary.sand_m3 * materials.sand_density_kg_per_m3;
    summary.sand_tonnes = summary.sand_kg / 1000.0;
    summary.sand_cost = summary.sand_tonnes * materials.sand_cost_per_tonne;

    // Total cost
    summary.total_cost = summary.cement_cost + summary.water_cost + summary.sand_cost;
    for (const auto &b : summary.bricks) summary.total_cost += b.cost;
}



bool House::writeOutputYaml(const std::string &outfile) {
    // Write a pre-computed ResourceSummary to YAML. It does NOT perform calculations.
    if (summary.total_bricks == 0 && summary.bricks.empty()) {
        std::cerr << "Error: resource summary is empty. Run calculate_bricks() and calculate_materials() before writing output." << std::endl;
        return false;
    }

    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "owner" << YAML::Value << owner_name;
    out << YAML::Key << "resources" << YAML::Value << YAML::BeginMap;

    // Bricks
    out << YAML::Key << "bricks" << YAML::Value << YAML::BeginSeq;
    for (const auto &be : summary.bricks) {
        out << YAML::BeginMap;
        out << YAML::Key << "type" << YAML::Value << be.type;
        out << YAML::Key << "num" << YAML::Value << be.num;
        out << YAML::Key << "unit" << YAML::Value << "piece";
        out << YAML::Key << "unit_cost" << YAML::Value << be.unit_cost;
        out << YAML::Key << "cost" << YAML::Value << be.cost;
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;

    // Cement
    out << YAML::Key << "cement" << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "kg" << YAML::Value << summary.cement_kg_final;
    out << YAML::Key << "kg_base" << YAML::Value << summary.cement_kg_base;
    out << YAML::Key << "unit" << YAML::Value << "5kg_bag";
    out << YAML::Key << "bags_5kg" << YAML::Value << materials.cement_bag_kg;
    out << YAML::Key << "unit_cost" << YAML::Value << materials.cement_cost_per_bag;
    out << YAML::Key << "cost" << YAML::Value << summary.cement_cost;
    out << YAML::EndMap;

    // Water
    out << YAML::Key << "water" << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "litres" << YAML::Value << summary.water_l;
    out << YAML::Key << "unit" << YAML::Value << "litre";
    out << YAML::Key << "unit_cost" << YAML::Value << materials.water_cost_per_litre;
    out << YAML::Key << "cost" << YAML::Value << summary.water_cost;
    out << YAML::EndMap;

    // Mortar
    out << YAML::Key << "mortar" << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "m3_base" << YAML::Value << summary.mortar_m3_base;
    out << YAML::Key << "m3_with_waste" << YAML::Value << summary.mortar_m3_with_waste;
    out << YAML::EndMap;

    // Sand
    out << YAML::Key << "sand" << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "m3" << YAML::Value << summary.sand_m3;
    out << YAML::Key << "kg" << YAML::Value << summary.sand_kg;
    out << YAML::Key << "tonnes" << YAML::Value << summary.sand_tonnes;
    out << YAML::Key << "unit_cost_per_tonne" << YAML::Value << materials.sand_cost_per_tonne;
    out << YAML::Key << "cost" << YAML::Value << summary.sand_cost;
    out << YAML::EndMap;

    out << YAML::Key << "total_cost" << YAML::Value << summary.total_cost;
    out << YAML::EndMap; // resources
    out << YAML::EndMap; // root

    std::ofstream ofs(outfile);
    if (!ofs) {
        std::cerr << "Error opening output file: " << outfile << std::endl;
        return false;
    }
    ofs << out.c_str();
    ofs.close();
    return true;
}

void House::set_name() {
    std::cout << "Enter the owner's name: "; std::cin >> owner_name;
}


// Helper: find brick pointer by id (member of House)
const Bricks* House::find_brick_by_id(const std::string &id) const {
    for (const auto &b : bricks_arr) {
        if (b.get_id() == id) return &b;
    }
    return nullptr;
}

void House::calculate_bricks() {
    summary.bricks.clear();
    summary.total_bricks = 0;

    // For each brick type, sum bricks per wall that uses that brick
    for (const auto &brick : bricks_arr) {

        int total_bricks_for_type = 0;
        double total_net_area_for_type = 0.0;

        for (const auto &wall : walls_arr) {
            if (wall.get_brick_type() != brick.get_id()) continue;

            double wall_area = wall.get_height() * wall.get_width();

            // Subtract openings that belong to this wall
            for (const auto &win : windows_arr) {
                if (win.get_wall_id() == wall.get_id()) wall_area -= win.get_area();
            }
            for (const auto &door : doors_arr) {
                if (door.get_wall_id() == wall.get_id()) wall_area -= door.get_area();
            }
            if (wall_area <= 0.0) continue;

            total_net_area_for_type += wall_area;

            // Determine bricks per m2 for this wall thickness
            double thickness = wall.get_thickness();
            double bricks_per_m2 = 0.0;
            try {
                bricks_per_m2 = brick.get_bricks_per_m2(thickness);
            } catch (const std::exception &e) {
                std::cerr << "Error: brick type '" << brick.get_id()
                          << "' has no bricks_per_m2 defined for thickness "
                          << thickness << ". " << e.what() << std::endl;
                throw; // or handle gracefully
            }

            // Bricks for this wall (round up per wall)
            int bricks_for_wall = static_cast<int>(std::ceil(wall_area * bricks_per_m2));
            total_bricks_for_type += bricks_for_wall;
        }

        // If no walls used this brick type, skip
        if (total_bricks_for_type == 0) continue;

        BrickEstimate est;
        est.type = brick.get_id();
        est.num = total_bricks_for_type;
        est.unit_cost = brick.unit_cost;
        est.cost = est.num * est.unit_cost;

        summary.bricks.push_back(est);
        summary.total_bricks += est.num;
    }
}


/*
//pseudo-code to implement logic, then will name everything properly 
void House::calculate_bricks() {
    std::cout << "Entered calculate_bricks()\n"; // DEBUGGING
    // For each brick type, accumulate adjusted wall volume (subtract openings) and convert to number of bricks
    for (size_t i = 0; i < bricks_arr.size(); ++i) {
        int num_bricks = 0;
        std::cout << "Calculating for brick type: " << bricks_arr[i].get_id() << "\n"; // DEBUG
        double total_volume = 0.0;
        std::cout << "Number of walls: " << walls_arr.size() << "\n"; // DEBUG

        for (size_t j = 0; j < walls_arr.size(); ++j) {
            std::cout << "Checking wall: " << walls_arr[j].get_id() << " (brick_type=" << walls_arr[j].get_brick_type() << ")\n";
            if (bricks_arr[i].get_id() != walls_arr[j].get_brick_type()) {
                continue;
            }

            // Add wall volume
            double wall_vol = walls_arr[j].get_volume();

            // Subtract windows volume attached to this wall
            if (walls_arr[j].has_window) {
                for (const auto &win : windows_arr) {
                    if (walls_arr[j].get_id() == win.get_asc_wall()) {
                        wall_vol -= (win.get_area() * walls_arr[j].get_thickness());
                    }
                }
            }

            // Subtract doors volume attached to this wall
            if (walls_arr[j].has_door) {
                for (const auto &door : doors_arr) {
                    if (walls_arr[j].get_id() == door.get_asc_wall()) {
                        wall_vol -= (door.get_area() * walls_arr[j].get_thickness());
                    }
                }
            }

            total_volume += wall_vol;
            std::cout << "Accumulated volume after wall " << walls_arr[j].get_id() << ": " << total_volume << "\n";
        }

        // Calculate bricks required for this brick type (round up)
        if (bricks_arr[i].get_volume() <= 0.0) {
            std::cerr << "Error: brick volume for type '" << bricks_arr[i].get_id() << "' is zero or negative.\n";
            bricks_arr[i].set_num_req(0);
            continue;
        }

        num_bricks = static_cast<int>( std::ceil(total_volume / bricks_arr[i].get_volume()) );
        bricks_arr[i].set_num_req(num_bricks);

        house_data += "The number of " + bricks_arr[i].get_id() + " bricks required is : "; 
        house_data += std::to_string(bricks_arr[i].get_num_req()) + "\n";
        std::cout << "House info now: " << house_data << "\n";
    }
    std::cout << "FINISHED CALCULATING\n"; //DEBUGGING
}
*/
