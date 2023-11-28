
#include <sstream>
#include "State.h"

State::State() = default;

State::State(int id, bool isAccepting, const std::string &token) {
    this->id = id;
    this->isAccepting = isAccepting;
    this->token = token;
}

State State::copy() const {
    return {getId(), this->isAccepting, this->token};
}

std::string State::getToken() const {
    return this->token;
}

void State::setToken(const std::string &t) {
    this->token = t;
}

int State::getId() const {
    return this->id;
}

void State::setId(int value) {
    this->id = value;
}

bool State::getIsAccepting() const {
    return this->isAccepting;
}

void State::setAccepting(bool value) {
    this->isAccepting = value;
}

bool State::operator==(const State &other) const {
    return (this->id == other.id);
//    && (this->isAccepting == other.isAccepting);
//    && (this->token == other.token);
}

bool State::operator!=(const State &other) const {
    return !(*this == other);
}

bool State::operator<(const State &other) const {
    if (this->id < other.id)
        return true;
    if (this->id > other.id)
        return false;
    return this->isAccepting < other.isAccepting;
//    if (this->isAccepting < other.isAccepting)
//        return true;
//    if (this->isAccepting > other.isAccepting)
//        return false;
//    return this->token < other.token;
}

bool State::operator>(const State &other) const {
    if (this->id > other.id)
        return true;
    if (this->id < other.id)
        return false;
    return this->isAccepting > other.isAccepting;
//    if (this->isAccepting > other.isAccepting)
//        return true;
//    if (this->isAccepting < other.isAccepting)
//        return false;
//    return this->token > other.token;
}

std::size_t State::hash() const {
    return std::hash<int>()(id);
//            ^std::hash<bool>()(isAccepting) ^
//           std::hash<std::string>()(token);
}

std::string State::toString() const {
    std::ostringstream oss;
    oss << "[" << id << "]";
    return oss.str();
}

std::string State::toStringFull() const {
    std::ostringstream oss;
    oss << "[" << id << ", " << this->isAccepting << ", " << this->token << "]";
    return oss.str();
}