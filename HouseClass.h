#ifndef HOUSECLASS_H // Header Guard
#define HOUSECLASS_H

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <vector>
#include <yaml-cpp/yaml.h>
#include "WallsClass.h"
#include "WindowsClass.h"
#include "BricksClass.h"
#include "DoorsClass.h"

class House {
    private:
    std::string owner_name;
    std::vector<Bricks> bricks_arr;
    std::vector<Walls> walls_arr;
    std::vector<Windows> windows_arr;
    std::vector<Doors> doors_arr;
    std::string house_data;

    public:
    House();
    House(std::string filename);
    ~House();
    void readHouseData(std::string filename);
    bool readHouseYaml(const std::string &filename);
    bool House::readBricksYAML(const std::string &filename);
    bool readMaterialsYaml(const std::string &filename);
    void append_Wall(Walls new_wall);
    void append_Window(Windows new_window);
    void append_Door(Doors new_door);
    void append_Brick(Bricks new_brick);
    void displayHouseData();
    void writeHouseData();
    // Calculate material requirements (cement, water, sand) and costs from computed brick counts
    void calculate_materials();
    // Write a pre-computed resource summary to a YAML file (does not perform calculations)
    bool writeOutputYaml(const std::string &outfile);
    void set_name();
    // Perform brick counting only (counts bricks per brick type based on wal
    void calculate_bricks();
    const Bricks* House::find_brick_by_id(const std::string &id) const;

    // Structures to hold calculation results (keeps calculation & I/O separated)
    struct BrickEstimate {
        std::string type;
        int num = 0;
        double unit_cost = 0.0;
        double cost = 0.0;
    };

    struct ResourceSummary {
        // Brick info
        std::vector<BrickEstimate> bricks;   // Estimated per brick type
        int total_bricks = 0;

        // Mortar
        double mortar_m3_base = 0.0;          // Base mortar volume (without waste)
        double mortar_m3_with_waste = 0.0;    // Mortar volume including waste factor

        // Cement
        double cement_kg_base = 0.0;          // Base cement mass (without waste)
        double cement_kg_final = 0.0;         // Cement mass including waste
        int cement_bags = 0;                  // Number of cement bags required (configurable bag size)
        double cement_cost = 0.0;             // Total cement cost

        // Water
        double water_l = 0.0;                 // Total water in litres
        double water_cost = 0.0;

        // Sand
        double sand_m3 = 0.0;                 // Sand volume in m3
        double sand_kg = 0.0;                 // Sand mass in kg
        double sand_tonnes = 0.0;             // Sand mass in tonnes
        double sand_cost = 0.0;

        // Total cost
        double total_cost = 0.0;
    } summary;

    struct Materials {
        // Brick
        double brick_cost = 0.0;              // Cost per brick

        // Cement
        double cement_bag_kg = 25.0;          // Bag weight (kg) — configurable per region
        double cement_cost_per_bag = 0.0;     // Cost per bag (matches bag size)
        double cement_kg_per_1000_bricks = 70.0;  // Legacy / optional assumption

        // Mortar (optional, more accurate calculation)
        double mortar_m3_per_1000_bricks = 0.0;       // m3 of mortar per 1000 bricks
        double cement_kg_per_m3_mortar = 300.0;       // kg cement per 1 m3 mortar
        double waste_factor = 0.0;                    // Fraction extra (e.g., 0.05 = 5% waste)
        double water_l_per_kg_cement = 0.5;           // Litres of water per kg of cement
        double water_cost_per_litre = 0.0;            // Cost per litre of water

        // Sand
        double sand_m3_per_m3_mortar = 0.75;         // Volume of sand per 1 m3 mortar
        double sand_density_kg_per_m3 = 1600.0;      // kg per m3
        double sand_cost_per_tonne = 0.0;            // Currency per tonne
    } materials;

    // Note: units used throughout:
    // - lengths: metres
    // - brick_cost: currency per single brick (piece)
    // - cement_cost_per_5kg: currency per 5kg bag
    // - water_cost_per_litre: currency per litre
};


#endif