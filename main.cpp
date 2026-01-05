#include <iostream>
#include <string>
#include "HouseClass.h"
#include <yaml-cpp/yaml.h>

static void usage(const char *prog) {
    std::cout << "Usage: " << prog << " -H <house.yaml> -m <materials.yaml> [-o output.yaml]\n";
}

int main(int argc, char** argv) {
    std::string house_file, materials_file, out_file = "output.yaml";
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "-h" || a == "--help") { usage(argv[0]); return 0; }
        if ((a == "-H" || a == "--house") && i + 1 < argc) { house_file = argv[++i]; continue; }
        if ((a == "-m" || a == "--materials") && i + 1 < argc) { materials_file = argv[++i]; continue; }
        if ((a == "-o" || a == "--output") && i + 1 < argc) { out_file = argv[++i]; continue; }
        std::cerr << "Unknown arg: " << a << "\n"; usage(argv[0]); return 1;
    }

    if (house_file.empty() || materials_file.empty()) {
        std::cerr << "Error: must specify both house and materials input files (use -H <house.yaml> -m <materials.yaml>).\n";
        usage(argv[0]);
        return 1;
    }

    House userHouse;
    if (!userHouse.readHouseYaml(house_file)) {
        std::cerr << "Failed to read house file: " << house_file << std::endl;
        return 1;
    }
    if (!userHouse.readMaterialsYaml(materials_file)) {
        std::cerr << "Failed to read materials file: " << materials_file << std::endl;
        return 1;
    }

    // Perform calculations in two phases: brick counts, then materials derived from bricks.
    userHouse.calculate_bricks();
    userHouse.calculate_materials();

    if (!userHouse.writeOutputYaml(out_file)) {
        std::cerr << "Failed to write output file: " << out_file << std::endl;
        return 1;
    }

    std::cout << "Calculation complete. Output written to " << out_file << std::endl;
    return 0;
}