#ifndef H_TICTACTOE
#define H_TICTACTOE

#include <iostream>
#include <vector>
#include <string>
#include <tuple>

//Holds information about specific
// game(board) configuration
struct Node
{
    int key;
    int depth;
    int pKey;
    int score;
    std::string config;
    std::vector<int> childNodes;

    Node() = default;
    Node(int _key, int _depth, int _pKey, int _score, std::string _config) : key{_key}, depth{_depth}, pKey{_pKey}, score{_score}, config{_config} {}
};
//Checks whether player A or B,
//represented by X or Y respectively, is the winner.
char checkWinner(std::string &board, char player);
//Returns current game state(win, loose or draw)
int isWinner(std::string board);
//Returns whether or not current node is external
std::tuple<bool, int> isExternal(std::vector<Node> &boards, int pKey);
//Adds nodes to game tree at appropriate depths.Parameter
// index is key of node to add
void initBoard(std::vector<Node> &boards, std::string parentBoard, int &index, int &parentKey, int nodeDepth, std::string playerMark);
//Returns score and position of
//child with maximum score
std::tuple<int, int> max_element(std::vector<Node> &nodeList, std::vector<int> &childNodes, int &key);
//Returns score and position of
//child with minimum score
std::tuple<int, int> min_element(std::vector<Node> &nodeList, std::vector<int> &childNodes, int &key);
//Updates the score of each node
void updateScores(std::vector<Node> &nodeList);
//Displays current game configuration
void drawBoard(std::string config);
void init(std::vector<Node> &boards);
//Initializes the game
void init(std::vector<Node> &boards);
//Places human player's mark on board
//at chosen position
void placeMark(std::string &config, char player);
//Places AI's X mark at selected position
int AImarkX(std::vector<Node> &boards, int key);
//Places AI's Y mark at selected position
int AImarkY(std::vector<Node> &boards, int key);
//Updates AI's game position on game tree
void updateAIconfig(std::vector<Node> &boards, Node *&root, std::string &currentConfig, bool AImoveFirst = true);
//Displays gameboard's positional information
void displayMenu();
//Game loop where gameplay starts and ends
void play(std::vector<Node> &boards);
//Determines if game has ended
bool isGameOver(std::string &config);

#endif