/**
 * The Adversary
 * Game logic
 *
 * @details This module defines the game logic and data structures for Reversi.
 * @author The dark net programmers
 */

#ifndef GAME_H_
#define GAME_H_

#include <cstdint>
#include <vector>

#define BOARD_SIZE 8

/**
 * @brief Represents the state of a square on the board.
 */
enum GameSquare
{
    SQUARE_EMPTY = 0,
    SQUARE_BLACK = 1,
    SQUARE_WHITE = 2,
};

/**
 * @brief Represents the phase of the game.
 */
enum GamePhase
{
    GAME_BLACK_PLAYS = 0,
    GAME_WHITE_PLAYS = 1,
    GAME_BLACK_WINS = 2,
    GAME_WHITE_WINS = 3,
    GAME_DRAW = 4,
};

/**
 * @brief Represents a position on the board.
 */
struct GamePosition
{
    int32_t x;
    int32_t y;
};

/**
 * @brief Represents a list of valid moves.
 */
using GameMoves = std::vector<GamePosition>;

/**
 * @brief Represents the game state.
 */
struct Game
{
    GameSquare board[BOARD_SIZE][BOARD_SIZE];

    GamePhase phase;
};

/**
 * @brief Initializes the game state to the standard Reversi opening position.
 *
 * @param game The game state to initialize.
 */
void InitializeGame(Game &game);

/**
 * @brief Gets the current phase of the game.
 *
 * @param game The current game state.
 *
 * @return The current phase of the game.
 */
GamePhase GetGamePhase(const Game &game);

/**
 * @brief Gets the score of a player in the current game state.
 *
 * @param game The current game state.
 * @param square The square representing the player to get the score for (SQUARE_BLACK or SQUARE_WHITE).
 *
 * @return The score of the player in the current game state.
 */
uint32_t GetGameScore(const Game &game, GameSquare square);

/**
 * @brief Gets the square at a given position on the board.
 *
 * @param game The current game state.
 * @param position The position to get the square from.
 *
 * @return The square at the given position on the board.
 */
GameSquare GetGameSquare(const Game &game, GamePosition position);

/**
 * @brief Checks if a move is valid for the current player in the given game state.
 *
 * @param game The current game state.
 * @param position The position to check the move for.
 *
 * @return true if the move is valid, false otherwise.
 */
bool IsValidMove(const Game &game, GamePosition position);

/**
 * @brief Gets the list of valid moves for the current player in the given game state.
 *
 * @param game The current game state.
 * @param moves The list to store the valid moves in.
 */
void GetValidMoves(const Game &game, GameMoves &moves);

/**
 * @brief Plays a move for the current player in the given game state.
 *
 * @param game The current game state.
 * @param position The position to play the move at.
 *
 * @return true if the move is played successfully, false otherwise.
 */
bool PlayMove(Game &game, GamePosition position);

#endif // GAME_H_
