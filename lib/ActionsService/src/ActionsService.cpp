#include <ActionsService.h>
#include <esp_log.h>

char TAG[] = "ACTIONS";

void ActionsService::handle(std::string topic, std::string payload) {
    if (handlers.find(topic) == handlers.end()) {
        ESP_LOGD(TAG, "action %s does not exists", topic.c_str());
        return;
    }
    handlers[topic](payload);
}

void ActionsService::registerAction(std::string topic, std::function<void(std::string)> handler) {
    if (handlers.find(topic) != handlers.end()) {
        ESP_LOGD(TAG, "action %s already exists", topic.c_str());
        return;
    }
    handlers[topic] = handler;
}
