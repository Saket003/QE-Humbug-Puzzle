#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>
#include <functional>
#include <bits/stdc++.h>

using namespace std;

// Class for representing the game state

static const vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; 

struct Coordinate {
    int x;
    int y;

    Coordinate(int x_val, int y_val) : x(x_val), y(y_val) {}
};

// Class for representing actions
class Action {
public:
    Coordinate start;
    Coordinate end;

    string to_string() const {
        return "Move insect at {" + std::to_string(start.x) + ',' + std::to_string(start.y) + "} to {" + std::to_string(end.x) + ',' + std::to_string(end.y) + "} .";
    }
};

class State {
public:
    vector<string> grid;
    int n;
    int m;
    int starCount; //==0 for a goal state
    int moves; //moves remaining

    State(vector<string>g, int maxMoves=10){
        n = static_cast<int>(g.size());     
        m = static_cast<int>(g[0].length());  
        for(auto line: g){
            grid.push_back(line);
        }
        starCount = 0;
        getStarCount();
        this->moves = maxMoves;
        
    }

    void getStarCount(){
        for(int i = 0; i<n; i++){
            for(int j = 0; j<m; j++){
                // cout << grid[i] << endl;
                if(grid[i][j]=='Y') starCount++;
            }
        }
    }

    bool operator==(const State &other) const {
        auto &grid2 = other.grid;
        int n = grid.size();
        int m = grid[0].size();
        for(int i = 0; i<n; i++){
            for(int j = 0; j<m; j++){
                if(grid[i][j]!=grid2[i][j]){
                    return false;
                }
            }
        }
        return true;
    }

    // Custom hash function for the State
    struct HashFunction {
        size_t operator()(const State &s) const {
            size_t hash = 0;
            for (const auto &row : s.grid) {
                for (const char &cell : row) {
                    hash ^= std::hash<char>()(cell) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
                }
            }
            return hash;
        }
    };

    vector<Action> getActions() {
        vector<Action> actions;
        for (int i = 1; i < n; i += 2) {
            for (int j = 1; j < m; j += 2) {
                char insect = grid[i][j];
                Coordinate insectPos(i, j);
                
                if (insect == 'P') {
                    // Append actions for spider
                    vector<Action> spiderActions = getActionsSpider(insectPos);
                    actions.insert(actions.end(), spiderActions.begin(), spiderActions.end());
                } else if (insect == 'G') {
                    // Append actions for hopper
                    vector<Action> hopperActions = getActionHopper(insectPos);
                    actions.insert(actions.end(), hopperActions.begin(), hopperActions.end());
                } else if (insect == 'S') {
                    // Append actions for snail
                    vector<Action> snailActions = getActionsSnail(insectPos);
                    actions.insert(actions.end(), snailActions.begin(), snailActions.end());
                } else if (insect == 'L') {
                    // Append actions for ladybug
                    vector<Action> ladybugActions = getActionsLadybug(insectPos);
                    actions.insert(actions.end(), ladybugActions.begin(), ladybugActions.end());
                } else if (insect == 'H') {
                    // Append actions for HBee
                    vector<Action> hBeeActions = getActionHBee(insectPos);
                    actions.insert(actions.end(), hBeeActions.begin(), hBeeActions.end());
                } else {
                    // No actions for other insects
                }
            }
        }

        return actions;
}


    bool isVacant(int x, int y){  return grid[x][y]=='X' || grid[x][y]=='Y';
    }

    bool isWallAhead(int x, int y, pair<int,int> dir){
        return grid[x+dir.first][y+dir.second]=='W';
    }

    bool isInGrid(int x, int y){
        return x<n && y<m && x>=0 && y>=0 && grid[x][y]!='.'; 
    }


    vector<Action> getActionsSpider(const Coordinate &spiderPos){
        // spider can keep moving in one direction, unless obstacle comes.
      
        vector<Action> actions;

        // Iterate through each direction
        for (const auto &dir : directions) {
            int nextX = spiderPos.x + 2*dir.first; //2 to watch out for walls
            int nextY = spiderPos.y + 2*dir.second;

            int curX, curY;
            
            // Keep moving in the same direction until boundary or obstacle is encountered
            while (isInGrid(nextX, nextY)) {
                curX = nextX - 2*dir.first;
                curY = nextY - 2*dir.second;

                // check for wall|insect...
                if(isWallAhead(nextX, nextY, dir) || (isInGrid(nextX+2*dir.first, nextY+2*dir.second) && !isVacant(nextX+2*dir.first, nextY+2*dir.second))){
                    actions.push_back({spiderPos, Coordinate(nextX, nextY)});
                    break;
                }
                // Update nextX and nextY for the next step in the same direction
                nextX += 2*dir.first;
                nextY += 2*dir.second;
            }
        }

        return actions;
    }

    vector<Action> getActionsSnail(const Coordinate &snailPos){
        //snail takes one step at a time

        vector<Action> actions;
        for (const auto &dir : directions) {
            int nextX = snailPos.x + 2*dir.first;
            int nextY = snailPos.y + 2*dir.second;

            // Check if the next position is within the grid bounds
            if (isInGrid(nextX, nextY)) {
                // Check if the next position is vacant
                if (isVacant(nextX, nextY) && !isWallAhead(snailPos.x, snailPos.y, dir)) {
                    // If it's a valid move, append the destination coordinate to actions
                    actions.push_back({snailPos, Coordinate(nextX, nextY)});
                }
            }
        }

        return actions;
    }
    
    vector<Action> getActionsLadybug(const Coordinate &ladybugPos){
         // Ladybug takes step of size two in one move
        vector<Action> actions;

        for (const auto &dir : directions) {
            int nextX = ladybugPos.x + 4 * dir.first;
            int nextY = ladybugPos.y + 4 * dir.second;

            // Check if the next position is within the grid bounds
            if (isInGrid(nextX, nextY)) {
                // Check if the first step is vacant and accesible
                if (isVacant(nextX-2*dir.first, nextY-2*dir.second) && !isWallAhead(ladybugPos.x, ladybugPos.y, dir)) {
                    
                    // check if second step is vacant and accesible
                    if((isVacant(nextX, nextY) && !isWallAhead(nextX-2*dir.first, nextY-2*dir.second, dir))){
                    // If it's a valid move, append the destination coordinate to actions
                        actions.push_back({ladybugPos, Coordinate(nextX, nextY)});
                    }
                    else{
                        actions.push_back({ladybugPos, Coordinate(nextX-2*dir.first, nextY-2*dir.second)});
                    }
                }
            }
        }

        return actions;
    }

    vector<Action> getActionHBee(const Coordinate &hbeePos){
        // HBee flies to two size step, and if it is occupied, flies to the next position
        // Bee flies over wall.
        vector<Action> actions;

        for (const auto &dir : directions) {
            int nextX = hbeePos.x + 4 * dir.first;
            int nextY = hbeePos.y + 4 * dir.second;

            // Check if the next position is within the grid bounds
            while (isInGrid(nextX, nextY)==true) {
                // If the next position is accessible, 
                if ((isVacant(nextX, nextY))){
                    // If it's a valid move, append the destination coordinate to actions
                    actions.push_back({hbeePos, Coordinate(nextX, nextY)});
                }
                else{
                    //jump ahead!
                    nextX+=2*dir.first; nextY+=2*dir.second;
                }
            }
        }

        return actions;
    }

    vector<Action> getActionHopper(const Coordinate &hopperPos){
        // Hopper jumps to one size step, and if it is occupied, jumps to the next position
        // Hopper jumps over wall.
        vector<Action> actions;

        for (const auto &dir : directions) {
            int nextX = hopperPos.x + 2 * dir.first;
            int nextY = hopperPos.y + 2 * dir.second;
            // Check if the next position is within the grid bounds

            while (isInGrid(nextX, nextY)) {
                // If the next position is accessible, 
                if ((isVacant(nextX, nextY))){
                    // If it's a valid move, append the destination coordinate to actions
                    actions.push_back({hopperPos, Coordinate(nextX, nextY)});
                    break;
                }
                else{
                    //jump ahead!
                    nextX+=2*dir.first; nextY+=2*dir.second;
                }
            }
        }

        return actions;

    }
    vector<Action> getActionBFly; //yet to encounter butterfly.

    State transition(const Action& action) {
        // Create a copy of the current grid
        vector<string> new_grid = grid;

        Coordinate start = action.start;
        Coordinate end = action.end;

        char insect = grid[start.x][start.y];
        char target_cell = grid[end.x][end.y];

        // Update the grid based on the action
        new_grid[start.x][start.y] = 'X';

        if (target_cell == 'Y') { //if star present
            new_grid[end.x][end.y] = 'X';
        } 
        else {
            new_grid[end.x][end.y] = insect;
        }

        // Return a new State object with the modified grid
        return State(new_grid, this->moves - 1);
    }

    bool isGoal(){ //check if state is goal state
        return starCount==0;
    }

    bool isFailed(){
        return moves==0 || moves<starCount; //little pruning
    }
};



// Class for single player game using BFS
class Humbug {
public:
    Humbug(State initial)
        : initial_state(initial) {}

    void bfs() {
        unordered_set<State, State::HashFunction> visited;
        queue<pair<State, vector<Action>>> q;

        q.push({initial_state, {}});

        while (!q.empty()) {
            // auto [current_state, path] = q.front();
            auto current_state = q.front().first;
            auto path = q.front().second;

            // cout << "new state" << endl;

            q.pop();

            if (current_state.isGoal()) {
                print_solution(path);
                return;
            }

            if (current_state.isFailed()){
                continue;
            }

            if (visited.find(current_state) == visited.end()) {
                visited.insert(current_state);

                vector<Action> actions = current_state.getActions();
                for (const auto &action : actions) {
                    // cout << action.to_string() << " ";
                
                    State next_state = current_state.transition(action);
                    vector<Action> new_path = path;
                    new_path.push_back(action);
                    q.push({next_state, new_path});
                }
                cout << endl;
            }
        }

        cout << "No solution found." << endl;
    }

public:
    State initial_state;

    void print_solution(const vector<Action> &path) const {
        cout << "Solution found:" << endl;
        for (const auto &action : path) {
            cout << action.to_string() << endl;
        }
        cout << endl;
    }
};

// Example usage
int main() {
    string line;
    vector<string> grid;
    // vector<string> grid = {
    //     ".........",
    //     ".YWX.X.G.",
    //     ".........",
    //     "...X.X...",
    //     ".........",
    //     ".Y.L.X...",
    //     "........."
    // };
    ifstream inputFile("encodings/level27.txt");
    while (getline(inputFile, line)) {
        grid.push_back(line);
    }
    inputFile.close();
    int moves = 10;
    State initial_state(grid, moves);
    cout << initial_state.starCount << "\n";
    Humbug game(initial_state);

    game.bfs();
 

    return 0;
}
