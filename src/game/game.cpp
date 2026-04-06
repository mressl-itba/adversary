/**
 * The Adversary
 * Game logic
 *
 * @details This module defines the game logic and data structures for Reversi.
 * @author The dark net programmers
 */

#include "game.h"

GameSquare GetGameSquare(const Game &game, GamePosition position)
{
    if (position.x < 0 ||
        position.y < 0 ||
        position.x >= BOARD_SIZE ||
        position.y >= BOARD_SIZE)
        return SQUARE_EMPTY;

    return game.board[position.x][position.y];
}

static void SetSquare(Game &game, GamePosition position, GameSquare square)
{
    if (position.x < 0 ||
        position.y < 0 ||
        position.x >= BOARD_SIZE ||
        position.y >= BOARD_SIZE)
        return;

    game.board[position.x][position.y] = square;
}

void InitializeGame(Game &game)
{
    for (int32_t x = 0; x < BOARD_SIZE; x++)
    {
        for (int32_t y = 0; y < BOARD_SIZE; y++)
        {
            SetSquare(game, {x, y}, SQUARE_EMPTY);
        }
    }

    // Standard Reversi opening
    SetSquare(game, {BOARD_SIZE / 2 - 1, BOARD_SIZE / 2 - 1}, SQUARE_WHITE);
    SetSquare(game, {BOARD_SIZE / 2 - 1, BOARD_SIZE / 2}, SQUARE_BLACK);
    SetSquare(game, {BOARD_SIZE / 2, BOARD_SIZE / 2 - 1}, SQUARE_BLACK);
    SetSquare(game, {BOARD_SIZE / 2, BOARD_SIZE / 2}, SQUARE_WHITE);

    game.phase = GAME_BLACK_PLAYS;
}

/**
 * @brief Gets the square representing the opponent player in the given game state.
 *
 * @param game The current game state.
 *
 * @return The square representing the opponent player (SQUARE_BLACK or SQUARE_WHITE).
 */
static GameSquare GetOpponentSquare(const Game &game)
{
    if (game.phase == GAME_BLACK_PLAYS)
        return SQUARE_WHITE;

    return SQUARE_BLACK;
}

/**
 * @brief Gets the square representing the current player in the given game state.
 *
 * @param game The current game state.
 *
 * @return The square representing the current player (SQUARE_BLACK or SQUARE_WHITE).
 */
static GameSquare GetPlayerSquare(const Game &game)
{
    if (game.phase == GAME_BLACK_PLAYS)
        return SQUARE_BLACK;

    return SQUARE_WHITE;
}

/**
 * @brief Checks if a move at the given start position can capture opponent pieces in the given direction.
 *
 * @param game The current game state.
 * @param start The position of the move being checked.
 * @param direction The direction to check for capturing opponent pieces. This should be a vector with components in the range [-1, 1], where (0, 0) is not a valid direction.
 *
 * @return true if the move at the start position can capture opponent pieces in the given direction, false otherwise.
 */
static bool CanCaptureInDirection(const Game &game, GamePosition start, GamePosition direction)
{
    if (GetGameSquare(game, start) != SQUARE_EMPTY)
        return false;

    GamePosition position = {start.x + direction.x, start.y + direction.y};
    GameSquare opponent_square = GetOpponentSquare(game);
    bool found_opponent = false;

    while (GetGameSquare(game, position) == opponent_square)
    {
        found_opponent = true;

        position.x += direction.x;
        position.y += direction.y;
    }

    return found_opponent && GetGameSquare(game, position) == GetPlayerSquare(game);
}

/**
 * @brief Flips the opponent's pieces in the given direction if the move at the start position is valid in that direction.
 *
 * @param game The current game state.
 * @param start The position of the move being played.
 * @param direction The direction to check for capturing opponent pieces. This should be a vector with components in the range [-1, 1], where (0, 0) is not a valid direction.
 */
static void FlipInDirection(Game &game, GamePosition start, GamePosition direction)
{
    if (!CanCaptureInDirection(game, start, direction))
        return;

    GamePosition position = {start.x + direction.x, start.y + direction.y};
    const GameSquare opponent_square = GetOpponentSquare(game);

    while (GetGameSquare(game, position) == opponent_square)
    {
        SetSquare(game, position, GetPlayerSquare(game));

        position.x += direction.x;
        position.y += direction.y;
    }
}

bool IsValidMove(const Game &game, GamePosition position)
{
    if (GetGameSquare(game, position) != SQUARE_EMPTY)
        return false;

    for (int32_t direction_x = -1; direction_x <= 1; direction_x++)
    {
        for (int32_t direction_y = -1; direction_y <= 1; direction_y++)
        {
            if (direction_x == 0 && direction_y == 0)
                continue;

            if (CanCaptureInDirection(game, position, {direction_x, direction_y}))
                return true;
        }
    }

    return false;
}

/**
 * @brief Checks if the current player has any valid move available in the given game state.
 *
 * @param game The current game state.
 *
 * @return true if the current player has at least one valid move available, false otherwise.
 */
static bool HasAnyValidMove(const Game &game)
{
    for (int32_t x = 0; x < BOARD_SIZE; x++)
    {
        for (int32_t y = 0; y < BOARD_SIZE; y++)
        {
            if (IsValidMove(game, {x, y}))
                return true;
        }
    }

    return false;
}

bool PlayMove(Game &game, GamePosition position)
{
    if (!IsValidMove(game, position))
        return false;

    for (int32_t direction_x = -1; direction_x <= 1; direction_x++)
    {
        for (int32_t direction_y = -1; direction_y <= 1; direction_y++)
        {
            if (direction_x == 0 && direction_y == 0)
                continue;

            FlipInDirection(game, position, {direction_x, direction_y});
        }
    }

    SetSquare(game, position, GetPlayerSquare(game));
    game.phase = (game.phase == GAME_BLACK_PLAYS) ? GAME_WHITE_PLAYS : GAME_BLACK_PLAYS;

    if (!HasAnyValidMove(game))
        game.phase = (game.phase == GAME_BLACK_PLAYS) ? GAME_WHITE_PLAYS : GAME_BLACK_PLAYS;

    if (!HasAnyValidMove(game))
    {
        uint32_t black_count = GetGameScore(game, SQUARE_BLACK);
        uint32_t white_count = GetGameScore(game, SQUARE_WHITE);

        if (black_count > white_count)
            game.phase = GAME_BLACK_WINS;
        else if (black_count < white_count)
            game.phase = GAME_WHITE_WINS;
        else
            game.phase = GAME_DRAW;
    }

    return true;
}

void GetValidMoves(const Game &game, GameMoves &moves)
{
    moves.clear();

    for (int32_t x = 0; x < BOARD_SIZE; x++)
    {
        for (int32_t y = 0; y < BOARD_SIZE; y++)
        {
            if (IsValidMove(game, {x, y}))
                moves.push_back({x, y});
        }
    }
}

uint32_t GetGameScore(const Game &game, GameSquare square)
{
    uint32_t score = 0;

    for (int32_t x = 0; x < BOARD_SIZE; x++)
    {
        for (int32_t y = 0; y < BOARD_SIZE; y++)
        {
            if (GetGameSquare(game, {x, y}) == square)
                score++;
        }
    }

    return score;
}

GamePhase GetGamePhase(const Game &game)
{
    return game.phase;
}
