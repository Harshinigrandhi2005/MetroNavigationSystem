# MetroNavigationSystem
MetroNetworkPathfinding is a C-based application that models a metro network using graph algorithms. It involves concepts from DSA like graphs, DFS, and BFS for route management and station analysis.


# Metro Network Path-Finding System

This is a C-based project designed to model and interact with a metro network. The system allows users to input various metro stations, routes, and lines, and provides functionality for pathfinding between stations, identifying intersections, and displaying terminal stations.

## Features
- **Find Routes**: Find all paths between two stations, including fare calculation.
- **Station Information**: Display information about a station, including the lines it belongs to and its adjacent stations.
- **Intersections**: Find stations that exist in multiple lines (intersections).
- **Terminal Stations**: Display the terminal stations for each metro line.
- **Nearest Intersection**: Find the shortest path to a station that serves multiple metro lines.

## Project Structure

The project consists of the following main structures:

1. **Route**: Represents a route between two metro stations.
    - `destination`: The name of the destination station.
    - `fare`: The fare to travel this route.
    - `next`: A pointer to the next route in the adjacency list.

2. **Line**: Represents a particular metro line (e.g., Red Line, Blue Line).
    - `line_name`: Name of the metro line.
    - `station_count`: Total number of stations on this line.
    - `station_names`: Array of station names on this line.

3. **MetroNetwork**: Represents the entire metro network.
    - `station_names`: Array of all station names in the network.
    - `adjacency_list`: Pointer to a linked list of routes for each station.
    - `lines`: Pointer to an array of metro lines.
    - `total_stations`: Total number of stations in the network.
    - `total_lines`: Total number of lines in the network.

## Key Functions

### 1. **find_station_index()**
   - Finds the index of a station from the station names array.

### 2. **find_line_index()**
   - Finds the index of a line from the lines array.

### 3. **add_route()**
   - Adds a route between two stations in the metro network, including a reverse route.

### 4. **add_line()**
   - Adds a metro line with its stations to the network.

### 5. **find_routes_util()**
   - Recursively finds all paths from a source station to a destination station.

### 6. **find_routes()**
   - A wrapper function that calls `find_routes_util()` and prints the paths along with the total fare.

### 7. **int_station_info()**
   - Displays information about a station, including the lines it is part of and its adjacent stations.

### 8. **display_intersections()**
   - Displays stations that appear in multiple lines (intersections).

### 9. **display_terminal_stations()**
   - Displays the terminal stations (first and last stations) for each metro line.

### 10. **find_path_to_nearest_intersections()**
   - Finds the shortest path to a station that serves multiple metro lines using BFS, and prints the path along with the total fare cost.

## Compilation & Execution

### Prerequisites
- A C compiler (e.g., GCC) must be installed on your system.
- You may need to install GTK if you plan on adding a graphical user interface (GUI) for the metro network (optional).

### Compile the Program
```bash
gcc -o metro_network metro_network.c

