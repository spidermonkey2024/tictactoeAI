#include <iostream>
#include <string>
#include <vector>
#include "tictactoe.cpp"

int main(int argc, char *argv[])
{
    try
    {
        std::vector<Node> boards;
        init(boards);
        updateScores(boards);
        displayMenu();
        play(boards);
    }
    catch (const char *message)
    {
        std::cout << message << std::endl;
    } //try-catch
} //main