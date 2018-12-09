#pragma once

#include <string>

class BasePlayerState {
  public:
    virtual std::string to_string() const = 0;
    virtual ~BasePlayerState() = default;
};

class IdlePlayerState : public BasePlayerState {
  public:
    virtual std::string to_string() const override { return "Idle"; }
};

class MovingToPlayerState : public BasePlayerState {
  public:
    virtual std::string to_string() const override { return "Moving to "; }
};

class RotatingToPlayerState : public BasePlayerState {
  public:
    virtual std::string to_string() const override { return "Rotating to "; }
};

enum class player_states { IDLE, MOVING_TO, ROTATING_TO };
