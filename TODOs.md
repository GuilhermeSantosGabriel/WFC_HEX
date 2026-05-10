# Main TO-DOs

## Rules
- Implement new types of tiles
- Make a read_rules_from_json method, so is not needed to compile the code on every rule change
- Setup the relation between tiles and their color

## Seeds
- For both the noises randomness and the random weight choice of tile type to collapse in wfc, make a easy way to define the wanted seed, (probably inside the rules .json itself)

## Open GL
- Atomize generate_river() and wave_function_collapse() into steps so an wrapper using the draw_hex_map_frame can be used easier

# Sketched TO-DOs

## Rules
- Use a nise to implement a slight variation on colors for tiles of each type
