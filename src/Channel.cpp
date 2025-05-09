#include <foxogram/Channel.h>
#include <foxogram/HttpClient.h>
#include <foxogram/exceptions.h>
#include <foxogram/Utils.h>
#include <utility>
#include <foxogram/Logger.h>

foxogram::Channel::Channel(long long id, std::string name, std::string displayName, short type,
                           std::string ownerName, long long createdAt, std::string icon) : BaseEntity(id), name(std::move(name)),
                                                                                           type(type), ownerName(std::move(ownerName)), createdAt(createdAt), displayName(std::move(displayName)), icon(std::move(icon)) {
    messages = std::make_shared<foxogram::Cache<Message>>();
    members = std::make_shared<foxogram::Cache<Member>>();
}

void foxogram::Channel::handleError(const nlohmann::json &response) const {
    if (!response.value("ok", true)) {
        switch (Utils::value<int>(response, "code", 0)) {
            case 301: throw UserUnauthorizatedException();
            case 302: throw UserEmailNotVerfiedException();
            case 401: throw MissingPermissionException();
            case 403: throw MemberInChannelNotFoundException();
            default: throw HttpException(Utils::value<std::string>(response, "message", ""));
        }
    }
}

bool foxogram::Channel::deleteChannel() {
    auto j = HttpClient::request(Payload("DELETE", "/channels/" + std::to_string(id), token));
    handleError(j);
    return Utils::value<bool>(j, "ok", true);
}

void foxogram::Channel::edit(const std::string& displayName, const std::string& name, const std::string& icon) {
    auto j = HttpClient::request(Payload("PATCH", "/channels/" + std::to_string(id), nlohmann::json{
        {"displayName", displayName},
        {"name", name},
        {"icon", icon}
    }, token));
    handleError(j);
    this->displayName = displayName;
    this->name = name;
    this->icon = icon;
}

bool foxogram::Channel::leave() {
    auto j =HttpClient::request(Payload("DELETE", "/channels/" + std::to_string(id) + "/member/@me", token));
    handleError(j);
    return Utils::value<bool>(j, "ok", true);
}

std::list<foxogram::MessagePtr> foxogram::Channel::getMessages() const {
    std::list<foxogram::MessagePtr> messageList;
    std::transform(messages->getMap().begin(), messages->getMap().end(),
        std::back_inserter(messageList), [](const std::pair<long long, std::shared_ptr<Proxy<Message>>>& p) {return p.second;});
    return messageList;
}

std::list<foxogram::MessagePtr> foxogram::Channel::fetchMessages(long long before, int limit) {
    const auto j = HttpClient::request(Payload("GET", "/channels/" + std::to_string(id) + "/messages?before=" +
        std::to_string(before) +"&limit=" + std::to_string(limit), token));
    if (!j.is_array()) {
        handleError(j);
    }
    for (auto& message : j) {
        auto member = foxogram::Member::fromJSON(message.at("author"), id);
        auto msg = foxogram::Message::fromJSON(message);
        msg->token = token;
        msg->author = members->store(member);
        messages->store(msg);
    }

    std::list<foxogram::MessagePtr> messageList;
    std::transform(messages->getMap().begin(), messages->getMap().end(),
        std::back_inserter(messageList), [](const std::pair<long long, std::shared_ptr<Proxy<Message>>>& p) {return p.second;});
    return messageList;
}

foxogram::MessagePtr foxogram::Channel::fetchMessage(long long id) {
    const auto j = HttpClient::request(
        Payload("GET", "/channels/" + std::to_string(this->id) + "/messages/" + std::to_string(id), token));

    handleError(j);
    auto msg = foxogram::Message::fromJSON(j);
    auto member = foxogram::Member::fromJSON(j.at("author"), id);
    msg->token = token;
    msg->author = members->store(member);
    return messages->store(msg);
}

std::list<foxogram::MemberPtr> foxogram::Channel::fetchMembers() {
    auto j = HttpClient::request(Payload("GET", "/channels/" + std::to_string(id) + "/members", token));
    handleError(j);
    for (const auto& member : j) {
        members->store(foxogram::Member::fromJSON(member, id));
    }
    std::list<foxogram::MemberPtr> memberList;
    std::transform(members->getMap().begin(), members->getMap().end(),
        std::back_inserter(memberList), [](const std::pair<long long, std::shared_ptr<Proxy<Member>>>& p) {return p.second;});
    return memberList;
}

foxogram::MemberPtr foxogram::Channel::fetchMember(long long id) {
    auto j = HttpClient::request(Payload("GET", "/channels/" + std::to_string(this->id)
    + "/members/" + std::to_string(id), token));
    handleError(j);
    auto member = foxogram::Member::fromJSON(j, id);
    return members->store(member);;
}

foxogram::MemberPtr foxogram::Channel::getMember(long long id) {
    return members->get(id);
}

foxogram::MessagePtr foxogram::Channel::createMessage(std::string content, const std::list<long long>& attachments) {
    const auto j = HttpClient::request(Payload("POST", "/channels/" + std::to_string(id) + "/messages",
        nlohmann::json({{"content", content}, {attachments, attachments}}), token));

    handleError(j);
    auto msg = foxogram::Message::fromJSON(j);
    auto member = foxogram::Member::fromJSON(j.at("author"), id);
    msg->token = token;
    msg->author = members->store(member);
    return messages->store(msg);
}

std::string foxogram::Channel::getName() const {
    return name;
}

int foxogram::Channel::getType() const {
    return type;
}

std::list<foxogram::MemberPtr> foxogram::Channel::getMembers() const {
    std::list<foxogram::MemberPtr> memberList;
    std::transform(members->getMap().begin(), members->getMap().end(),
        std::back_inserter(memberList), [](const std::pair<long long, std::shared_ptr<Proxy<Member>>>& p) {return p.second;});
    return memberList;
}

std::string foxogram::Channel::getOwnerName() const {
    return ownerName;
}

long long foxogram::Channel::getCreatedAt() const {
    return createdAt;
}

const std::string &foxogram::Channel::getDisplayName() const {
    return displayName;
}

const std::string &foxogram::Channel::getIcon() const {
    return icon;
}

std::shared_ptr<foxogram::Channel> foxogram::Channel::fromJSON(nlohmann::json j) {
    return std::make_shared<Channel>(Utils::value<long long>(j, "id", 0),
        Utils::value<std::string>(j, "name", ""),
        Utils::value<std::string>(j, "display_name", ""), Utils::value<short>(j, "type", 0),
        Utils::value<std::string>(j.at("owner"), "username", ""),
        Utils::value<long long>(j, "created_at", 0), Utils::value<std::string>(j, "icon", ""));
}