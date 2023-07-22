#include "chat.h"
#include "message.h"
#include <string>
#include <chrono>
#include <ctime>
#include <exception>


using namespace std;
using chrono::system_clock;

#pragma warning(disable : 4996) // Отключает предупреждение компилятора С4996, связанное с 'ctime'

class bad_login : public exception
{
public:
    const char* what() const noexcept override
    {
        return "Too many failed login attempts";
    }
};

class bad_password : public exception
{
public:
    const char* what() const noexcept override
    {
        return "Too many failed attempts to enter the system password";
    }
};

template <typename T> void Chat<T>::setCurrentUser(std::string user)
{
    currentUser = user;
}

template <typename T> void Chat<T>::getCurrentUser() const
{
    if (currentUser != nullptr) {
        cout << currentUser->get_login() << endl;
    }
    else cout << "No registered users" << endl;
}

template <typename T> void Chat<T>::loginOperation() {
    T login, password;

    map<std::string, std::string>::iterator it;

    for (size_t try_num = 0; try_num < 6; ++try_num)
    {
        cout << "Log in. Enter your username:" << endl;
        cin >> login;
        if (login != "")
        {
            break;
        }
        if (login == "" && try_num < 5)
        {
            cout << "Login not entered! Try again!" << try_num + 1 << endl;
        }
        else throw bad_login();
    }
    bool success = false;

    it = _users.find(login);

    if (it != _users.end())
    {
        success = true;
    }

    if (success) {
        for (size_t try_num = 0; try_num < 6; ++try_num)
        {
            cout << "Enter the password:" << endl;
            cin >> password;

            if (it->second == password)
            {
                break;
            }
            if (try_num < 5) {
                cout << "Invalid password! Try again" << endl;
            }
            else throw bad_password();
        }
        system("cls");
        cout << "Welcome, " << it->first << endl;
        this->setCurrentUser(it->first);
        cout << endl;
        this->userMenu();
    }
    else {
        cout << "Such a user is not registered" << endl;
    }
}

template <typename T> void Chat<T>::logoutOperation() {
    this->setCurrentUser(nullptr);
    system("cls");
}

template <typename T> void Chat<T>::addUser()
{
    T login, password;
    cout << "Registration of a new user. Enter your username:" << endl;
    cin >> login;
    bool error = true;
    if (login == "") cout << "You have not entered the data" << endl;
    else if (login == "all") cout << "The user 'all' is forbidden to use" << endl;
    else
    {
        error = false;

        map<std::string, std::string>::iterator it = _users.find(login);
        if (it != _users.end())
        {
            cout << "A user with this name is already registered";
            error = true;
        }
    }
    if (!error)
    {
        cout << "Enter the password:" << endl;
        cin >> password;
        _users.emplace(login, password);
        cout << "User added!" << endl;
    }
}

template <typename T> void Chat<T>::showUsersByLogin()
{
    int n_users = 0;
    for (map<string, std::string>::iterator it = _users.begin(); it != _users.end(); ++it)
    {
        if (it != _users.find(currentUser)) {
            cout << (*it).first << endl;
            n_users++;
        }
    }
}

template <typename T> void Chat<T>::createMessage(bool toAll)
{
    toAll = false;
    T from, to, text;
    system_clock::time_point value_t = system_clock::now();
    time_t timestamp = system_clock::to_time_t(value_t);
    if (toAll) {
        to = "all";
    }
    else
    {
        while (1) {
            cout << "Online Users: " << endl;
            showUsersByLogin();
            cout << "Enter the recipient's login: " << endl;
            cin >> to;
            if (to == currentUser) {
                cout << "You can't send a message to yourself" << endl;
            }
            else break;
        }
    }
    from = currentUser;
    cout << "Write your message, press 'enter' to send: " << endl;
    while (getline(cin, text))
    {
        if (text != "") break;
    }
    _messages.emplace_back(from, to, text, timestamp);
    cout << "Message '" << text << "' from the user <" << from << "> to the user <" << to << "> sent" << endl;
    userMenu();
}

template <typename T> void Chat<T>::showMessages(bool toAll)
{
    toAll = false;
    size_t message_num = 0;
    for (auto& text : _messages)
    {
        time_t x = text.getTime();
        bool toAll = false;
        if (toAll)
        {
            if (text.getTo() == "all") {
                cout << "<" << text.getFrom() << ">: " << "'" << text.getText() << "'" << " received in " << ctime(&x) << endl;
                message_num++;
            }
        }
        else
        {
            if (currentUser == text.getTo() && currentUser != text.getFrom()) {
                cout << "From <" << text.getFrom() << ">: " << "'" << text.getText() << "'" << " received in " << ctime(&x) << endl;
                message_num++;
            }
        }
    }
    if (!message_num) cout << "No messages" << endl;
}

template <typename T> void Chat<T>::showAllMessagesWith()
{
    T with;
    size_t message_num = 0;
    showUsersByLogin();
    cout << "Enter the recipient's login: " << endl;
    cin >> with;
    if (currentUser == with)
    {
        cout << "You enter your own login" << endl;
    }
    else
    {
        for (auto& text : _messages)
        {
            time_t x = text.getTime();
            if (currentUser == text.getFrom())
            {
                cout << text.getFrom() << " >>> " << text.getTo() << ": " << text.getText()
                    << " | received in " << ctime(&x) << endl;
                message_num++;
            }
            else if (currentUser == text.getTo())
            {
                cout << text.getTo() << " <<< " << text.getFrom() << ": " << text.getText()
                    << " | received in " << ctime(&x) << endl;
                message_num++;
            }
        }
    }
    if (!message_num) cout << "No messages" << endl;
}

template <typename T> void Chat<T>::sentMessages()
{
    for (auto& text : _messages)
        if (currentUser == text.getFrom())
        {
            auto x = text.getTime();
            auto y = ctime(&x);
            cout << "Message for <" << text.getTo() << ">: " << "'" << text.getText() << "'"
                << " received in " << y << endl;
        }
}

template <typename T> void Chat<T>::userMenu()
{
    while (!currentUser.empty())
    {
        char user_choice;
        cout << endl;
        cout << "\033[93m" << "************** Custom Menu: Select an option: **************" << endl;
        cout << "\033[93m" << "1 - Read new messages | 2 - Read messages from... | 3 - Read mailing lists" << endl;
        cout << "\033[93m" << "4 - Send a message | 5 - Send a newsletter | 6 - View sent messages" << endl;
        cout << "\033[93m" << "0 - Log Out / Start Over" << endl;
        cin >> user_choice;
        cout << endl;
        switch (user_choice)
        {
        case '1':
            showMessages(false);
            break;
        case '2':
            showAllMessagesWith();
            break;
        case '3':
            showMessages(true);
            break;
        case '4':
            createMessage(false);
            break;
        case '5':
            createMessage(true);
            break;
        case '6':
            sentMessages();
            break;
        case '7':
            logoutOperation();
            break;
        default:
            logoutOperation();
            cout << "Invalid input. Try again" << endl;
        }
    }
    cout << endl;
}

template <typename T> void Chat<T>::runChat()
{
    bool chat_enable = true;
    while (chat_enable)
    {
        system_clock::time_point today = system_clock::now();
        time_t tt;
        tt = system_clock::to_time_t(today);
        cout << "Today: " << ctime(&tt);
        char user_choise;
        cout << "\033[0m" << "*********** Start menu: Select the option: ***********" << endl;
        cout << "\033[0m" << "   1 - Registering a new user | 2 - Logging in | 0 - Logging out" << endl;
        cin >> user_choise;
        cout << endl;

        switch (user_choise)
        {
        case '1':
            addUser();
            break;
        case '2':
            loginOperation();
            break;
        case '0':
            cout << "Exit" << endl;
            chat_enable = false;
            break;
        default:
            cout << "Invalid input. Exit" << endl;
            chat_enable = false;
            break;
        }
    }
    cout << "The chat is over! See you again!" << endl;
}
