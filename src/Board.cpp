#include "Board.hpp"

Board::Board(Rectangle box, bool haveBorders)
{
    this->clear(box);
    this->loadDefaultLevel();
    this->setBorders(haveBorders);
}
Board::~Board()
{ }
void Board::clear(Rectangle box)
{
    // Clearing board if it had any elements.
    for (unsigned int i = 0; i < (this->board.size()); i++)
        this->board[i].clear();

    this->board.clear();

    this->box = box;

    this->setSpeed(0);

    // Setting up 1st dimension
    this->board.resize(this->box.w);

    // Setting up 2nd dimension
    for (unsigned int i = 0; i < (this->board.size()); i++)
        this->board[i].resize(this->box.h);

    // All the tiles are initialized by default with Tile::EMPTY
}
void Board::loadDefaultLevel()
{
    // The default level is simply a map with this->box.w and
    // this->box.h, boxed in with borders.
    for (int i = 0; i < (this->box.w); i++)
    {
        for (int j = 0; j < (this->box.h); j++)
        {
            // All the borders at the extremes of the board
            if ((i == 0) || (i == (this->box.w - 1)) ||
                (j == 0) || (j == (this->box.h - 1)))
            {
                this->board[i][j].set(Tile::BORDER);
            }
        }
    }
}
Tile& Board::at(int x, int y)
{
    if ((x < 0) || (x >= this->box.w) ||
        (y < 0) || (y >= this->box.h))
        throw "Board::at Access to invalid index.";

    return (this->board[x][y]);
}
void Board::render()
{
    for (int i = 0; i < (this->box.w); i++)
        for (int j = 0; j < (this->box.h); j++)
            this->board[i][j].render(this->box.x + i,
                                     this->box.y + j);
}
int Board::getWidth()
{
    return (this->box.w);
}
int Board::getHeight()
{
    return (this->box.h);
}
void Board::setBorders(bool option)
{
    this->borders = option;

    // Refreshing the borders.
    for (int i = 0; i < (this->box.w); i++)
    {
        for (int j = 0; j < (this->box.h); j++)
        {
            if (this->board[i][j].has(Tile::BORDER) ||
                this->board[i][j].has(Tile::TELEPORT_BORDER))
            {
                if (this->borders)
                    this->board[i][j].set(Tile::BORDER);
                else
                    this->board[i][j].set(Tile::TELEPORT_BORDER);
            }
        }
    }
}
bool Board::hasBorders()
{
    return (this->borders);
}
int Board::maxLengthInsideMe()
{
    // We have a 1x1 border
    return (this->box.w - 1) * (this->box.h - 1);
}
bool Board::loadFile(std::string filename)
{
    bool result = this->level.load(filename);
    if (!result)
        return false;

    this->clear(Rectangle(this->box.x, this->box.y,
                          this->level.getWidth(), this->level.getHeight()));

    // now we parse the enum thingy into the actual board, nigga
    for (int i = 0; i < (this->box.w); i++)
        for (int j = 0; j < (this->box.h); j++)
            this->board[i][j].set(this->level.at(i, j));

    this->setBorders(this->borders);
    return true;
}
Level* Board::getLevel()
{
    return &(this->level);
}
bool Board::isInsideMap(int x, int y)
{
    Tile* tile = &(this->board[x][y]);

    if (tile->has(Tile::BORDER) ||
        tile->has(Tile::TELEPORT_BORDER))
        return true;

    // I will walk in all directions.
    // If I don't find a border in any of them, it's outside
    // the game map.
    //
    // (I know there are strange exceptions to this, but
    //  I can't find a simple way to do this.)
    //

    /// TODO A general pathfinding algorighm (A*?)

    Point origin(x, y);
    Point current = origin;

    // walking right
    while (!(this->board[current.x][current.y].isBorder()))
    {
        current.x++;
        if (current.x >= this->box.w)
            return false;
    }
    current = origin;

    // walking left
    while (!(this->board[current.x][current.y].isBorder()))
    {
        current.x--;
        if (current.x < 0)
            return false;
    }
    current = origin;

    // walking up
    while (!(this->board[current.x][current.y].isBorder()))
    {
        current.y--;
        if (current.y < 0)
            return false;
    }
    current = origin;

    // walking down
    while (!(this->board[current.x][current.y].isBorder()))
    {
        current.y++;
        if (current.y >= this->box.h)
            return false;
    }

    // Whew! If it got here, it means that exists borders
    // in ALL 4 sides of this point!
    //
    // Basically, it's in!
    return true;
}
int Board::getSupportedPlayers()
{
    return (this->level.getPlayers());
}
void Board::setSpeed(int speed)
{
    if ((speed) > 10) return;

    this->speed = speed;
    switch (speed)
    {
    case 0:  (this->timeout) = 800; break;
    case 1:  (this->timeout) = 600; break;
    case 2:  (this->timeout) = 400; break;
    case 3:  (this->timeout) = 300; break;
    case 4:  (this->timeout) = 250; break;
    case 5:  (this->timeout) = 200; break;
    case 6:  (this->timeout) = 150; break;
    case 7:  (this->timeout) = 100; break;
    case 8:  (this->timeout) = 75;  break;
    case 9:  (this->timeout) = 25;  break;
    case 10: (this->timeout) = 0;   break;
    default: break;
    }
}
int Board::getSpeed()
{
    return (this->speed);
}

void Board::increaseSpeed()
{
    this->setSpeed(this->speed + 1);
}
int Board::getTimeout()
{
    return (this->timeout);
}
std::string Board::getLevelName()
{
    return (this->level.getName());
}
void Board::setX(int x)
{
    this->box.x = x;
}
void Board::setY(int y)
{
    this->box.y = y;
}
