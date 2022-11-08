#include "StateStack.hpp"
#include <cassert>

StateStack::StateStack(State::Context context)
    : m_context(context)
{
}

void StateStack::Update(sf::Time dt)
{
    for (auto itr = m_stack.rbegin(); itr != m_stack.rend(); ++itr)
    {
        if (!(*itr)->Update(dt))
        {
            break;
        }
    }
    ApplyPendingChanges();
}

void StateStack::Draw()
{
    for (State::Ptr& state : m_stack)
    {
        state->Draw();
    }
}

void StateStack::HandleEvent(const sf::Event& event)
{
    for (auto itr = m_stack.rbegin(); itr != m_stack.rend(); ++itr)
    {
        if (!(*itr)->HandleEvent(event))
        {
            break;
        }
    }
    ApplyPendingChanges();
}

void StateStack::PushState(StateID state_id)
{
    m_pending_list.emplace_back(PendingChange(StackAction::kPush, state_id));
}

void StateStack::PopState()
{
    m_pending_list.emplace_back(PendingChange(StackAction::kPop));
}

void StateStack::ClearState()
{
    m_pending_list.emplace_back(PendingChange(StackAction::kClear));
}

bool StateStack::IsEmpty() const
{
    return m_stack.empty();
}

State::Ptr StateStack::CreateState(StateID state_id)
{
    auto found = m_state_factory.find(state_id);
    assert(found != m_state_factory.end());
    return found->second();
}

void StateStack::ApplyPendingChanges()
{
    for (PendingChange change : m_pending_list)
    {
        switch (change.action)
        {
        case StackAction::kPush:
            m_stack.emplace_back(CreateState(change.state_id));
            break;
        case StackAction::kPop:
            m_stack.pop_back();
            break;
        case StackAction::kClear:
            m_stack.clear();
            break;
        }
    }
    m_pending_list.clear();
}

StateStack::PendingChange::PendingChange(StackAction action, StateID stateID)
    : action(action)
    , state_id(stateID)
{

}