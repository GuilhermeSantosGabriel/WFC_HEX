import os
import re

search_dir = "WFC_Core"
output_dot_file = "./tools/dependencies/uml_com_main.dot"

include_regex = re.compile(r'#include\s+["<](?:cli/|engine/|models/)?([^">]+)[">]')

edges = set()
valid_modules = set()

# 1. Map headers and execution entry files (like main.cpp)
for root, _, files in os.walk(search_dir):
    if "external" in root:
        continue
    for file in files:
        if file.endswith('.h'):
            valid_modules.add(file)
        elif file == "main.cpp":  # Dynamically add main if it exists
            valid_modules.add(file)

# 2. Scan dependencies
for root, _, files in os.walk(search_dir):
    if "external" in root:
        continue
    for file in files:
        if file.endswith(('.h', '.cpp')):
            # If it's main, it is the origin. For other .cpp files, map to their respective .h
            origin_module = file if file == "main.cpp" else file.replace('.cpp', '.h')
            
            if origin_module not in valid_modules:
                continue
            
            with open(os.path.join(root, file), "r", errors="ignore") as f_in:
                for line in f_in:
                    match = include_regex.search(line)
                    if match:
                        included = os.path.basename(match.group(1))
                        if included in valid_modules and included != origin_module:
                            edges.add((origin_module, included))

# 3. Generate Tuned UML DOT File to Untangle Edges
with open(output_dot_file, "w") as f_out:
    f_out.write("digraph G {\n")
    f_out.write("  rankdir=TB;\n")  # Top to Bottom architecture flow
    
    # FLOW AND EDGE TUNING
    f_out.write("  splines=spline;\n")   # Smooth dynamic curves to avoid collisions
    f_out.write("  nodesep=1.2;\n")       # Generous horizontal spacing between blocks
    f_out.write("  ranksep=1.2;\n")       # Generous vertical spacing between layers
    
    # Classic UML Styling
    f_out.write("  node [fontname=\"Helvetica\", fontsize=11, shape=rect, style=\"filled\", fillcolor=\"#ffffff\", color=\"#333333\", penwidth=1.5];\n")
    f_out.write("  edge [fontname=\"Helvetica\", fontsize=10, color=\"#444444\", arrowsize=0.8, penwidth=1.0];\n")
    
    # Highlight main.cpp discrete styling
    if "main.cpp" in valid_modules:
        f_out.write("  \"main.cpp\" [fillcolor=\"#f5f5f5\", style=\"filled,dashed\"];\n")

    # Write dependencies with layout edge weight adjustments
    for origin, destination in edges:
        if origin == "main.cpp":
            f_out.write(f'  "{origin}" -> "{destination}" [weight=0, style="solid", color="#444444"];\n')
        else:
            f_out.write(f'  "{origin}" -> "{destination}";\n')
        
    f_out.write("}\n")

print("Complete UML dependency graph (with main) generated successfully!")