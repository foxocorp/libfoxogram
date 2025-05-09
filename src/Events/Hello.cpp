#include <foxogram/Events.h>
#include <foxogram/Me.h>
#include <foxogram/Logger.h>
#include <foxogram/Utils.h>

void foxogram::events::Hello::handle(foxogram::Me *me, nlohmann::json j, const std::string raw) {
    me->gateway.heartbeatInterval = Utils::value<int>(j.at("d"), "heartbeat_interval", 0);
    me->gateway.running = true;
    me->gateway.pingThread = std::thread([me](){me->gateway.ping(me->gateway.heartbeatInterval);});
    Hello_t e;
    e.me = me;
    e.rawEvent = raw;
    e.interval = me->gateway.heartbeatInterval;
    if (callback != nullptr) {
        callback(e);
    }
}