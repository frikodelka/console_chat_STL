#pragma once
#include "message.h"
#include <vector>
#include <map>

template <typename T> class Chat {
private:
    std::vector<Message> _messages;
    std::map <std::string, std::string> _users;
    std::string currentUser = "";

public:
    Chat() = default;
    virtual ~Chat() = default;
    Chat(Chat& _arr) = delete;
    Chat& operator=(Chat& _list) = delete;

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