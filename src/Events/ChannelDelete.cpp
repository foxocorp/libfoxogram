#include <foxogram/Events.h>
#include <foxogram/Me.h>

void foxogram::events::ChannelDelete::handle(foxogram::Me *me, nlohmann::json j, const std::string raw) {
    ChannelDelete_t e;
    e.me = me;
    e.rawEvent = raw;
    auto channel = foxogram::Channel::fromJSON(j);
    channel->token = *me->token;
    e.channel = me->channelCache->get(channel->getId());
    me->channelCache->Delete(channel->getId());
    if (callback != nullptr) {
        callback(e);
    }
}