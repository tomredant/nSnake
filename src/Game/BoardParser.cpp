#include <Game/BoardParser.hpp>
#include <Config/Globals.hpp>

#include <fstream>
#include <vector>
#include <string>

#define COMMENT_CHAR ';'
#define WALL_CHAR    '#'
#define SNAKE_CHAR   '@'

#include <Interface/Ncurses.hpp>
#include <stdlib.h>

Board* BoardParser::load(std::string filename)
{
    std::ifstream file(filename.c_str());

    if (!(file.is_open()))
        return NULL;

//    if (!(file.good())) what does it do?

    // This will get used to the end of the
    // function.
    std::string current_line = "";

    // Those store the level metadata
    std::string level_name    = "";
    std::string level_author  = "";
    std::string level_date    = "";
    std::string level_comment = "";

    // First we get the metadata (name, comment, etc),
    // then we focus on the level itself
    while (true)
    {
	    if (!std::getline(file, current_line))
	    {
		    // End-of-file...
		    // Something wrong happened
		    return NULL;
	    }

	    // Ignoring comments and empty lines
	    if ((current_line[0] == COMMENT_CHAR) ||
	        (current_line.empty()))
		    continue;

	    // Look for identifiers and get everything after
	    // them.
	    // For example:
	    //
	    //     name=This is the name
	    //
	    // We find `name=` and get everything after it
	    // to the end of line.
	    // And so on for other identifiers.
	    //
	    // TODO make this thing more flexible
	    //      (like handling whitespace between
	    //      the identifier and '=')

	    // Tells where we find a substring
	    // If it doesn't exist, will be `std::string::npos`
	    std::size_t pos;

	    pos = current_line.find("name=");
        if (pos != std::string::npos)
        {
            level_name = current_line.substr(pos + 5, std::string::npos);
            continue;
        }

        pos = current_line.find("author=");
        if (pos != std::string::npos)
        {
            level_author = current_line.substr(pos + 7, std::string::npos);
            continue;
        }

        pos = current_line.find("date=");
        if (pos != std::string::npos)
        {
            level_date = current_line.substr(pos + 5, std::string::npos);
            continue;
        }

        pos = current_line.find("comment=");
        if (pos != std::string::npos)
        {
            level_comment = current_line.substr(pos + 8, std::string::npos);
            continue;
        }

        // Yay, let's start it!
        if (current_line == "start")
	        break;
    }

    int player_start_x = 1; // It's (1, 1) because if it somehow starts
    int player_start_y = 1; // at (0, 0) it will always end up in a wall
                            // and die right at the beginning

    // Finally, when we read the level we have
    // two states for each tile - "wall" or "not wall"
    std::vector<std::vector<bool> > rawBoard;

    while (true)
    {
	    if (!std::getline(file, current_line))
		    return NULL;

	    // Ignoring comments and empty lines
	    if ((current_line[0] == COMMENT_CHAR) ||
	        (current_line.empty()))
		    continue;

        if (current_line == "end")
	        // Finally, we can finish it all!
	        break;

        std::vector<bool> rawBoardLine;

        // TODO Remove \n or \r\n from line
        //      (create Utils::String::chomp)

        // And now we go through each char on the line
        // checking if it's a wall, blank space or the
        // player's starting point.
        //
        for (int i = 0; i < current_line.size() - 2; i++)
        {
	        if (current_line[i] == SNAKE_CHAR)
	        {
		        player_start_x = i;
		        player_start_y = rawBoard.size();

		        // It IS an empty space, after all...
		        rawBoardLine.push_back(false);
	        }
	        else
		        rawBoardLine.push_back(current_line[i] == WALL_CHAR);
        }

        // Commit this line to the level
        rawBoard.push_back(rawBoardLine);
    }

    // I know it's counter-intuitive, but the width
    // and height is just like this
    int board_width  = rawBoard[0].size();
    int board_height = rawBoard.size();

    // Ncurses::exit();
    // std::cout << board_width << ' ' << board_height << std::endl;
    // exit(0);

    Board* board = new Board(board_width,
                             board_height,
                             ((Globals::Game::teleport) ?
                              Board::TELEPORT :
                              Board::SOLID),
                             player_start_x,
                             player_start_y);

    board->setBoard(rawBoard);
    return board;
}

// int BoardParser::getStartingX(int player)
// {
//     if (player >= MAX_NUMBER_OF_PLAYERS)
//         return 0;

//     return (this->startingPosition[player].x);
// }
// int BoardParser::getStartingY(int player)
// {
//     if (player >= MAX_NUMBER_OF_PLAYERS)
//         return 0;

//     return (this->startingPosition[player].y);
// }
// int BoardParser::getPlayers()
// {
//     return (this->players);
// }
// std::string BoardParser::getName()
// {
//     return (this->name);
// }

