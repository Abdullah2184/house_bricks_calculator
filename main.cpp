#include <iostream>
#include <string>
#include "HouseClass.h"
#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>

static void usage(const char *prog) {
    std::cout << "Usage: " << prog << " -H <house.yaml> -b <bricks.yaml> -m <materials.yaml> [-o output.yaml]\n";
}

int main(int argc, char** argv) {
    std::string house_file, bricks_file, materials_file, out_file = "output.yaml";
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "-h" || a == "--help") { usage(argv[0]); return 0; }
        if ((a == "-H" || a == "--house") && i + 1 < argc) { house_file = argv[++i]; continue; }
        if ((a == "-b" || a == "--bricks") && i + 1 < argc) { bricks_file = argv[++i]; continue; }
        if ((a == "-m" || a == "--materials") && i + 1 < argc) { materials_file = argv[++i]; continue; }
        if ((a == "-o" || a == "--output") && i + 1 < argc) { out_file = argv[++i]; continue; }
        std::cerr << "Unknown arg: " << a << "\n"; usage(argv[0]); return 1;
    }

    if (house_file.empty() || bricks_file.empty() || materials_file.empty()) {
        std::cerr << "Error: must specify house, bricks and materials input files (use -H <house.yaml> -b <bricks.yaml> -m <materials.yaml>).\n";
        usage(argv[0]);
        return 1;
    }

    House userHouse;
    spdlog::info("Reading house YAML file: {}", house_file);
    if (!userHouse.readHouseYaml(house_file)) {
        spdlog::error("Failed to read house file: {}", house_file);
        return 1;
    }
    if (!userHouse.readBricksYAML(bricks_file)) {
        spdlog::error("Failed to read bricks file: {}", bricks_file);
        return 1;
    }
    if (!userHouse.readMaterialsYaml(materials_file)) {
        spdlog::error("Failed to read materials file: {}", materials_file);
        return 1;
    }

    // Perform calculations in two phases: brick counts, then materials derived from bricks.
    userHouse.calculate_bricks();
    userHouse.calculate_materials();

    if (!userHouse.writeOutputYaml(out_file)) {
        spdlog::error("Failed to write output file: {}", out_file);
        return 1;
    }

    spdlog::info("Calculation complete. Output written to {}", out_file);
    return 0;
}