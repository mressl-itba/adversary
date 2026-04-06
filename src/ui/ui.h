/**
 * The Adversary
 * Game UI mode
 * 
 * @details This module runs the game in UI mode.
 * @author The dark net programmers
 */

#ifndef UI_H_
#define UI_H_

#include "../game/agent.h"

/**
 * @brief Initializes the UI. This function should be called once at the beginning of the application.
 * 
 * @param instances A pointer to the array of agent instances that the UI can use to interact with the agents. The UI should not modify this array, but it can store the pointer for later use when it needs to call the agents' functions.
 */
void LoadUI(const AgentInstance *instances);

/**
 * @brief Unloads the UI. This function should be called once at the end of the application.
 */
void UnloadUI();

/**
 * @brief Updates the UI. This function should be called in a loop until it returns false, which indicates that the application should exit.
 * 
 * @return true if the application should continue running, false if it should exit.
 */
bool UpdateUI();

#endif // UI_H_
