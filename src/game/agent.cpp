/**
 * The Adversary
 * Agent interface
 *
 * @details This module defines the agent interface for Reversi agents.
 * @author The dark net programmers
 */

#include "agent.h"

void LoadAgentInstance(AgentInstance &instance, AgentDescriptor &descriptor)
{
    instance.descriptor = &descriptor;
    instance.state = descriptor.load();
}

void UnloadAgentInstance(AgentInstance &instance)
{
    instance.descriptor->unload(instance.state);
}
