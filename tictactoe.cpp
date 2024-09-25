#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include "tictactoe.h"

char checkWinner(std::string &board, char player)
{
    return ((board[0] + board[1] + board[2] == player * 3) || (board[3] + board[4] + board[5] == player * 3) || (board[6] + board[7] + board[8] == player * 3) || (board[0] + board[3] + board[6] == player * 3) || (board[1] + board[4] + board[7] == player * 3) || (board[2] + board[5] + board[8] == player * 3) || (board[0] + board[4] + board[8] == player * 3) || (board[2] + board[4] + board[6] == player * 3));
} //checkWin

int isWinner(std::string board)
{
    if (checkWinner(board, 'X'))
        return 'X';
    else if (checkWinner(board, 'Y'))
        return 'Y';
    else
        return 0;
} //isWinner

std::tuple<bool, int> isExternal(std::vector<Node> &boards, int pKey)
{
    int numOfZeros = std::count(boards[pKey].config.begin(), boards[pKey].config.end(), '0');
    bool skipNodes{false};

    if (isWinner(boards[pKey].config) == 'X')
    {
        boards[pKey].score = 1;
        skipNodes = true;
    }
    else if (isWinner(boards[pKey].config) == 'Y')
    {
        boards[pKey].score = -1;
        skipNodes = true;
    }
    if (numOfZeros == 0)
        skipNodes = true;

    return std::make_tuple(skipNodes, numOfZeros);
} //isExternal

std::vector<int> externalNodesKeys;
std::vector<int> nodesPerDepth = {1, 9, 72, 504, 3024, 15120, 60480, 181440, 362880};

void initBoard(std::vector<Node> &boards, std::string pBoard, int &index, int &pKey, int depth, std::string mark)
{
    auto [skipNodes, numOfZeros] = isExternal(boards, pKey);

    if (!skipNodes)
    {
        auto newSize = boards.size() + numOfZeros;
        boards.resize(newSize);
        std::string::size_type pos;

        std::string temp{pBoard};
        while (index < newSize)
        {
            pos = temp.find('0');
            temp.replace(pos, 1, mark);
            std::string board{pBoard};
            board.replace(pos, 1, mark);
            boards[index] = Node(index, depth + 1, pKey, 0, board);

            if (isWinner(boards[index].config) == 'X')
                boards[index].score = 1;
            else if (isWinner(boards[index].config) == 'Y')
                boards[index].score = -1;
            boards[pKey].childNodes.push_back(index);
            ++index;
        }
    }
    else
    {
        externalNodesKeys.push_back(pKey);
        nodesPerDepth[depth + 1] -= (nodesPerDepth[depth + 1] / nodesPerDepth[depth]);
    }
    ++pKey;
} //initBoard

std::tuple<int, int> max_element(std::vector<Node> &nodeList, std::vector<int> &childNodes, int &key)
{
    if (childNodes.size() == 0)
        return std::make_tuple(0, -1);

    auto maxIndex{0};
    auto maxValue{nodeList[childNodes[0]].score};
    key = childNodes[0];

    for (auto i = 1; i < childNodes.size(); ++i)
    {
        if (maxValue < nodeList[childNodes[i]].score)
        {
            maxValue = nodeList[childNodes[i]].score;
            key = childNodes[i];
            maxIndex = i;
        }
    }
    return std::make_tuple(maxValue, maxIndex);
} //max_element

std::tuple<int, int> min_element(std::vector<Node> &nodeList, std::vector<int> &childNodes, int &key)
{
    if (childNodes.size() == 0)
        return std::make_tuple(0, -1);

    auto minIndex{0};
    auto minValue{nodeList[childNodes[0]].score};
    key = childNodes[0];

    for (auto i = 1; i < childNodes.size(); ++i)
    {
        if (minValue > nodeList[childNodes[i]].score)
        {
            minValue = nodeList[childNodes[i]].score;
            key = childNodes[i];
            minIndex = i;
        }
    }
    return std::make_tuple(minValue, minIndex);
} //min_element

void updateScores(std::vector<Node> &nodeList)
{
    int lastKey = nodeList.size() - 1;
    int pKey = nodeList[lastKey].pKey;
    while (pKey > 0)
    {
        Node *pNode = &nodeList[pKey];
        int minmaxKey{0};

        if ((pNode->childNodes).size() > 0)
        {
            if (pNode->depth % 2 == 0)
            {
                auto [maxScore, maxIndex] = max_element(nodeList, pNode->childNodes, minmaxKey);
                pNode->score = maxScore;
            }
            else
            {
                std::tuple<int, int> min = min_element(nodeList, pNode->childNodes, minmaxKey);
                pNode->score = std::get<0>(min);
            }
        }
        --pKey;
    } //while
} //updateScores

void drawBoard(std::string config)
{
    std::string tempStr{config};
    std::replace(tempStr.begin(), tempStr.end(), '0', static_cast<char>(32));

    std::cout << "\n " << tempStr[0] << " | " << tempStr[1] << " | " << tempStr[2] << "\n";
    std::cout << "-----------\n";
    std::cout << " " << tempStr[3] << " | " << tempStr[4] << " | " << tempStr[5] << "\n";
    std::cout << "-----------\n";
    std::cout << " " << tempStr[6] << " | " << tempStr[7] << " | " << tempStr[8] << "\n";
} //drawBoard

void init(std::vector<Node> &boards)
{
    int index{0};
    int pIndex{0};
    int depth{-1};

    boards.emplace_back(index, 0, depth, 0, "000000000");
    ++index;

    std::string player{"X"};
    for (depth = 0; depth <= 9; ++depth)
    {
        for (int i = 0; i < nodesPerDepth[depth]; ++i)
        {
            player = (depth) % 2 == 0 ? "X" : "Y";
            initBoard(boards, boards[pIndex].config, index, pIndex, depth, player);
        }
    }
} //init

void placeMark(std::string &config, char player)
{
    int boardPos;
    while (true)
    {
        std::cout << "\nChoose position on board(1-9): ";
        std::cin >> boardPos;
        if (1 > boardPos || boardPos > 9)
            throw "Position out of bounds.Exiting...";

        if (config[boardPos - 1] == '0')
        {
            config[boardPos - 1] = player;
            break;
        }
    } //while
    std::cout << "\nPlacing " << player << " ...\n";
} //placeMark

int AImarkX(std::vector<Node> &boards, int key)
{
    //Find max score in depth below
    std::vector<int> childList(boards[key].childNodes);
    auto [maxScore, maxIndex] = max_element(boards, boards[key].childNodes, key);

    std::vector<int> bucket; //Collect child nodes with max score
    bucket.reserve(3);
    std::vector<int> highPriorityNodes;

    for (auto i = maxIndex; i < childList.size(); ++i)
    {
        if (boards[childList[i]].score == maxScore)
        {
            bucket.push_back(childList[i]);
            if ((boards[childList[i]].childNodes).size() == 0)
            {
                highPriorityNodes.push_back(childList[i]);
            }
        }
    } //for
    //Randomly select node with good probability of winning
    auto seed = static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count());
    std::default_random_engine eng{seed};
    int lastKey = static_cast<int>(bucket.size() - 1);
    int index;

    if (highPriorityNodes.size() == 0)
    {
        lastKey = static_cast<int>(bucket.size() - 1);
        std::uniform_int_distribution<int> distr{bucket[0], bucket[lastKey]};
        index = distr(eng);
    }
    else
    {
        lastKey = static_cast<int>(highPriorityNodes.size() - 1);
        std::uniform_int_distribution<int> distr{highPriorityNodes[0], highPriorityNodes[lastKey]};
        index = distr(eng);
    }
    return index;
} //AImarkX

int AImarkY(std::vector<Node> &boards, int key)
{
    //Find min score in depth below
    std::vector<int> childList(boards[key].childNodes);
    auto [minScore, minIndex] = min_element(boards, boards[key].childNodes, key);

    std::vector<int> bucket; //Collect child nodes with min score
    bucket.reserve(3);
    std::vector<int> highPriorityNodes;

    for (auto i = minIndex; i < childList.size(); ++i)
    {
        if (boards[childList[i]].score == minScore)
        {
            bucket.push_back(childList[i]);
            if ((boards[childList[i]].childNodes).size() == 0)
            {
                highPriorityNodes.push_back(childList[i]);
            }
        }
    }
    //Randomly select lowest priority node
    auto seed = static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count());
    std::default_random_engine eng{seed};
    int lastKey = static_cast<int>(bucket.size() - 1);
    int index;

    if (highPriorityNodes.size() == 0)
    {
        lastKey = static_cast<int>(bucket.size() - 1);
        std::uniform_int_distribution<int> distr{bucket[0], bucket[lastKey]};
        index = distr(eng);
    }
    else
    {
        lastKey = static_cast<int>(highPriorityNodes.size() - 1);
        std::uniform_int_distribution<int> distr{highPriorityNodes[0], highPriorityNodes[lastKey]};
        index = distr(eng);
    }
    return index;
} //AImarkY

void updateAIconfig(std::vector<Node> &boards, Node *&root, std::string &currentConfig, bool AImoveFirst)
{
    auto childPos = std::find_if((root->childNodes).begin(), (root->childNodes).end(), [&boards, root, &currentConfig](int i) { return ((boards[i].config == currentConfig) && (boards[i].pKey == root->key)); });
    root = &(boards[*childPos]);
    if (AImoveFirst)
        return;

    auto nodePos = std::find_if(boards.begin(), boards.end(), [&currentConfig](Node &item) { return (item.config == currentConfig); });
    *root = (*nodePos);
    if ((*root).config == "")
        (*root).config = currentConfig;
} //updateAIconfig

void displayMenu()
{
    std::cout << "\nBOARD POSITIONS\n-----------\n";
    std::string boardPositions{"123456789"};
    drawBoard(boardPositions);
    std::cout << "\nGAME BOARD\n";
} //displayMenu

void play(std::vector<Node> &boards)
{
    std::string currentConfig{boards[0].config};
    drawBoard(currentConfig);
    char player; //X or Y

    while (true)
    {
        std::cout << "\nEnter X to choose player A, Y for player B: ";
        std::cin >> player;
        if (player == 'x' || player == 'y')
            break;
    }

    player = std::toupper(player);
    char AImark = player == 'X' ? 'Y' : 'X';
    Node *root = nullptr;
    bool drawGameBoard{false};

    while (true)
    {
        if (AImark == 'X') //human player chose B(Y)
        {
            if (root != nullptr)
                updateAIconfig(boards, root, currentConfig);
            else
                root = &boards[0];
            if (drawGameBoard)
                drawBoard(root->config);
            drawGameBoard = true;
            if (isGameOver(root->config))
                break;

            std::cout << "\nAI places X on the board...\n";
            int maxKey = AImarkX(boards, root->key);
            root = &boards[maxKey];
            currentConfig = root->config;
            drawBoard(currentConfig);

            if (isGameOver(root->config))
                break;
            //player places Y on board
            placeMark(currentConfig, player);
        }
        else
        { //human player chose A(X)
            if (root == nullptr)
                root = &(boards[0]);
            placeMark(currentConfig, player);
            bool moveFirst{false};
            updateAIconfig(boards, root, currentConfig, moveFirst);
            drawBoard(currentConfig);
            if (isGameOver(root->config))
                break;

            std::cout << "\nAI places Y on the board...\n";
            int minKey = AImarkY(boards, root->key);
            root = &boards[minKey];
            currentConfig = root->config;
            drawBoard(currentConfig);
            if (isGameOver(root->config))
                break;
        }
    } //while
} //play

bool isGameOver(std::string &config)
{
    if (isWinner(config) == 'X')
    {
        std::cout << "\nX is the winner\n";
        return true;
    }
    else if (isWinner(config) == 'Y')
    {
        std::cout << "\nY is the winner\n";
        return true;
    }
    else
    {
        int numOfZeros = std::count(config.begin(), config.end(), '0');
        if (numOfZeros == 0)
        {
            std::cout << "\nIt's a draw\n";
            return true;
        }
    }
    return false;
} //isGameOver