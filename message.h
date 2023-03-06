#pragma once
#include <iostream>
#include <string>
#include <ctime>


class Message
{
private:
    const std::string _from;
    const std::string _to;
    const std::string _text;
    const std::time_t _timestamp;


public:
    ~Message() = default;
    Message(const std::string& from, const std::string& to, const std::string& text, std::time_t timestamp)
        : _from(from), _to(to), _text(text), _timestamp(timestamp) {}

    const std::string getFrom() const { return _from; }
    const std::string getTo() const { return _to; }
    const std::string getText() const { return _text; }
    const std::time_t getTime() const { return _timestamp; }
};