#include <iostream>
#include <cmath>
#include <vector>
#include <unistd.h>

class Ladder
{
public:
    int from;
    int to;

    Ladder(int, int);
};

Ladder::Ladder(int from, int to)
{
    this->from = from;
    this->to = to;
}

class Snake
{
public:
    int from;
    int to;

    Snake(int, int);
};

Snake::Snake(int from, int to)
{
    this->from = from;
    this->to = to;
}

class Player
{
public:
    int pos;
    int id;
    Player(int);
    // Player(int, int);
};

Player::Player(int id)
{
    this->pos = 1;
    this->id = id;

    std::cout << "Player spawned with id " << id << "\n";
}

// Player::Player(int initialPosition, int initialHealth)
// {
//     pos = initialPosition;
//     health = initialHealth;
// };

class Board
{
private:
    int rows, columns;

public:
    std::vector<Ladder> ladders;
    // std::vector<int> ladderStarts;
    std::vector<Snake> snakes;
    // std::vector<int> snakeStarts;
    int playerCount;
    std::vector<Player> players;
    int winnerID;
    int currentRound;

    Board(int, int, int, int, int);
    void print();

    // returns 0 if not found, 1 if found in positives, 2 if found in negatives
    int includes(int);

    // returns 0 if no one has won, 1 if a player wins, 2 if it's already won
    int nextRound();
    std::string getPosition(int);
    int getLadderTo(int);
    int getSnakeTo(int);
};

int Board::getLadderTo(int from)
{
    for (int i = 0; i < ladders.size(); i++)
    {
        Ladder ladder = ladders.at(i);
        if (ladder.from == from)
        {
            return ladder.to;
        }
    }

    throw std::out_of_range("Couldn't find the ladder");
}

int Board::getSnakeTo(int from)
{
    for (int i = 0; i < snakes.size(); i++)
    {
        Snake snake = snakes.at(i);
        if (snake.from == from)
        {
            return snake.to;
        }
    }
    throw std::out_of_range("Couldn't find the snake");
}

std::string Board::getPosition(int position)
{
    for (int i = 0; i < players.size(); i++)
    {
        Player p = players.at(i);
        if (p.pos == position)
        {
            return '[' + std::to_string(p.id) + ']';
        }
    }
    return std::to_string(position);
}

void Board::print()
{
    std::cout << "\n\n\n-=======================[Round " + std::to_string(this->currentRound) + "]=======================-\n";

    for (int i = 0; i < players.size(); i++)
    {
        Player p = players.at(i);
        std::cout << "Player " << p.id << " position: " << p.pos << "\n";
    }

    for (int i = rows - 1; i >= 0; i--)
    {
        if (!(i % 2))
        {
            for (int j = 1; j < columns + 1; j++)
            {
                int position = (10 * i) + j;
                std::string posStr = getPosition(position);
                std::cout << posStr << std::string(posStr == std::to_string(position) ? (int)(5 - floor(log10(position))) : (6 - posStr.length()), ' ');
                // std::cout << getPosition(position) << std::string((int)(5 - floor(log10(position))), ' ');
            }
        }
        else
        {
            for (int j = columns; j > 0; j--)
            {
                int position = (10 * i) + j;
                std::string posStr = getPosition(position);
                std::cout << posStr << std::string(posStr == std::to_string(position) ? (int)(5 - floor(log10(position))) : (6 - posStr.length()), ' ');
            }
        }
        std::cout << "\n";
    }

    std::cout << "-========================================================-\n";
}

int Board::includes(int i)
{
    int ret = 0;

    for (int i = 0; i < ladders.size(); i++)
    {
        Ladder ladder = ladders.at(i);
        if (ladder.from == i || ladder.to == i)
        {
            ret = 1;
        }
    }

    for (int i = 0; i < snakes.size(); i++)
    {
        Snake snake = snakes.at(i);
        if (snake.from == i || snake.to == i)
        {
            ret = 2;
        }
    }
    return ret;
}

Board::Board(int rows, int columns, int playerCount, int ladderCount, int snakeCount)
{
    this->rows = rows;
    this->columns = columns;
    currentRound = 0;

    for (int i = 0; i < ladderCount; i++)
    {
        int retries = 0;
        int randomFrom = rand() % (rows * columns - 3) + 1;
        while (includes(randomFrom) != 0)
        {
            if (++retries >= 100)
            {
                throw std::range_error("Probably in infinite loop: ladders length should be smaller than the board size");
            }
            randomFrom = rand() % (rows * columns - 3) + 1;
        }
        retries = 0;
        int randomTo = rand() % (rows * columns - randomFrom - 1) + randomFrom;
        while (includes(randomTo) != 0)
        {
            if (++retries >= 100)
            {
                throw std::range_error("Probably in infinite loop: ladders length should be smaller than the board size");
            }
            randomTo = rand() % (rows * columns - randomFrom - 1) + randomFrom;
        }

        ladders.push_back(Ladder(randomFrom, randomTo));
    }

    for (int i = 0; i < snakeCount; i++)
    {
        int retries = 0;
        int randomFrom = rand() % (rows * columns - 10) + 8;
        while (includes(randomFrom) != 0)
        {
            if (++retries >= 100)
            {
                throw std::range_error("Probably in infinite loop: snakes length should be smaller than the board size");
            }
            randomFrom = rand() % (rows * columns - 10) + 8;
        }
        retries = 0;
        int randomTo = rand() % (randomFrom - 2) + 1;
        while (includes(randomTo) != 0)
        {
            if (++retries >= 100)
            {
                throw std::range_error("Probably in infinite loop: snakes length should be smaller than the board size");
            }
            randomTo = rand() % (randomFrom - 2) + 1;
        }

        snakes.push_back(Snake(randomFrom, randomTo));
    }

    std::cout << "Ladders:";
    for (int i = 0; i < ladderCount; i++)
    {
        Ladder ladder = ladders.at(i);
        std::cout << " " << ladder.from << "->" << ladder.to;
    }
    std::cout << "\n";
    std::cout << "Snakes:";
    for (int i = 0; i < snakeCount; i++)
    {
        Snake snake = snakes.at(i);
        std::cout << " " << snake.from << "->" << snake.to;
    }
    std::cout << "\n";

    this->playerCount = playerCount;

    players = {};

    for (int i = 0; i < playerCount; i++)
    {
        players.push_back(Player(i));
    }
    std::cout << "Board init\n";

    print();
}

int Board::nextRound()
{
    this->currentRound++;
    for (int i = 0; i < players.size(); i++)
    {
        Player p = players.at(i);

        if (p.pos == rows * columns)
        {
            return 2;
        }

        int roll = rand() % 6 + 1;
        // std::cout << "Player " << players.at(i).id << " rolled " << roll << "\n";

        int nextPos = p.pos + roll;

        if (nextPos > rows * columns)
        {
            continue;
        }
        else if (nextPos == rows * columns)
        {
            winnerID = p.id;
            p.pos = nextPos;

            players.at(i) = p;
            return 1;
        }

        int includes = this->includes(nextPos);

        // if the current position has a ladder
        if (includes == 1)
        {
            p.pos = getLadderTo(nextPos);
        }
        // if the current position has a snake
        else if (includes == 2)
        {
            p.pos = getSnakeTo(nextPos);
        }
        else
        {
            p.pos = nextPos;
        }

        players.at(i) = p;
    }
    return 0;
}

int main()
{
    srand(time(0));
    Board board(12, 10, 50, 5, 5);

    sleep(5);

    while (!board.nextRound())
    {
        board.print();
        usleep(300000);
    }

    board.print();
    std::cout << "Player " << board.winnerID << " won!\n";

    return 0;
}