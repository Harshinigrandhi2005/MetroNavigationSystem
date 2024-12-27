#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATIONS 100
#define MAX_LINES 50
#define MAX_PATHS 100

typedef struct Route {
    char destination[50];
    int fare;
    struct Route* next;
} Route;

typedef struct Line {
    char name[50];
    char stations[MAX_STATIONS][50];
    int station_count;
} Line;

typedef struct MetroNetwork {
    char station_names[MAX_STATIONS][50];
    Route* adjacency_list[MAX_STATIONS];
    Line lines[MAX_LINES];
    int station_count;
    int line_count;
} MetroNetwork;

// Helper functions
int find_station_index(MetroNetwork* network,char* station_name) {
    //in network it goes to total station count (~n)
    for (int i = 0; i < network->station_count; i++) {
        //in network the list of station names, if matches return i
        if (strcmp(network->station_names[i], station_name) == 0) {
            return i;
        }
    }
    return -1;
}

int find_line_index(MetroNetwork* network,char* line_name) {
    //total no. of lines in network
    for (int i = 0; i < network->line_count; i++) {
        //compare it to the lines list in network and return i
        if (strcmp(network->lines[i].name, line_name) == 0) {
            return i;
        }
    }
    return -1;
}

void add_route(MetroNetwork* network,char* source,char* destination, int fare) {
    int src_index = find_station_index(network, source);
    int dest_index = find_station_index(network, destination);
    
    //source to destination
    Route* new_route = (Route*)malloc(sizeof(Route));
    strcpy(new_route->destination, destination);////in the newroot destination is set
    new_route->fare = fare;
    new_route->next = network->adjacency_list[src_index];//setting the ad_list for that particular index
    network->adjacency_list[src_index] = new_route;

    //destination to source
    new_route = (Route*)malloc(sizeof(Route));
    strcpy(new_route->destination, source);
    new_route->fare = fare;
    new_route->next = network->adjacency_list[dest_index];
    network->adjacency_list[dest_index] = new_route;
}

void add_line(MetroNetwork* network,char* line_name,char* stations[], int station_count) {
    int line_index = network->line_count++;//incremeanting the value of line_count
    strcpy(network->lines[line_index].name, line_name);
    //in networks the list of lines
    network->lines[line_index].station_count = station_count;
    //take in all line structures into the MetroNetwork
    for (int i = 0; i < station_count; i++) {
        strcpy(network->lines[line_index].stations[i], stations[i]);
    }
}

void print_station_info(MetroNetwork* network,char* station_name) {
    int station_index = find_station_index(network, station_name);//taking index from helper function
    if (station_index == -1) {
        printf("Station not found!\n");
        return;
    }

    printf("Station Name: %s\n", station_name);
    printf("Lines:\n");
    //going through all lines via line_count
    for (int i = 0; i < network->line_count; i++) {
        //going through stations in a particular line
        for (int j = 0; j < network->lines[i].station_count; j++) {
            //matches the station then displays which line it belongs
            if (strcmp(network->lines[i].stations[j], station_name) == 0) {
                printf("- %s\n", network->lines[i].name);//prints the line 
                break;
            }
        }
    }

    printf("Adjacent Stations:\n");
    //print the adjacent stations from the adjacency list
    Route* route = network->adjacency_list[station_index];
    while (route != NULL) {
        printf("- %s, Fare: %d\n", route->destination, route->fare);
        route = route->next;
    }
}

//recursive function to explore all paths from src to dest
void find_routes_util(MetroNetwork* network, int src_index, int dest_index, int visited[], char path[][50], int path_len) {
    
    visited[src_index] = 1; //current statation marked as visisted

    //current station is copied on to path
    strcpy(path[path_len], network->station_names[src_index]);
    path_len++;

    // If we reached the destination, print the path and the cost
    if (src_index == dest_index) {
        int total_fare = 0;
        printf("Route: ");
        for (int i = 0; i < path_len; i++) {
            printf("%s ", path[i]); //print the path till the dest
            if (i < path_len - 1) printf("-> ");
        }
        printf(", Cost: ");
        // Calculate the total cost
        for (int i = 0; i < path_len - 1; i++) {
            int src_idx = find_station_index(network, path[i]);
            Route* route = network->adjacency_list[src_idx];
            while (route != NULL) {
                if (strcmp(route->destination, path[i + 1]) == 0) {
                    total_fare += route->fare;//func to calculate the total fare
                    break;
                }
                route = route->next;
            }
        }
        printf("%d\n", total_fare);
    } else {

        // Recursively visit adjacent stations
        Route* route = network->adjacency_list[src_index];
        while (route != NULL) {
            int next_index = find_station_index(network, route->destination);//next_index is where you reached currently
            if (!visited[next_index]) { 
                //if not visisted to put to path
                find_routes_util(network, next_index, dest_index, visited, path, path_len);
            }
            route = route->next;//move to next route
        }
    }
    visited[src_index] = 0; // Unmark the current station
}

void find_routes(MetroNetwork* network,char* source,char* destination) {
    int src_index = find_station_index(network, source);
    int dest_index = find_station_index(network, destination);

    if (src_index == -1 || dest_index == -1) {
        printf("One or both stations not found.\n");
        return;
    }

    printf("Possible routes from %s to %s:\n", source, destination);
    int visited[MAX_STATIONS] = {0};
    char path[MAX_PATHS][50];
    find_routes_util(network, src_index, dest_index, visited, path, 0);
}



void display_intersections(MetroNetwork* network) {
    int intersection_count[MAX_STATIONS] = {0};

    //finds number of lines and loops it
    for (int i = 0; i < network->line_count; i++) {
        //gives the number of stations on the current line and loops
        for (int j = 0; j < network->lines[i].station_count; j++) {
            //returns the corresponding index of this station
            int station_index = find_station_index(network, network->lines[i].stations[j]);
            //reflect that another line passes through this station and increments
            intersection_count[station_index]++;
        }
    }

    printf("Intersections (stations serving multiple lines):\n");
    //loops through total stations in the metro
    for (int i = 0; i < network->station_count; i++) {
        //checks if it is an intersection
        if (intersection_count[i] > 1) {
            printf("%s, %d\n", network->station_names[i], intersection_count[i]);
        }
    }
}

void display_terminal_stations(MetroNetwork* network) {
    printf("Terminal Stations:\n");
    for (int i = 0; i < network->line_count; i++) {
        int start_index = 0;
        //in that particular line , the stations (n) -1
        int end_index = network->lines[i].station_count - 1;
        printf("%s: %s, %s\n", network->lines[i].name,
               network->lines[i].stations[start_index],
               network->lines[i].stations[end_index]);
    }
}

void find_path_to_nearest_intersection(MetroNetwork* network, const char* station_name) {
    int station_index = find_station_index(network, station_name); //helper function
    
    if (station_index == -1) {
        printf("Station not found!\n");
        return;
    }

    int visited[MAX_STATIONS] = {0};//denotes the visited elements
    int queue[MAX_STATIONS], front = 0, rear = 0;//used in BFS
    int parent[MAX_STATIONS];//keeps the index of parent i.e prev element of temp_index
    
    for (int i = 0; i < MAX_STATIONS; i++)
    {
    parent[i] = -1;
    }

    queue[rear++] = station_index; //enqueue starting station to the queue
    visited[station_index] = 1;

    int min_distance = -1;  //to track the minimum distance to an intersection
    int found_intersections = 0;  //to track if any intersections are found

    while (front < rear) {
        int current_index = queue[front++];//dequeue
        Route* route = network->adjacency_list[current_index];

        while (route != NULL) {
            int next_index = find_station_index(network, route->destination); //destination stn index from curr node

            //if not visited enqueue
            if (!visited[next_index]) {
                visited[next_index] = 1;
                parent[next_index] = current_index;
                queue[rear++] = next_index;

                int lines_serving = 0;
                // Count how many lines serve the next station
                for (int i = 0; i < network->line_count; i++) {
                    for (int j = 0; j < network->lines[i].station_count; j++) {
                        if (strcmp(network->lines[i].stations[j], network->station_names[next_index]) == 0) {
                            lines_serving++;
                        }
                    }
                }

                //if this station is an intersection 
                if (lines_serving > 1) {
                    int current_distance = 0;
                    int temp_index = next_index;
                    int cost = 0;

                    //backtrack and calculate the distance from the source
                    while (temp_index != -1) {
                        if (parent[temp_index] != -1) {
                            Route* temp_route = network->adjacency_list[parent[temp_index]];
                            while (temp_route != NULL) {
                                if (strcmp(temp_route->destination, network->station_names[temp_index]) == 0) {
                                    cost += temp_route->fare;
                                    break;
                                }
                                temp_route = temp_route->next;
                            }
                        }
                        current_distance++;
                        temp_index = parent[temp_index];
                    }

                    // If it's the first intersection or matches the minimum distance, print it
                    if (min_distance == -1 || current_distance == min_distance) {
                        if (min_distance == -1) {
                            min_distance = current_distance;  // Set the minimum distance
                        }
                        found_intersections = 1;

                        // Backtrack to collect and print the path
                        int path[MAX_STATIONS];
                        int path_length = 0;
                        temp_index = next_index;
                        while (temp_index != -1) {
                            path[path_length++] = temp_index;
                            temp_index = parent[temp_index];
                        }

                        // Print the path in reverse order
                        for (int i = path_length - 1; i >= 0; i--) {
                            printf("%s", network->station_names[path[i]]);
                            if (i > 0) {
                                printf(" -> ");
                            }
                        }
                        printf(", Cost: %d\n", cost);
                    } 
                }
            }
            route = route->next;
        }
    }

    if (!found_intersections) {
        printf("No intersections found near %s.\n", station_name);
    }
}


int main() {
    MetroNetwork network = {0};
   

    int n;
    printf("Enter number of edges: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        char source[50], destination[50];
        int fare;
        printf("Enter edge %d (source destination fare): ", i + 1);
        scanf("%s %s %d", source, destination, &fare);

        if (find_station_index(&network, source) == -1) {
            strcpy(network.station_names[network.station_count++], source);
        }
        if (find_station_index(&network, destination) == -1) {
            strcpy(network.station_names[network.station_count++], destination);
        }

        add_route(&network, source, destination, fare);
    }

    int m;
    printf("Enter number of lines: ");
    scanf("%d", &m);
    for (int i = 0; i < m; i++) {
        char line_name[50];
        int k;
        printf("Enter line %d name: ", i + 1);
        scanf("%s", line_name);
        printf("Enter number of stations on %s: ", line_name);
        scanf("%d", &k);

        const char* stations[k];
        printf("Enter stations on %s:\n", line_name);
        for (int j = 0; j < k; j++) {
            char station[50];
            scanf("%s", station);
            stations[j] = strdup(station);
        }

        add_line(&network, line_name, stations, k);
    }

    // Menu for testing functionalities
    int choice;
    char station_name[50];
    char source[50],destination[50];

    while (1) {
        printf("\n--- Metro Network Menu ---\n");
        printf("1. Display station info\n");
        printf("2. Display intersections (stations with multiple lines)\n");
        printf("3. Display terminal stations\n");
        printf("4. Find path to nearest intersection\n");
        printf("5.Find Possible Routes between stations\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter station name: ");
                scanf("%s", station_name);
                print_station_info(&network, station_name);
                break;
            case 2:
                display_intersections(&network);
                break;
            case 3:
                display_terminal_stations(&network);
                break;
            case 4:
                printf("Enter station name: ");
                scanf("%s", station_name);
                find_path_to_nearest_intersection(&network, station_name);
                break;
            case 5:
                printf("Enter source and destination to find routes: ");
                scanf("%s %s", source, destination);
                find_routes(&network, source, destination);
                break;
            case 6:
                printf("Exiting program. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

