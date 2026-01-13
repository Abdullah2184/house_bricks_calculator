#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>
#include "../HouseClass.h"
#include <string>

//Testing if the Append functions correctly add new objects to the respective vectors
TEST(AppendFunctionality, AppendWall) {
    House house;
    Walls wall("wall_1", 2.5, 5.0, 0.20, "brick_type_1");
    house.append_Wall(wall);
    ASSERT_EQ(house.walls_arr.size(), 1);
    EXPECT_EQ(house.walls_arr[0].get_id(), "wall_1");
    EXPECT_DOUBLE_EQ(house.walls_arr[0].get_height(), 2.5);
    EXPECT_DOUBLE_EQ(house.walls_arr[0].get_width(), 5.0);
    EXPECT_DOUBLE_EQ(house.walls_arr[0].get_thickness(), 0.20);
    EXPECT_EQ(house.walls_arr[0].get_brick_type(), "brick_type_1");
}
TEST(AppendFunctionality, AppendWindow) {
    House house;
    Windows window("window_1", 1.2, 1.5, "wall_1");
    house.append_Window(window);
    ASSERT_EQ(house.windows_arr.size(), 1);
    EXPECT_EQ(house.windows_arr[0].get_id(), "window_1");
    EXPECT_DOUBLE_EQ(house.windows_arr[0].get_height(), 1.2);
    EXPECT_DOUBLE_EQ(house.windows_arr[0].get_width(), 1.5);
    EXPECT_EQ(house.windows_arr[0].get_wall_id(), "wall_1");
}
TEST(AppendFunctionality, AppendDoor) {
    House house;
    Doors door("door_1", 2.0, 0.9, "wall_2");
    house.append_Door(door);
    ASSERT_EQ(house.doors_arr.size(), 1);
    EXPECT_EQ(house.doors_arr[0].get_id(), "door_1");
    EXPECT_DOUBLE_EQ(house.doors_arr[0].get_height(), 2.0);
    EXPECT_DOUBLE_EQ(house.doors_arr[0].get_width(), 0.9);
    EXPECT_EQ(house.doors_arr[0].get_wall_id(), "wall_2");
}
TEST(AppendFunctionality, AppendBrick) {
    House house;
    Bricks brick("brick_type_1", 0.2, 0.1, 0.1);
    house.append_Brick(brick);
    ASSERT_EQ(house.bricks_arr.size(), 1);
    EXPECT_EQ(house.bricks_arr[0].get_id(), "brick_type_1");
    EXPECT_DOUBLE_EQ(house.bricks_arr[0].get_length(), 0.2);
    EXPECT_DOUBLE_EQ(house.bricks_arr[0].get_height(), 0.1);
    EXPECT_DOUBLE_EQ(house.bricks_arr[0].get_width(), 0.1);
}


//Paramterized house validation tests

//Running this struct in the parser function for each test case and comparing the
//result to the expected result will allow us to validate the YAML parsing functionality for
//valid and invalid inputs 
struct HouseYamlTestCase {
    std::string name;
    std::string yaml;
    bool expected_result;
};

class HouseParsingYamlTest : public ::testing::TestWithParam<HouseYamlTestCase> {};

TEST_P(HouseParsingYamlTest, ParseHouseYaml) {
    auto const& param = GetParam();
    House house;
    YAML::Node root = YAML::Load(param.yaml);
    bool result = house.parseHouseYaml(root);
    EXPECT_EQ(result, param.expected_result) << "Test case: " << param.name;
}

INSTANTIATE_TEST_SUITE_P(
    HouseYamlValidation,
    HouseYamlParamTest,
    ::testing::Values(
        // Valid case
        HouseYamlCase{
            "Valid house",
            R"(
owner: "Alice"
walls:
  - id: W1
    height: 3.0
    width: 5.0
    thickness: 0.2
    brick_type: standard
    windows: 1
    doors: 1
windows:
  - id: Win1
    height: 1.2
    width: 1.0
    wall: W1
doors:
  - id: Door1
    height: 2.0
    width: 0.9
    wall: W1
)",
            true
        },

        // Invalid; Missing wall thickness
        HouseYamlCase{
            "Wall missing thickness",
            R"(
owner: "Alice"
walls:
  - id: W1
    height: 3.0
    width: 5.0
    brick_type: standard
    windows: 0
    doors: 0
)",
            false
        },

        // Invalid; Negative wall width
        HouseYamlCase{
            "Negative wall width",
            R"(
owner: "Alice"
walls:
  - id: W1
    height: 3.0
    width: -5.0
    thickness: 0.2
    brick_type: standard
    windows: 0
    doors: 0
)",
            false
        },

        // Invalid; Window references unknown wall
        HouseYamlCase{
            "Window references invalid wall",
            R"(
owner: "Alice"
walls:
  - id: W1
    height: 3.0
    width: 5.0
    thickness: 0.2
    brick_type: standard
    windows: 0
    doors: 0
windows:
  - id: Win1
    height: 1.2
    width: 1.0
    wall: W999
)",
            false
        }
    )
);

struct BricksYamlTestCase {
    std::string name;
    std::string yaml;
    bool expected_result;
};

class BricksParsingYamlTest : public ::testing::TestWithParam<BricksYamlTestCase> {};

TEST_P(BricksParsingYamlTest, ParseBricksYaml) {
    auto& tc = GetParam();
    House house;
    YAML::Node root = YAML::LoadFile(tc.yaml);
    bool result = house.parseBricksYAML(root);
    EXPECT_EQ(result, tc.expected_result) << "Test case: " << tc.name;
}

INSTANTIATE_TEST_SUITE_P(
    BricksYamlValidation,
    BricksYamlParamTest,
    ::testing::Values(

        // VALID
        BricksYamlCase{
            "Valid brick definition",
            R"(
bricks:
  - id: standard
    bond: stretcher
    size:
      length: 0.215
      height: 0.065
      width: 0.1025
    unit_cost: 0.50
    bricks_per_m2:
      "0.1025": 60
      "0.20": 120
    mortar_m3_per_1000_bricks: 0.45
)",
            true
        },

        // Invalid; Missing size.width
        BricksYamlCase{
            "Brick missing width",
            R"(
bricks:
  - id: standard
    bond: stretcher
    size:
      length: 0.215
      height: 0.065
    unit_cost: 0.50
)",
            false
        },

        // Invalid; Negative bricks_per_m2
        BricksYamlCase{
            "Negative bricks_per_m2",
            R"(
bricks:
  - id: standard
    bond: stretcher
    size:
      length: 0.215
      height: 0.065
      width: 0.1025
    unit_cost: 0.50
    bricks_per_m2:
      "0.1025": -60
)",
            false
        }
    )
);

//Materials
struct MaterialsYamlTestCase {
    std::string name;
    std::string yaml;
    bool expected_result;
};

class MaterialsParsingYamlTest : public ::testing::TestWithParam<MaterialsYamlTestCase> {};

TEST_P(MaterialsParsingYamlTest, ParseMaterialsYaml) {
    auto& tc = GetParam();
    House house;
    YAML::Node root = YAML::LoadFile(tc.yaml);
    bool result = house.parseMaterialsYAML(root);
    EXPECT_EQ(result, tc.expected_result) << "Test case: " << tc.name;
}

INSTANTIATE_TEST_SUITE_P(
    MaterialsYamlValidation,
    MaterialsYamlParamTest,
    ::testing::Values(

        // VALID
        MaterialsYamlCase{
            "Valid materials",
            R"(
materials:
  cement:
    bag_size_kg: 50
    cost_per_bag: 20.0
    kg_per_m3_mortar: 300
  water:
    litres_per_kg_cement: 0.5
    cost_per_litre: 0.0
  sand:
    m3_per_m3_mortar: 0.75
    density_kg_per_m3: 1600
    cost_per_tonne: 20.0
  waste:
    bricks: 0.05
    cement: 0.07
    sand: 0.10
    water: 0.05
)",
            true
        },

        // ❌ Waste factor > 0.25
        MaterialsYamlCase{
            "Invalid waste factor",
            R"(
materials:
  cement:
    bag_size_kg: 50
    cost_per_bag: 20.0
    kg_per_m3_mortar: 300
  water:
    litres_per_kg_cement: 0.5
    cost_per_litre: 0.0
  sand:
    m3_per_m3_mortar: 0.75
    density_kg_per_m3: 1600
    cost_per_tonne: 20.0
  waste:
    bricks: 0.40
    cement: 0.07
    sand: 0.10
    water: 0.05
)",
            false
        },

        // ❌ Missing cement bag size
        MaterialsYamlCase{
            "Missing cement bag size",
            R"(
materials:
  cement:
    cost_per_bag: 20.0
    kg_per_m3_mortar: 300
)",
            false
        }
    )
);
