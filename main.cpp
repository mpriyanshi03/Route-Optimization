#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <limits>
#include <cctype>  // Required for tolower function

using namespace std;

const int INF = numeric_limits<int>::max();

class CityGraph {
public:
    map<string, int> cityIndex;
    vector<vector<int>> adj;
    vector<string> cities;

    CityGraph(string filename) {
        ifstream file(filename);
        string line;
        int row = 0;

        while (getline(file, line)) {
            stringstream ss(line);
            string city;
            if (row == 0) { // First row contains city names
                int col = 0;
                while (getline(ss, city, ',')) {
                    if (col > 0) {
                        cityIndex[city] = col - 1;
                        cities.push_back(city);
                    }
                    col++;
                }
            } 
            else { 
                // Other rows contain distances
                int col = 0;
                while (getline(ss, city, ',')) {
                    if (col == 0) { // Skip the city name column
                        col++;
                        continue;
                    }
                    if (row - 1 >= adj.size()) 
                        adj.push_back(vector<int>(cities.size(), 0));
                    adj[row - 1][col - 1] = stoi(city);
                    col++;
                }
            }
            row++;
        }
        file.close();
    };

    void dijkstra(string src, string dest) {
        // Check if the source or destination city is not found
        if (cityIndex.find(src) == cityIndex.end()) {
            cout << "Error: Source city '" << src << "' not found in the dataset." << endl;
            return;
        }
        if (cityIndex.find(dest) == cityIndex.end()) {
            cout << "Error: Destination city '" << dest << "' not found in the dataset." << endl;
            return;
        }

        int V = cities.size();
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        vector<int> dist(V, INF);
        vector<int> parent(V, -1);

        int srcIndex = cityIndex[src];
        int destIndex = cityIndex[dest];
        pq.emplace(0, srcIndex);
        dist[srcIndex] = 0;

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (u == destIndex) break;

            for (int v = 0; v < V; v++) {
                if (adj[u][v] && dist[u] + adj[u][v] < dist[v]) {
                    dist[v] = dist[u] + adj[u][v];
                    pq.emplace(dist[v], v);
                    parent[v] = u;
                }
            }
        }

        if (dist[destIndex] != INF) {
            cout << "Shortest route from " << src << " to " << dest << " is " << dist[destIndex] << " km" << endl;
            cout << "Route: ";

            // Store the route in a vector
            vector<string> route;
            int crawl = destIndex;
            while (crawl != -1) {
                route.push_back(cities[crawl]);
                crawl = parent[crawl];
            }

            // Print the route in reverse order
            for (auto it = route.rbegin(); it != route.rend(); ++it) {
                cout << *it << " ";
            }
            cout << endl;
        } else {
            cout << "No route exists between " << src << " and " << dest << endl;
        }
    }


    
};

int main() {
    CityGraph g("cities_distances.csv");

    string src, dest;
    cout<<"Enter your source and final destination\n(Keep in mind that it's case sensitive and uses capiatlisation, means  first letter of a word is capitalized, while the rest of the letters are lowercase)"<<endl;
    cout << "Enter the source city: ";
    getline(cin, src);
    cout << "Enter the destination city: ";
    getline(cin, dest);

    g.dijkstra(src, dest);

    return 0;
}
