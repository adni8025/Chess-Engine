#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<string> parseMoves(const string& movesString) {
    vector<string> moves;
    string move;
    size_t start = 0;
    size_t end = movesString.find(' ');
    while (end != string::npos) {
        move = movesString.substr(start, end - start);
        moves.push_back(move);
        start = end + 1;
        end = movesString.find(' ', start);
    }

    // Add the last move (after the last space)
    move = movesString.substr(start);
    moves.push_back(move);

    return moves;
}

int main() {
    string movesString = "e2e4 d7d5 g1f3 e7e6 e3 e3 f4 ";
    vector<string> movesVector = parseMoves(movesString);
   string movesString1 = "f2 g3 h4";
   movesVector = parseMoves(movesString+movesString1);
    for (const string& move : movesVector) {
        cout << endl << move << " ";
    }
    cout<<movesVector[1]<<endl;

    return 0;
}
