#pragma once
#include "message.h"
#include <vector>
#include <map>

template <typename T> class Chat {
private:
    std::vector<Message> _messages;
    std::map <std::string, std::string> _users;
    std::string currentUser = "";
    const std::string _login;
    std::string _password;

public:
    Chat() = default;
    virtual ~Chat() = default;
    Chat(const std::string& login, const std::string& password)
        : _login(login), _password(password) {}
    Chat(Chat& _arr) = delete;
    Chat& operator=(Chat& _list) = delete;

    const std::string get_login() { return _login; }
    bool pwdVerify(const std::string& password) const {
        if (password == _password)
            return true;
        return false;
    }

    void runChat();
    void userMenu();
    void addUser();
    void loginOperation();
    void logoutOperation();
    void showUsersByLogin();
    void setCurrentUser(std::string user);
    void getCurrentUser() const;

    void createMessage(bool toAll);
    void showMessages(bool toAll);
    void showAllMessagesWith();
    void sentMessages();

};
