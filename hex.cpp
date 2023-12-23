#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <set>
#include <vector>

using namespace std;

// A simple hex game
// James Swafford, Dec. 2023

enum HexPlayer { NONE, BLUE, RED };


class HexBoard
{
public:

    enum direction { NORTH, SOUTH, EAST, WEST };

    HexBoard(int n) : dim(n) {
        occupancy.resize(dim*dim);
        for (int r=0;r<dim;r++) {
            for (int c=0;c<dim;c++) {
                occupancy[r*dim+c] = NONE;
                unoccupied.push_back(make_pair(r,c));
            }
        }
        ptm = BLUE;
    }

    // copy constructor
    HexBoard(const HexBoard &hb) {
        dim = hb.dim;
        occupancy = hb.occupancy;
        unoccupied = hb.unoccupied;
        ptm = hb.ptm;
    }

    std::string to_str() const;

    // the game is over when the blue player has a connected path from east to west
    // or the red player has a connected path from north to south
    bool game_is_won(HexPlayer p) const;

    // make a move.  If the move is legal, the move is applied and true is returned.  
    // otherwise (the move is not legal), false is returned.
    bool make_move(int row, int col);
    void unmake_move(int row, int col);

    inline int get_dim() const { return dim; }

    inline HexPlayer get_ptm() const { return ptm; }

    inline HexPlayer get_opponent() const { return ptm==BLUE ? RED : BLUE; }

    // get all the unoccupied positions
    vector<pair<int,int>> get_unoccupied() const { return vector<pair<int,int>>(unoccupied); }

    // fill the remaining positions on the board, alternating colors
    // beginning with the player currently on move
    void fill_board(); 

private:
    int dim;  // the height and width of the board

    vector<HexPlayer> occupancy;
    vector<pair<int,int>> unoccupied;

    HexPlayer ptm; // player to move

    friend std::ostream & operator<<(std::ostream &os, const HexBoard& hb);

    // determine if a player p has a connected path from r,c to any position on the border of d
    bool is_connected_path(HexPlayer p, int r, int c, direction d, set<pair<int,int>> &visited) const;

    // is row,col a boundary position in the direction of d?
    bool is_boundary_node(int row, int col, direction d) const;

    vector<pair<int,int>> get_neighbors(int row,int col) const;

};

std::ostream & operator<<(std::ostream &os, const HexBoard& hb)
{
    return os << hb.to_str();
}

std::string HexBoard::to_str() const {

   /*  3x3 board for reference
    *  . - . - .
    *   \ / \ / \
    *    . - . - .
    *     \ / \ / \
    *      . - . - .
    */

    string board_str = "\n";
    for (int r=0;r<dim;r++) {
        // indent
        int num_spaces = r*2;
        for (int i=0;i<num_spaces;i++) board_str += " ";

        for (int c=0;c<dim;c++) {
            // print player symbol
            HexPlayer p = occupancy[r*dim+c];
            if (p==BLUE) board_str += "B";
            else if (p==RED) board_str += "R";
            else board_str += ".";

            // if not last column add an edge to the right
            if (c != dim-1) board_str += " - ";
            else board_str += "\n";
        }

        // if not the last row add edges below
        if (r != dim-1) {
            for (int i=0;i<=num_spaces;i++) board_str += " ";

            // the first node only has one vertical edge
            board_str += "\\";
            for (int c=1;c<dim;c++) board_str += " / \\";
            board_str += "\n";
        }
    }
    board_str += "\n";
    
    if (ptm==BLUE) board_str += "BLUE to move\n\n";
    else board_str += "RED to move\n\n";

    return board_str;
}

bool HexBoard::make_move(int r, int c) {
    if (occupancy[r*dim+c] != NONE) return false;

    vector<pair<int,int>>::iterator unocc_it = find(begin(unoccupied), end(unoccupied), make_pair(r,c));
    assert(unocc_it != end(unoccupied));
    unoccupied.erase(unocc_it);

    occupancy[r*dim+c] = ptm;

    // swap the player
    ptm = ptm==BLUE ? RED : BLUE;

    return true;
}

// reverse the last move.  put the opponent back on move.
void HexBoard::unmake_move(int r, int c) {
    assert(occupancy[r*dim+c] != NONE);

    occupancy[r*dim+c] = NONE;
    unoccupied.push_back(make_pair(r,c));

    // swap the player
    ptm = ptm==BLUE ? RED : BLUE;
}

bool HexBoard::game_is_won(HexPlayer p) const {

    assert(p==BLUE || p==RED);

    set<pair<int,int>> visited;

    if (p==BLUE) {
        for (int r=0;r<dim;r++) {
            if (is_connected_path(BLUE, r, 0, EAST, visited)) return true;
        }
    } else {
        for (int c=0;c<dim;c++) {
            if (is_connected_path(RED, 0, c, SOUTH, visited)) return true;
        }
    }

    return false;
}

void HexBoard::fill_board() {

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(unoccupied.begin(), unoccupied.end(), std::default_random_engine(seed));

    for (pair<int,int> pos : unoccupied) {
        assert(occupancy[pos.first * dim + pos.second]==NONE);
        occupancy[pos.first * dim + pos.second] = ptm;
        ptm = ptm==BLUE ? RED : BLUE; // swap players
    }
    unoccupied.clear();

    // the game is guaranteed to be won (not a draw)
    assert(game_is_won(BLUE) || game_is_won(RED));
}

bool HexBoard::is_boundary_node(int r, int c, direction d) const {
    if (d==NORTH && r==0) return true;
    if (d==SOUTH && r==(dim-1)) return true;
    if (d==WEST && c==0) return true;
    if (d==EAST && c==(dim-1)) return true;
    return false;
}

vector<pair<int,int>> HexBoard::get_neighbors(int r,int c) const {
    vector<pair<int,int>> neighbors;

    if (c>0) neighbors.push_back(make_pair(r,c-1));
    if (r>0) neighbors.push_back(make_pair(r-1,c)); 
    if (r>0 && c<dim-1) neighbors.push_back(make_pair(r-1,c+1));
    if (c<dim-1) neighbors.push_back(make_pair(r,c+1)); 
    if (r<dim-1) neighbors.push_back(make_pair(r+1,c));
    if (r<dim-1 && c>0) neighbors.push_back(make_pair(r+1,c-1));

    return neighbors;
}

bool HexBoard::is_connected_path(HexPlayer p, int r, int c, direction d, set<pair<int,int>> &visited) const {

    if (occupancy[r * dim + c] != p) return false;

    // base case: if we're on the boundary we're done
    if (is_boundary_node(r, c, d)) return true;

    // we have a connected path if there is a connected path from any of our neighbors
    vector<pair<int,int>> neighbors = get_neighbors(r,c);
    for (auto neighbor : neighbors) {
        bool in_visited = visited.find(neighbor) != visited.end();
        if (!in_visited) {
            visited.insert(neighbor);
            bool found_path = is_connected_path(p, neighbor.first, neighbor.second, d, visited);
            if (found_path) return true;
        }
    }

    return false;
}

class HexAI
{
    public:

        // choose a move
        void make_move(HexBoard &hb);

    private:
        const int NUM_SIMULATIONS = 10000;

        int run_simulations(const HexBoard &hb);

};

// the AI (computer player) makes a move using a Monte Carlo selection
void HexAI::make_move(HexBoard &hb)
{
    vector<pair<int,int>> moves = hb.get_unoccupied();

    pair<int,int> best_mv;
    int max_wins = -1;
    for (auto mv : moves) {
        hb.make_move(mv.first, mv.second);
        int num_wins = run_simulations(hb);
        hb.unmake_move(mv.first, mv.second);
        if (num_wins > max_wins) {
            best_mv = mv;
            max_wins = num_wins;
        }
    }

    cout << "The computer chooses move " << (best_mv.first+1) << " " << (best_mv.second+1) << endl;
    float win_pct = static_cast<float>(max_wins) / NUM_SIMULATIONS * 100;
    cout << "win % after " << NUM_SIMULATIONS << " simulations: " << win_pct << "%" << endl;
    hb.make_move(best_mv.first, best_mv.second);
}

// run Monte Carlo simulations
// return the number of wins from the opponent perspective, since the player
// we're maximizing for just made a move
int HexAI::run_simulations(const HexBoard &hb)
{
    HexPlayer player = hb.get_opponent();
    int num_wins = 0;
    for (int i=0;i<NUM_SIMULATIONS;i++) {
        HexBoard my_board(hb);
        my_board.fill_board();
        // did the player win?
        if (my_board.game_is_won(player)) num_wins++;
    }
    return num_wins;
}

HexPlayer choose_color() {
    char color_choice;
    HexPlayer user = NONE;
    cout << "Which color would you like - blue or red? (blue goes first)" << endl;
    while (user == NONE) {
        cout << "Enter your choice (b/r): " << endl;
        cin >> color_choice;
        switch (color_choice) {
            case 'b': {
                cout << "Blue it is!" << endl;
                user = BLUE;
                break;
            }
            case 'r': {
                cout << "Red it is!" << endl;
                user = RED;
                break;
            }
            default: cout << "Invalid input." << endl;
        }
    } 
    return user;
}

void user_move(HexBoard &hb) {
    bool move_made;
    do {
        int row,col;
        bool out_of_bounds;
        do {
            cout << "Enter your move (row col):" << endl;
            cout << "Note: the top left corner is 1 1" << endl;
            cin >> row >> col;
            if (cin.fail()) {
                cout << "Invalid input, try again." << endl;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        } while (row < 1 || row > hb.get_dim() || col < 1 || col > hb.get_dim());
        
        move_made = hb.make_move(row-1, col-1);
        if (!move_made) cout << "Illegal move!  Try again." << endl;
    } while (!move_made);
}


int main() {

    cout << "Let's play a game of Hex!" << endl;
    cout << "Blue wins by connecting east to west." << endl;
    cout << "Red wins by connecting north to south." << endl << endl;

    int dim;
    do {
        cout << "What size board?  Enter a number 5-11." << endl;
        cin >> dim;
    } while (dim < 5 || dim > 11);

    HexBoard hb = HexBoard(dim);
    HexAI ai = HexAI();

    // ask the user which color they would like to play
    HexPlayer user = choose_color();

    // if the user opted for the second move, let the computer make a move
    if (user == RED) {
        ai.make_move(hb);
    }

    // game loop - user moves then computer
    while (!hb.game_is_won(hb.get_opponent())) {
        cout << hb;
        user_move(hb);
        if (hb.game_is_won(hb.get_opponent())) break;
        ai.make_move(hb);
    }

    // the game is over.  the winner is the last player to move.
    cout << hb  << "Game over.  The winner is ... ";
    if (hb.get_ptm()==BLUE) cout << "RED!" << endl;
    else cout << "BLUE!" << endl;
    
}
