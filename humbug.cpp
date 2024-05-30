#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>
#include <functional>
#include <bits/stdc++.h>

using namespace std;

// Class for representing the game state
class State {
public:
    vector<string> grid;
    int n;
    int m;

    static const vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; 

    State(const vector<string>& g = {}) : grid(g) {
        n = g.size();
        m = g[0].length();
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

    //get possible actions from this state -> the OG function! (PGSLFH)
    vector<Action> getActions(){
        for(int i = 1; i<n; i+=2){ 
            for(int j = 1; i<m; j+=2){
                char insect = grid[i][j];
                Coordinate insectPos(i,j);
                if(insect == 'P') return getActionsSpider(insectPos);
                else if(insect=='G') return getActionHopper();
                else if (insect=='S') return getActionsSnail();
                else if(insect=='L') return getActionsLadybug();
                else if(insect=='H') return getActionHBee();
                else{
                    return getActionBFly();
                }
            }
        }
    }

    bool isVacant(int x, int y){
        return grid[x][y]=='X' || grid[x][y]=='Y';
    }

    bool isWallAhead(int x, int y, pair<int,int> dir){
        return grid[x+dir.first][y+dir.first]=='W';
    }

    bool isInGrid(int x, int y){
        return x<n && y<m; 
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
                curX = nextX - 1;
                curY = nextY - 1;

                // check for wall...
                if(isWallAhead(nextX, nextY, dir)) break;

                //check for insect...
                if(!isVacant(nextX, nextY)) break;

                actions.push_back({spiderPos, Coordinate(nextX, nextY)});
                
                // Update nextX and nextY for the next step in the same direction
                nextX += dir.first;
                nextY += dir.second;
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
                // Check if the next position is vacant and accesible
                if (isVacant(nextX, nextY) && isVacant(nextX-2, nextY-2) && !isWallAhead(ladybugPos.x, ladybugPos.y, dir) && !isWallAhead(nextX-2, nextY-2, dir)) {
                    // If it's a valid move, append the destination coordinate to actions
                    actions.push_back({ladybugPos, Coordinate(nextX, nextY)});
                }
            }
        }

        return actions;
    }

    vector<Action> getActionHBee(const Coordinate &hbeePos){
        // HBee flies to two size step, and if it is occupied, jumps to the next position
        vector<Action> actions;

        for (const auto &dir : directions) {
            int nextX = hbeePos.x + 4 * dir.first;
            int nextY = hbeePos.y + 4 * dir.second;

            // Check if the next position is within the grid bounds
            if (isInGrid(nextX, nextY)) {
                // If the next position is accessible, 
                if ((isVacant(nextX, nextY) && !isWallAhead(hbeePos.x, hbeePos.y, dir) && !isWallAhead(nextX-2, nextY-2, dir))){
                    
                    
                    // If it's a valid move, append the destination coordinate to actions
                    actions.push_back({hbeePos, Coordinate(nextX, nextY)});
                }
            }
        }

        return actions;
    }

    vector<Action> getActionHopper;
    vector<Action> getActionBFly;

};

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
};


// Class for single player game using BFS
class SinglePlayerGame {
public:
    using TransitionFunction = function<State(const State&, const Action&)>;

    SinglePlayerGame(State initial, State goal, TransitionFunction transition)
        : initial_state(initial), goal_state(goal), transition_function(transition) {}

    void bfs() {
        unordered_set<State, State::HashFunction> visited;
        queue<pair<State, vector<Action>>> q;

        q.push({initial_state, {}});

        while (!q.empty()) {
            auto [current_state, path] = q.front();
            q.pop();

            if (current_state == goal_state) {
                print_solution(path);
                return;
            }

            if (visited.find(current_state) == visited.end()) {
                visited.insert(current_state);

                vector<Action> actions = current_state.getActions();
                for (const auto &action : actions) {
                    State next_state = transition_function(current_state, action);
                    vector<Action> new_path = path;
                    new_path.push_back(action);
                    q.push({next_state, new_path});
                }
            }
        }

        cout << "No solution found." << endl;
    }

private:
    State initial_state;
    State goal_state;
    TransitionFunction transition_function;

    void print_solution(const vector<Action> &path) const {
        cout << "Solution found:" << endl;
        for (const auto &action : path) {
            cout << action.to_string() << " ";
        }
        cout << endl;
    }
};

// Example usage
int main() {
    State initial_state(0); // Example initial state
    State goal_state(4); // Example goal state

    // Define the transition function according to your game's rules
    auto transition_function = [](const State &state, const Action &action) -> State {
        if (action.name == "add1") return State(state.value + 1);
        if (action.name == "add2") return State(state.value + 2);
        if (action.name == "add3") return State(state.value + 3);
        return state;
    };

    SinglePlayerGame game(initial_state, goal_state, transition_function);
    game.bfs();

    return 0;
}
