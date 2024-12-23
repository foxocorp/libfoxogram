#pragma once

#include <foxogram/export.h>
#include <foxogram/User.h>
#include <foxogram/Enums.h>

namespace foxogram {
    struct Channel;

    struct LIBFOXOGRAM_EXPORT Member : User {
        friend class Me;

    private:
        Channel *channel;

        FlagsBase<MemberPermissions> permissions;

    protected:
        std::string token;

    public:
        void addPermissions(MemberPermissions permission);

        void removePermissions(MemberPermissions permission);

        [[nodiscard]] bool hasPermissions(MemberPermissions permission) const;

        Member(Channel *channel, long long permissions, const std::string &username,
               const std::string &avatar, long long flags, int type, long long createdAt);

        [[nodiscard]] std::string getIdOrName() const override;
    };
}
