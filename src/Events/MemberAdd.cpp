#include <foxogram/Events.h>
#include <foxogram/Me.h>

void foxogram::events::MemberAdd::handle(foxogram::Me *me, nlohmann::json j, const std::string raw) {
    MemberAdd_t e;
    e.me = me;
    e.rawEvent = raw;
    auto member = foxogram::Member::fromJSON(j);
    member->token = *me->token;
    auto channel = me->getChannel(j.at("channel").at("id").get<long long>());
    if (!channel) {
        auto channelPtr = foxogram::Channel::fromJSON(j.at("channel"));
        channelPtr->token = *me->token;
        me->channelCache->store(channelPtr);
    }

    e.member = channel->members->store(member);
    if (callback != nullptr) {
        callback(e);
    }
}