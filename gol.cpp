// Conway's Game of Life

#include <chrono>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>


using namespace std;


// Calculates modulo for negative numbers
// like in math and Python: a % n -> [0..n-1]
// Python: -6 % 5 == 4
// C/C++:  -6 % 5 == -1
inline int positive_modulo (int i, int n) {
    return (i % n + n) % n;
}


class Field {
private:
    const int width;
    const int height;
    vector<bool> cells;
    vector<int> neighbours;

    // Converts indices on the cyclic 2d grid to 1d array index.
    int get_index (int x, int y) const {
        x = positive_modulo(x, width);
        y = positive_modulo(y, height);
        return y*width + x;
    }

public:
    Field (int w, int h):
        width(w <= 0 ? throw logic_error("Width should be positive!") : w),
        height(h <= 0 ? throw logic_error("Height should be positive!") : h),
        cells(w*h, false),
        neighbours(w*h, 0) {}

    // Sets random state to the cell field.
    // alive_probability sets the probability for an individual cell to be alive.
    // Use seed to reproduce state.
    void randomize (double alive_probability=0.5, int seed=-1) {
        if (alive_probability < 0 || alive_probability > 1) {
            throw logic_error("alive_probability should be within [0, 1] range!");
        }
        if (seed < 0) {
            seed = chrono::system_clock::now().time_since_epoch().count();
        }
        auto rng = default_random_engine(seed);
        auto dist = uniform_real_distribution<double>(0,1);
        for (int i = 0; i < width*height; i++) {
            cells[i] = dist(rng) < alive_probability;
        }
    }

    // Clears cell field
    void clear () {
        cells.assign(width*height, false);
    }

    // Calculates cell field for the next epoch.
    void update () {
        neighbours.assign(width*height, 0);

        // Neighbours' displacements
        // XXX
        // X0X
        // XXX
        const int num_neighbours = 8;
        const int dx[] {-1,  0,  1, -1,  1, -1,  0,  1};
        const int dy[] {-1, -1, -1,  0,  0,  1,  1,  1};

        // Count alive neighbours for each cell
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                bool alive = cells[get_index(x, y)];
                if (!alive) continue;

                for (int n = 0; n < num_neighbours; n++) {
                    int neighbour_ind = get_index(x + dx[n], y + dy[n]);
                    neighbours[neighbour_ind]++;
                }
            }
        }

        for (int i = 0; i < width*height; i++) {
            int alive_count = neighbours[i];
            if (alive_count < 2 || alive_count >= 4)
                cells[i] = false;
            else if (alive_count == 3)
                cells[i] = true;
        }
    }

    // Gets cell state for given coordinates
    bool get (int x, int y) const {
        return cells[get_index(x, y)];
    }

    // Sets cell state for given coordinates
    void set (int x, int y, bool state) {
        cells[get_index(x, y)] = state;
    }

    // Sets offset pattern on the cell field
    void set_pattern (const vector<string>& pattern, int dx=0, int dy=0) {
        for (unsigned y = 0; y < pattern.size(); y++) {
            for (unsigned x = 0; x < pattern[y].size(); x++) {
                set(x+dx, y+dy, pattern[y][x] == 'X');
            }
        }
    }

    string to_string () const {
        char state[] = ".X";
        string str((width+1)*height, '\n');
        for (int y=0; y<height; y++) {
            for (int x=0; x<width; x++) {
                int cell_ind = y*width + x;
                int str_ind = y*(width+1) + x;
                str[str_ind] = state[cells[cell_ind]];
            }
        }
        return str;
    }

    bool operator== (const Field& other) const {
        if (width != other.width or height != other.height) return false;
        for (int i = 0; i < width*height; i++) {
            if (cells[i] != other.cells[i]) return false;
        }
        return true;
    }

    bool operator!= (const Field& other) const {
        return !(*this == other);
    }
};


ostream& operator<< (ostream& os, const Field& f) {
    return os << f.to_string();
}


const vector<string> GLIDER {
    ".X.",
    "..X",
    "XXX"
};


const vector<string> PULSAR {
    ".................",
    ".................",
    "....XXX...XXX....",
    ".................",
    "..X....X.X....X..",
    "..X....X.X....X..",
    "..X....X.X....X..",
    "....XXX...XXX....",
    ".................",
    "....XXX...XXX....",
    "..X....X.X....X..",
    "..X....X.X....X..",
    "..X....X.X....X..",
    ".................",
    "....XXX...XXX....",
    ".................",
    ".................",
};


const vector<string> GOSPER_GLIDER_GUN {
    "......................................",
    ".........................X............",
    ".......................X.X............",
    ".............XX......XX............XX.",
    "............X...X....XX............XX.",
    ".XX........X.....X...XX...............",
    ".XX........X...X.XX....X.X............",
    "...........X.....X.......X............",
    "............X...X.....................",
    ".............XX.......................",
};


int main () {
    auto field = Field(70, 30);

    field.set_pattern(GOSPER_GLIDER_GUN);

    // field.set_pattern(GLIDER);
    // field.set_pattern(GLIDER, 0, 5);
    // field.set_pattern(GLIDER, 5, 0);
    // field.set_pattern(GLIDER, 5, 5);

    // field.randomize(0.2);

    for (;;) {
        cout << field << endl;
        field.update();
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    // auto pulsar = Field(20, 20);
    // pulsar.set_pattern(PULSAR);
    // auto pulsar_start = pulsar;
    // int epoch = 0;
    // do {
    //     pulsar.update();
    //     epoch++;
    // } while (pulsar != pulsar_start);

    // cout << "Pulsar repeats in " << epoch << " epochs." << endl;
}
