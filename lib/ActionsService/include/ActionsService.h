#pragma once

#include <map>
#include <string>
#include <functional>

class ActionsService {
public:
    void handle(std::string topic, std::string payload);
    void registerAction(std::string topic, std::function<void(std::string)> handler);

private:
    std::map<std::string, std::function<void(std::string)>> handlers;
};