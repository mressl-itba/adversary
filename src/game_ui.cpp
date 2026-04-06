/**
 * The Adversary
 * Game UI mode
 * 
 * @details This module runs the game in UI mode.
 * @author The dark net programmers
 */

#include "ui/ui.h"
#include "game/agent.h"
#include "starter_agent/starter_agent.h"
#include "greedy_agent/greedy_agent.h"

/**
 * @brief Main entry point of the application
 */
int main(int argc, char *argv[])
{
    // Load the agents
    AgentInstance agentInstances[2];

    LoadAgentInstance(agentInstances[0], starter_agent);
    LoadAgentInstance(agentInstances[1], greedy_agent);

    // Initialize the UI
    LoadUI(agentInstances);

    while(true)
    {
        if (!UpdateUI())
            break;
    }

    // Unload the UI
    UnloadUI();

    // Unload the agents
    UnloadAgentInstance(agentInstances[0]);
    UnloadAgentInstance(agentInstances[1]);
}
