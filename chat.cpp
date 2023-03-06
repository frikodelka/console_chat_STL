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
        return "Слишком много неудачных попыток входа в систему";
    }
};

class bad_password : public exception
{
public:
    const char* what() const noexcept override
    {
        return "Слишком много неудачных попыток ввода пароля систему";
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
    else cout << "Нет зарегестрированных пользователей" << endl;
}

template <typename T> void Chat<T>::loginOperation() {
    T login, password;

    map<std::string, std::string>::iterator it;

    for (size_t try_num = 0; try_num < 6; ++try_num)
    {
        cout << "Войдите в систему. Введите логин:" << endl;
        cin >> login;
        if (login != "") 
        {
            break;
        }
        if (login == "" && try_num < 5) 
        {
            cout << "Логин не введен! Попробуйте еще раз!" << try_num + 1 << endl;
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
            cout << "Введите пароль:" << endl;
            cin >> password;
            
            if (it->second == password)
            {
                break;
            }
            if (try_num < 5) {
                cout << "Неверный пароль! Попробуйте еще раз" << endl;
            }
            else throw bad_password();
        }
        system("cls");
        cout << "Добро пожаловать, " << it->first << endl;
        this->setCurrentUser(it->first);
        cout << endl;
        this->userMenu();
    }
    else {
        cout << "Такой пользователь не зарегестрирован" << endl;
    }
}

template <typename T> void Chat<T>::logoutOperation() {
    this->setCurrentUser(nullptr);
    system("cls");
}

template <typename T> void Chat<T>::addUser()
{
    T login, password;
    cout << "Регистрация нового пользователя. Введите логин:" << endl;
    cin >> login;
    bool error = true;
    if (login == "") cout << "Вы не ввели данные" << endl;
    else if (login == "all") cout << "Пользователя 'all' запрещено использовать" << endl;
    else
    {
        error = false;
        
        map<std::string, std::string>::iterator it = _users.find(login);
        if (it != _users.end())
            {
                cout << "Пользователь с таким именем уже зарегистрирован";
                error = true;
            }
    }
    if (!error)
    {
        cout << "Введите пароль:" << endl;
        cin >> password;
        _users.emplace(login, password);
        cout << "Пользователь добавлен" << endl;
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
            cout << "Пользователи онлайн:" << endl;
            showUsersByLogin();
            cout << "Введите логин получателя: " << endl;
            cin >> to;
            if (to == currentUser) {
                cout << "Вы не можете отправить сообщение самому себе" << endl;
            }
            else break;
        }
    }
    from = currentUser;
    cout << "Напишите свое сообщение, нажмите 'enter' для отправки: " << endl;
    while (getline(cin, text))
    {
        if (text != "") break;
    }
    _messages.emplace_back(from, to, text, timestamp);
    cout << "Сообщение '" << text << "' от пользователя <" << from << "> пользователю <" << to << "> отправлено " << endl;
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
                cout << "<" << text.getFrom() << ">: " << "'" << text.getText() << "'" << " получено в " << ctime(&x) << endl;
                message_num++;
            }
        }
        else
        {
            if (currentUser == text.getTo() && currentUser != text.getFrom()) {
                cout << "От <" << text.getFrom() << ">: " << "'" << text.getText() << "'" << " получено в " << ctime(&x) << endl;
                message_num++;
            }
        }
    }
    if (!message_num) cout << "Нет сообщений" << endl;
}

template <typename T> void Chat<T>::showAllMessagesWith()
{
    T with;
    size_t message_num = 0;
    showUsersByLogin();
    cout << "Введите логин получателя: " << endl;
    cin >> with;
    if (currentUser == with)
    {
        cout << "Вы вводите свой собственный логин" << endl;
    }
    else
    {
        for (auto& text : _messages)
        {
            time_t x = text.getTime();
            if (currentUser == text.getFrom())
            {
                cout << text.getFrom() << " >>> " << text.getTo() << ": " << text.getText()
                    << " | получено в " << ctime(&x) << endl;
                message_num++;
            }
            else if (currentUser == text.getTo())
            {
                cout << text.getTo() << " <<< " << text.getFrom() << ": " << text.getText()
                    << " | получено в " << ctime(&x) << endl;
                message_num++;
            }
        }
    }
    if (!message_num) cout << "Нет сообщений" << endl;
}

template <typename T> void Chat<T>::sentMessages()
{
    for (auto& text : _messages)
        if (currentUser == text.getFrom())
        {
            auto x = text.getTime();
            auto y = ctime(&x);
            cout << "Сообщение для <" << text.getTo() << ">: " << "'" << text.getText() << "'"
                << " получено в " << y << endl;
        }
}

template <typename T> void Chat<T>::userMenu()
{
    while (!currentUser.empty())
    {
        char user_choice;
        cout << endl;
        cout << "\033[93m" << "************** Пользовательское меню: Выберите опцию: **************" << endl;
        cout << "\033[93m" << "1 - Читать новые сообщения | 2 - Читать сообщения от... | 3 - Читать рассылки" << endl;
        cout << "\033[93m" << "4 - Отправить сообщение | 5 - Отправить рассылку | 6 - Просмотр отправленных сообщений" << endl;
        cout << "\033[93m" << "0 - Выход из системы / Начать сначала" << endl;
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
            cout << "Неверный ввод. Попробуйте снова" << endl;
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
        cout << "Сегодня: " << ctime(&tt);
        char user_choise;
        cout << "\033[0m" << "*********** Начальное меню: Выберите опцию: ***********" << endl;
        cout << "\033[0m" << "   1 - Регистрация нового пользователя | 2 - Вход в систему | 0 - Выход" << endl;
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
            cout << "Выход" << endl;
            chat_enable = false;
            break;
        default:
            cout << "Неверный ввод. Выход" << endl;
            chat_enable = false;
            break;
        }
    }
    cout << "Чат завершен! Увидимся снова!" << endl;
}