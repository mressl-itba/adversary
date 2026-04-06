/**
 * The Adversary
 * Simple starter AI agent
 *
 * @details This module implements a starter AI agent for Reversi.
 * @author The dark net programmers
 */

// IMPORTANT: RENAME THIS FOLDER, FILES AND STRUCTURE TO SUIT YOUR PROJECT

#include "starter_agent.h"

/**
 * @brief Represents the state of the agent.
 */
struct StarterAgentState
{
    // Your agent's state variables go here.
    // For example, you could store the number of moves played, or any other information you think is relevant for your agent's decision-making process.
};

/**
 * @brief Initializes the agent's state. This function will be called by the game engine when the agent is loaded, and can be used to set up any necessary data structures or variables for the agent.
 *
 * @return A pointer to the agent's state, which will be passed to the get_next_move function. The agent can return nullptr if it doesn't need to maintain any state.
 */
void *LoadStarterAgent()
{
    // Initialize your agent's state here.
    // You can allocate memory for any state variables you need and return a pointer to the state.
    return new StarterAgentState();
}

/**
 * @brief Unloads the agent's state. This function will be called by the game engine when the agent is no longer needed, and can be used to free any resources allocated by the agent.
 *
 * @param state A pointer to the agent's state that was returned by the init function. The agent can use this pointer to free any resources allocated for its state.
 */
void UnloadStarterAgent(void *userdata)
{
    StarterAgentState *agentState = (StarterAgentState *)userdata;

    // Clean up any resources allocated for your agent's state here.
    delete agentState;
}

/**
 * @brief Gets the next move from the agent. This function will be called by the game engine when it is the agent's turn to play, and should return the position of the next move that the agent wants to play.
 *
 * @param userdata A pointer to the agent's state that was returned by the init function. The agent can use this pointer to access any state variables it has maintained.
 * @param game The current game state. The agent can use this information to analyze the board and decide on its next move.
 * @param stopRequested A function pointer that the agent should call periodically during its computation to check if a stop has been requested by the game engine. If this function returns true, the agent should stop its computation and return immediately the best move found so far.
 *
 * @return The position of the next move that the agent wants to play. If the agent has no valid moves, it should return an invalid position (e.g., {-1, -1}).
 */
GamePosition GetStarterAgentNextMove(void *userdata, Game &game, StopRequestedCallback stopRequested)
{
    StarterAgentState *agentState = (StarterAgentState *)userdata;

    // Get all valid moves for the current player
    GameMoves moves;
    GetValidMoves(game, moves);

    if (moves.empty())
        return {-1, -1}; // No valid moves, return an invalid position

    // For now, we will just return the first valid move.
    // You can replace this with your own logic to choose a move based on your agent's strategy.
    return moves[0];
}

/**
 * @brief The agent structure that will be used by the game engine to interact with your agent. This structure should be defined in the .cpp file for your agent, and should be initialized with pointers to your load, unload, and get_next_move functions.
 */
AgentDescriptor starter_agent = {
    "Starter agent", // Name of this agent
    LoadStarterAgent,
    UnloadStarterAgent,
    GetStarterAgentNextMove,
};
