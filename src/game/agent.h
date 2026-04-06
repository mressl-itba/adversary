/**
 * The Adversary
 * Agent interface
 *
 * @details This module defines the agent interface for Reversi agents.
 * @author The dark net programmers
 */

#ifndef AGENT_H
#define AGENT_H

#include "game.h"

/**
 * @brief Function pointer type for initializing an agent.
 *
 * @return A pointer to the agent's state, which will be passed to the get_next_move function. The agent can return nullptr if it doesn't need to maintain any state.
 */
typedef void *(*LoadCallback)();

/**
 * @brief Function pointer type for unloading an agent. This function will be called by the game engine when the agent is no longer needed, and can be used to free any resources allocated by the agent.
 *
 * @param state A pointer to the agent's state that was returned by the init function. The agent can use this pointer to free any resources allocated for its state.
 */
typedef void (*UnloadCallback)(void *userdata);

/**
 * @brief Function pointer type for checking if a stop has been requested by the game engine.
 *
 * @return true if a stop has been requested, false otherwise. If true, the agent should stop its computation and return immediately the best move found so far.
 */
typedef bool (*StopRequestedCallback)();

/**
 * @brief Function pointer type for getting the next move from an agent.
 *
 * @param userdata A pointer to user data that can be passed to the agent.
 * @param game The current game state.
 * @param stopRequested A function pointer that the agent should call periodically to check if a stop has been requested by the game engine. If this function returns true, the agent should stop its computation and return immediately the best move found so far.
 *
 * @return The position of the next move that the agent wants to play. If the agent has no valid moves, it should return an invalid position (e.g., {-1, -1}).
 */
typedef GamePosition (*GetNextMoveCallback)(void *userdata, Game &game, StopRequestedCallback stopRequested);

/**
 * @brief A struct to describe an agent, containing its name and the function pointers for loading, unloading, and getting the next move.
 */
struct AgentDescriptor
{
    const char *name;
    LoadCallback load;
    UnloadCallback unload;
    GetNextMoveCallback get_next_move;
};

/**
 * @brief A struct to hold an instance of each agent type.
 *
 * @param descriptor A pointer to the agent descriptor.
 * @param state A pointer to the agent's state. This will be set to the value returned by the agent's load function.
 */
struct AgentInstance
{
    AgentDescriptor *descriptor;
    void *state;
};

/**
 * @brief A struct to hold an instance of each agent type.
 *
 * @param instance A reference to the agent instance to be loaded.
 * @param descriptor A reference to the agent descriptor to be loaded.
 */
void LoadAgentInstance(AgentInstance &instance, AgentDescriptor &descriptor);

/**
 * @brief Unloads an agent instance, freeing any resources allocated by the agent.
 *
 * @param instance A reference to the agent instance to be unloaded.
 */
void UnloadAgentInstance(AgentInstance &instance);

#endif // AGENT_H
