#pragma once

#include <foxogram/export.h>
#include <foxogram/User.h>
#include <foxogram/Channel.h>
#include <foxogram/Cache.h>

namespace foxogram {
    class LIBFOXOGRAM_EXPORT Me : public User {
        Cache<User>* userCache = new Cache<User>();
        Cache<Channel>* channelCache = new Cache<Channel>();
        std::string* token;
        [[nodiscard]] User fetchMe(std::string* token);
    public:
        explicit Me(const std::string& token);

        Me(const std::string& username, const std::string& email, const std::string& password);

        Me(const std::string& email, const std::string& password);

        Me(const Me &) = delete;

        Me(Me &&) = delete;

        Me(long long int id, std::string username, std::string avatar, long long int flags,
           int type, long long int createdAt) = delete;

        [[nodiscard]] UserPtr fetchUser(const std::string& username) const;

        [[nodiscard]] User fetchMe() const;

        static std::string login(std::string email, std::string password);

        static std::string signup(std::string username, std::string email, std::string password);

        bool verifyEmail(const std::string &code) const;

        bool resendEmail() const;

        bool deleteUser(std::string password) const;

        bool confirmDeleteUser(const std::string &code) const;

        ChannelPtr createChannel(std::string name, int type);

        ChannelPtr joinChannel(std::string name);

        [[nodiscard]] ChannelPtr fetchChannel(std::string name);

    protected:
        void handleError(const nlohmann::json &response) const override;
        static void _handleError(const nlohmann::json &response);
    };
}
