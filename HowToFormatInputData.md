This project now uses YAML-formatted inputs. There are two input files:

- House dimensions (walls, windows, doors, bricks) — units in metres.
- Materials and costs (brick cost, cement cost per 5kg, parameters) — units specified per field.

Example house YAML (see `house_example.yaml`):

owner: "Alice"

walls:
	- id: W1
		height: 3.0        # metres
		width: 5.0         # metres
		thickness: 0.2     # metres
		brick_type: standard
		windows: 1         # number of windows on this wall
		doors: 1           # number of doors on this wall

windows:
	- id: Win1
		height: 1.2
		width: 1.0
		wall: W1           # reference to the associated wall id

doors:
	- id: Door1
		height: 2.0
		width: 0.9
		wall: W1

bricks:
	- id: standard
		height: 0.065      # metres
		width: 0.215       # metres
		thickness: 0.1025  # metres

Example materials YAML (see `materials_example.yaml`):

prices:
	brick_cost: 0.50            # currency per brick (per piece)
	cement_cost_per_5kg: 2.00  # currency per 5kg bag
	water_cost_per_litre: 0.00 # currency per litre

parameters:
	cement_kg_per_1000_bricks: 70.0   # kg cement required per 1000 bricks (default assumption)
	water_l_per_kg_cement: 0.5        # litres of water per kg of cement
	# Optional parameters for a more accurate mortar-based calculation
	mortar_m3_per_1000_bricks: 0.4   # m^3 mortar per 1000 bricks (example value)
	cement_kg_per_m3_mortar: 300.0   # kg cement per m^3 mortar (depends on mix ratio)
	waste_factor: 0.05               # 5% waste by default
	sand_m3_per_m3_mortar: 0.75      # m^3 sand per 1 m^3 mortar (example)
	sand_density_kg_per_m3: 1600.0   # kg per m^3 of sand
	# Price fields
	sand_cost_per_tonne: 20.0        # currency per tonne (example)

Run the program as a CLI:

calcbricks.exe -H house_example.yaml -m materials_example.yaml -o output.yaml

(Note: the CLI requires explicit `-H` and `-m` options; `-i` is no longer supported.)

The output YAML contains resource lines for `bricks`, `cement`, and `water` with quantities, units and costs.