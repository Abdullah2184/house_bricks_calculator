#include "HouseClass.h"


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

// Read house description from a YAML file. Expected structure:
// owner: Name
// walls: [ {id, height, width, thickness, brick_type, windows, doors}, ... ]
// windows: [ {id, height, width, wall}, ... ]
// doors: [ {id, height, width, wall}, ... ]
// bricks: [ {id, height, width, thickness}, ... ]
bool House::readHouseYaml(const std::string &filename) {
    try {
        YAML::Node root = YAML::LoadFile(filename);

        if (root["owner"]) owner_name = root["owner"].as<std::string>();

        // Clear existing arrays (simple deallocation and reset)
        walls_arr.clear();
        windows_arr.clear();
        doors_arr.clear();
        bricks_arr.clear();

        if (root["walls"]) {
            for (auto node : root["walls"]) {
                std::string id = node["id"].as<std::string>();
                double h = node["height"].as<double>();
                double w = node["width"].as<double>();
                double t = node["thickness"].as<double>();
                std::string btype = node["brick_type"].as<std::string>();
                bool has_win = false, has_door = false;
                if (node["windows"]) has_win = node["windows"].as<int>() > 0;
                if (node["doors"]) has_door = node["doors"].as<int>() > 0;
                append_Wall( Walls(id,h,w,t,btype,has_win,has_door) );
            }
        }

        if (root["windows"]) {
            for (auto node : root["windows"]) {
                std::string id = node["id"].as<std::string>();
                double h = node["height"].as<double>();
                double w = node["width"].as<double>();
                std::string wall = node["wall"].as<std::string>();
                // Verify associated wall exists
                    bool found = false;
                    for (const auto &wl : walls_arr) if (wl.get_identifier() == wall) { found = true; break; }
                if (!found) {
                    std::cerr << "Error: window '"<<id<<"' references unknown wall '"<<wall<<"'\n";
                    return false;
                }
                append_Window( Windows(id,h,w,wall) );
            }
        }

        if (root["doors"]) {
            for (auto node : root["doors"]) {
                std::string id = node["id"].as<std::string>();
                double h = node["height"].as<double>();
                double w = node["width"].as<double>();
                std::string wall = node["wall"].as<std::string>();
                bool found = false;
                for (const auto &wl : walls_arr) if (wl.get_identifier() == wall) { found = true; break; }
                if (!found) {
                    std::cerr << "Error: door '"<<id<<"' references unknown wall '"<<wall<<"'\n";
                    return false;
                }
                append_Door( Doors(id,h,w,wall) );
            }
        }

        if (root["bricks"]) {
            for (auto node : root["bricks"]) {
                std::string id = node["id"].as<std::string>();
                double h = node["height"].as<double>();
                double w = node["width"].as<double>();
                double t = node["thickness"].as<double>();
                append_Brick( Bricks(id,h,w,t) );
            }
        }

    } catch (const YAML::Exception &e) {
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
        if (root["prices"]) {
            auto p = root["prices"];
            if (p["brick_cost"]) materials.brick_cost = p["brick_cost"].as<double>();
            if (p["cement_cost_per_5kg"]) materials.cement_cost_per_5kg = p["cement_cost_per_5kg"].as<double>();
            if (p["water_cost_per_litre"]) materials.water_cost_per_litre = p["water_cost_per_litre"].as<double>();
            if (p["sand_cost_per_tonne"]) materials.sand_cost_per_tonne = p["sand_cost_per_tonne"].as<double>();
        }
        if (root["parameters"]) {
            auto p = root["parameters"];
                if (p["cement_kg_per_1000_bricks"]) materials.cement_kg_per_1000_bricks = p["cement_kg_per_1000_bricks"].as<double>();
                if (p["water_l_per_kg_cement"]) materials.water_l_per_kg_cement = p["water_l_per_kg_cement"].as<double>();
                if (p["mortar_m3_per_1000_bricks"]) materials.mortar_m3_per_1000_bricks = p["mortar_m3_per_1000_bricks"].as<double>();
                if (p["cement_kg_per_m3_mortar"]) materials.cement_kg_per_m3_mortar = p["cement_kg_per_m3_mortar"].as<double>();
                if (p["waste_factor"]) materials.waste_factor = p["waste_factor"].as<double>();
            if (p["sand_m3_per_m3_mortar"]) materials.sand_m3_per_m3_mortar = p["sand_m3_per_m3_mortar"].as<double>();
            if (p["sand_density_kg_per_m3"]) materials.sand_density_kg_per_m3 = p["sand_density_kg_per_m3"].as<double>();
        }
    } catch (const YAML::Exception &e) {
        std::cerr << "YAML parse error (materials): " << e.what() << std::endl;
        return false;
    }
    return true;
}

void House::append_Wall(Walls new_wall) {
    // Push the new wall into the container. Simple and exception-safe.
    walls_arr.push_back(new_wall);
    std::cout << "Appending Wall: " << new_wall.get_identifier() << "\n"; // DEBUG
}

void House::append_Window(Windows new_window) {
    windows_arr.push_back(new_window);
    std::cout << "Appending Window: " << new_window.get_identifier() << "\n"; // DEBUG
}

void House::append_Door(Doors new_door) {
    doors_arr.push_back(new_door);
    std::cout << "Appending Door: " << new_door.get_identifier() << "\n"; // DEBUG
}

void House::append_Brick(Bricks new_brick) {
    bricks_arr.push_back(new_brick);
    std::cout << "Appending Brick: " << new_brick.get_type() << "\n"; // DEBUG
}
    

// Calculate material requirements from previously calculated brick counts.
// This method fills `summary` with per-brick counts, total bricks and derived material quantities and costs.
void House::calculate_materials() {
    // Reset summary
    summary = ResourceSummary();

    // Bricks: collect estimates and accumulate total
    for (size_t i = 0; i < bricks_arr.size(); ++i) {
        BrickEstimate be;
        be.type = bricks_arr[i].get_type();
        be.num = bricks_arr[i].get_num_req();
        be.unit_cost = materials.brick_cost;
        be.cost = be.num * be.unit_cost;
        summary.bricks.push_back(be);
        summary.total_bricks += be.num;
        summary.total_cost += be.cost;
    }

    // Mortar & Cement base calculations
    double mortar_m3_base = 0.0;
    double cement_kg_base = 0.0;
    if (materials.mortar_m3_per_1000_bricks > 0.0) {
        mortar_m3_base = (summary.total_bricks / 1000.0) * materials.mortar_m3_per_1000_bricks;
        cement_kg_base = mortar_m3_base * materials.cement_kg_per_m3_mortar;
    } else {
        // Fallback: use kg per 1000 bricks for base cement, infer mortar if possible
        cement_kg_base = (summary.total_bricks / 1000.0) * materials.cement_kg_per_1000_bricks;
        if (materials.cement_kg_per_m3_mortar > 0.0) {
            mortar_m3_base = cement_kg_base / materials.cement_kg_per_m3_mortar;
        }
    }

    // Apply waste (user-configurable) and compute final cement quantity & cost
    summary.mortar_m3_base = mortar_m3_base;
    summary.mortar_m3_with_waste = mortar_m3_base * (1.0 + materials.waste_factor);
    summary.cement_kg_base = cement_kg_base;
    summary.cement_kg_final = std::ceil(cement_kg_base * (1.0 + materials.waste_factor));
    summary.cement_bags_5kg = static_cast<int>(std::ceil(summary.cement_kg_final / 5.0));
    summary.cement_cost = summary.cement_bags_5kg * materials.cement_cost_per_5kg;
    summary.total_cost += summary.cement_cost;

    // Water calculation: use base cement kg and then apply waste once
    summary.water_l = cement_kg_base * materials.water_l_per_kg_cement * (1.0 + materials.waste_factor);
    summary.water_cost = summary.water_l * materials.water_cost_per_litre;
    summary.total_cost += summary.water_cost;

    // Sand calculation (if mortar known): convert to mass and compute cost
    if (mortar_m3_base > 0.0) {
        double sand_m3_base = mortar_m3_base * materials.sand_m3_per_m3_mortar;
        summary.sand_m3 = sand_m3_base * (1.0 + materials.waste_factor);
        summary.sand_kg = summary.sand_m3 * materials.sand_density_kg_per_m3;
        summary.sand_tonnes = summary.sand_kg / 1000.0;
        summary.sand_cost = summary.sand_tonnes * materials.sand_cost_per_tonne;
        summary.total_cost += summary.sand_cost;
    }
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
    out << YAML::Key << "bags_5kg" << YAML::Value << summary.cement_bags_5kg;
    out << YAML::Key << "unit_cost" << YAML::Value << materials.cement_cost_per_5kg;
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

//pseudo-code to implement logic, then will name everything properly 
void House::calculate_bricks() {
    std::cout << "Entered calculate_bricks()\n"; // DEBUGGING
    // For each brick type, accumulate adjusted wall volume (subtract openings) and convert to number of bricks
    for (size_t i = 0; i < bricks_arr.size(); ++i) {
        int num_bricks = 0;
        std::cout << "Calculating for brick type: " << bricks_arr[i].get_type() << "\n"; // DEBUG
        double total_volume = 0.0;
        std::cout << "Number of walls: " << walls_arr.size() << "\n"; // DEBUG

        for (size_t j = 0; j < walls_arr.size(); ++j) {
            std::cout << "Checking wall: " << walls_arr[j].get_identifier() << " (brick_type=" << walls_arr[j].get_brick_type() << ")\n";
            if (bricks_arr[i].get_type() != walls_arr[j].get_brick_type()) {
                continue;
            }

            // Add wall volume
            double wall_vol = walls_arr[j].get_volume();

            // Subtract windows volume attached to this wall
            if (walls_arr[j].has_window) {
                for (const auto &win : windows_arr) {
                    if (walls_arr[j].get_identifier() == win.get_asc_wall()) {
                        wall_vol -= (win.get_area() * walls_arr[j].get_thickness());
                    }
                }
            }

            // Subtract doors volume attached to this wall
            if (walls_arr[j].has_door) {
                for (const auto &door : doors_arr) {
                    if (walls_arr[j].get_identifier() == door.get_asc_wall()) {
                        wall_vol -= (door.get_area() * walls_arr[j].get_thickness());
                    }
                }
            }

            total_volume += wall_vol;
            std::cout << "Accumulated volume after wall " << walls_arr[j].get_identifier() << ": " << total_volume << "\n";
        }

        // Calculate bricks required for this brick type (round up)
        if (bricks_arr[i].get_volume() <= 0.0) {
            std::cerr << "Error: brick volume for type '" << bricks_arr[i].get_type() << "' is zero or negative.\n";
            bricks_arr[i].set_num_req(0);
            continue;
        }

        num_bricks = static_cast<int>( std::ceil(total_volume / bricks_arr[i].get_volume()) );
        bricks_arr[i].set_num_req(num_bricks);

        house_data += "The number of " + bricks_arr[i].get_type() + " bricks required is : "; 
        house_data += std::to_string(bricks_arr[i].get_num_req()) + "\n";
        std::cout << "House info now: " << house_data << "\n";
    }
    std::cout << "FINISHED CALCULATING\n"; //DEBUGGING
}

