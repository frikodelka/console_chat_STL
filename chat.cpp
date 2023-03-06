#include "chat.h"
#include "message.h"
#include <string>
#include <chrono>
#include <ctime>
#include <exception>

#pragma warning(disable : 4996)

using namespace std;
using chrono::system_clock;

class bad_login : public exception
{
public:
    const char* what() const noexcept override
    {
        return "������� ����� ��������� ������� ����� � �������";
    }
};

class bad_password : public exception
{
public:
    const char* what() const noexcept override
    {
        return "������� ����� ��������� ������� ����� ������ �������";
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
    else cout << "��� ������������������ �������������" << endl;
}

template <typename T> void Chat<T>::loginOperation() {
    T login, password;

    map<std::string, std::string>::iterator it;

    for (size_t try_num = 0; try_num < 6; ++try_num)
    {
        cout << "������� � �������. ������� �����:" << endl;
        cin >> login;
        if (login != "") 
        {
            break;
        }
        if (login == "" && try_num < 5) 
        {
            cout << "����� �� ������! ���������� ��� ���!" << try_num + 1 << endl;
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
            cout << "������� ������:" << endl;
            cin >> password;
            
            if (it->second == password)
            {
                break;
            }
            if (try_num < 5) {
                cout << "�������� ������! ���������� ��� ���" << endl;
            }
            else throw bad_password();
        }
        system("cls");
        cout << "����� ����������, " << it->first << endl;
        this->setCurrentUser(it->first);
        cout << endl;
        this->userMenu();
    }
    else {
        cout << "����� ������������ �� ���������������" << endl;
    }
}

template <typename T> void Chat<T>::logoutOperation() {
    this->setCurrentUser(nullptr);
    system("cls");
}

template <typename T> void Chat<T>::addUser()
{
    T login, password;
    cout << "����������� ������ ������������. ������� �����:" << endl;
    cin >> login;
    bool error = true;
    if (login == "") cout << "�� �� ����� ������" << endl;
    else if (login == "all") cout << "������������ 'all' ��������� ������������" << endl;
    else
    {
        error = false;
        
        map<std::string, std::string>::iterator it = _users.find(login);
        if (it != _users.end())
            {
                cout << "������������ � ����� ������ ��� ���������������";
                error = true;
            }
    }
    if (!error)
    {
        cout << "������� ������:" << endl;
        cin >> password;
        _users.emplace(login, password);
        cout << "������������ ��������" << endl;
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
            cout << "������������ ������:" << endl;
            showUsersByLogin();
            cout << "������� ����� ����������: " << endl;
            cin >> to;
            if (to == currentUser) {
                cout << "�� �� ������ ��������� ��������� ������ ����" << endl;
            }
            else break;
        }
    }
    from = currentUser;
    cout << "�������� ���� ���������, ������� 'enter' ��� ��������: " << endl;
    while (getline(cin, text))
    {
        if (text != "") break;
    }
    _messages.emplace_back(from, to, text, timestamp);
    cout << "��������� '" << text << "' �� ������������ <" << from << "> ������������ <" << to << "> ���������� " << endl;
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
                cout << "<" << text.getFrom() << ">: " << "'" << text.getText() << "'" << " �������� � " << ctime(&x) << endl;
                message_num++;
            }
        }
        else
        {
            if (currentUser == text.getTo() && currentUser != text.getFrom()) {
                cout << "�� <" << text.getFrom() << ">: " << "'" << text.getText() << "'" << " �������� � " << ctime(&x) << endl;
                message_num++;
            }
        }
    }
    if (!message_num) cout << "��� ���������" << endl;
}

template <typename T> void Chat<T>::showAllMessagesWith()
{
    T with;
    size_t message_num = 0;
    showUsersByLogin();
    cout << "������� ����� ����������: " << endl;
    cin >> with;
    if (currentUser == with)
    {
        cout << "�� ������� ���� ����������� �����" << endl;
    }
    else
    {
        for (auto& text : _messages)
        {
            time_t x = text.getTime();
            if (currentUser == text.getFrom())
            {
                cout << text.getFrom() << " >>> " << text.getTo() << ": " << text.getText()
                    << " | �������� � " << ctime(&x) << endl;
                message_num++;
            }
            else if (currentUser == text.getTo())
            {
                cout << text.getTo() << " <<< " << text.getFrom() << ": " << text.getText()
                    << " | �������� � " << ctime(&x) << endl;
                message_num++;
            }
        }
    }
    if (!message_num) cout << "��� ���������" << endl;
}

template <typename T> void Chat<T>::sentMessages()
{
    for (auto& text : _messages)
        if (currentUser == text.getFrom())
        {
            auto x = text.getTime();
            auto y = ctime(&x);
            cout << "��������� ��� <" << text.getTo() << ">: " << "'" << text.getText() << "'"
                << " �������� � " << y << endl;
        }
}

template <typename T> void Chat<T>::userMenu()
{
    while (!currentUser.empty())
    {
        char user_choice;
        cout << endl;
        cout << "\033[93m" << "************** ���������������� ����: �������� �����: **************" << endl;
        cout << "\033[93m" << "1 - ������ ����� ��������� | 2 - ������ ��������� ��... | 3 - ������ ��������" << endl;
        cout << "\033[93m" << "4 - ��������� ��������� | 5 - ��������� �������� | 6 - �������� ������������ ���������" << endl;
        cout << "\033[93m" << "0 - ����� �� ������� / ������ �������" << endl;
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
            cout << "�������� ����. ���������� �����" << endl;
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
        cout << "�������: " << ctime(&tt);
        char user_choise;
        cout << "\033[0m" << "*********** ��������� ����: �������� �����: ***********" << endl;
        cout << "\033[0m" << "   1 - ����������� ������ ������������ | 2 - ���� � ������� | 0 - �����" << endl;
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
            cout << "�����" << endl;
            chat_enable = false;
            break;
        default:
            cout << "�������� ����. �����" << endl;
            chat_enable = false;
            break;
        }
    }
    cout << "��� ��������! �������� �����!" << endl;
}