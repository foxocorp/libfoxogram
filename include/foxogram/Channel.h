#pragma once

#include <foxogram/export.h>
#include <string>
#include <list>
#include <foxogram/Member.h>
#include <foxogram/Message.h>
#include <foxogram/BaseEntity.h>
#include <foxogram/Cache.h>

namespace foxogram {
    struct LIBFOXOGRAM_EXPORT Channel : BaseEntity {
        friend class Me;
    private:
        std::string name;
        const int type;
        std::string ownerName;
        std::list<Member> members;
        std::list<Message> messages;
        const long long createdAt;
        std::shared_ptr<Cache<Message>> messageCache = std::make_shared<Cache<Message>>();

    protected:
        std::string token;

        void handleError(const nlohmann::json &response) const override;

    public:
        Channel(long long id, std::string name, short type, std::string ownerName, long long createdAt);

        void leave() const;

        void edit() const;

        void deleteChannel() const;

        [[nodiscard]] std::list<Message> getMessages() const;

        [[nodiscard]] Message getMessage(long long id) const;

        [[nodiscard]] Message createMessage() const;

        [[nodiscard]] const std::string &getName() const;

        [[nodiscard]] int getType() const;

        [[nodiscard]] std::string getOwnerName() const;

        [[nodiscard]] const std::list<Member> &getMembers() const;

    };
    using ChannelPtr = std::shared_ptr<Channel>;
}
