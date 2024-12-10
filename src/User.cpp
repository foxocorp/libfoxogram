#include <foxogram/User.h>
#include <utility>

foxogram::User::User(long long createdAt, std::string username, std::string avatar, long long int flags, int type): createdAt(createdAt),
    username(std::move(username)),
    avatar(std::move(avatar)), flags(flags),
    type(type) {}

void foxogram::User::handleError(const nlohmann::json &response) const {
}

void foxogram::User::addFlag(const UserFlags flag) {
    flags.addFlag(flag);
}

void foxogram::User::removeFlag(const UserFlags flag) {
    flags.removeFlag(flag);
}

bool foxogram::User::hasFlag(const UserFlags flag) const {
    return flags.hasFlag(flag);
}

const std::string &foxogram::User::getUsername() const {
    return username;
}

const std::string &foxogram::User::getAvatar() const {
    return avatar;
}

foxogram::FlagsBase<foxogram::UserFlags> foxogram::User::getFlags() const {
    return flags;
}

int foxogram::User::getType() const {
    return type;
}

long long int foxogram::User::getCreatedAt() const {
    return createdAt;
}
