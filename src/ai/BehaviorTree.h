#pragma once
#include <functional>
#include <memory>
#include <vector>

enum class Status { SUCCESS, FAILURE, RUNNING };

class BehaviorNode
{
public:
    virtual ~BehaviorNode() = default;
    virtual Status execute() = 0;
};

class Action : public BehaviorNode
{
public:
    using Func = std::function<Status()>;
    Action(Func func) : m_Func(func) {}
    Status execute() override { return m_Func(); }

private:
    Func m_Func;
};

class Condition : public BehaviorNode
{
public:
    using Func = std::function<bool()>;
    Condition(Func func) : m_Func(func) {}
    Status execute() override { return m_Func() ? Status::SUCCESS : Status::FAILURE; }

private:
    Func m_Func;
};

class Sequence : public BehaviorNode
{
public:
    void addChild(std::unique_ptr<BehaviorNode> child)
    {
        m_Children.push_back(std::move(child)); 
    }

    Status execute() override
    {
        for (auto& child : m_Children)
        {
            Status result = child->execute();
            if (result != Status::SUCCESS)
            {
                return result;
            }
        }
        return Status::SUCCESS;
    }

private:
    std::vector<std::unique_ptr<BehaviorNode>> m_Children;
};

class Selector : public BehaviorNode
{
public:
    void addChild(std::unique_ptr<BehaviorNode> child)
    {
        m_Children.push_back(std::move(child)); 
    }

    Status execute() override
    {
        for (auto& child : m_Children)
        {
            Status result = child->execute();
            if (result != Status::FAILURE)
            {
                return result;
            }
        }
        return Status::FAILURE;
    }

private:
    std::vector<std::unique_ptr<BehaviorNode>> m_Children;
};

class Inverter : public BehaviorNode
{
public:
    Inverter(std::unique_ptr<BehaviorNode> child) : m_Child(std::move(child)) {}
    Status execute() override
    {
        Status result = m_Child->execute();
        if (result == Status::SUCCESS) return Status::FAILURE;
        if (result == Status::FAILURE) return Status::SUCCESS;
        return result;
    }

private:
    std::unique_ptr<BehaviorNode> m_Child;
};

class BehaviorTree
{
public:
    BehaviorTree(std::unique_ptr<BehaviorNode> root) : m_Root(std::move(root)) {}
    Status execute() { return m_Root->execute(); }

private:
    std::unique_ptr<BehaviorNode> m_Root;
};
