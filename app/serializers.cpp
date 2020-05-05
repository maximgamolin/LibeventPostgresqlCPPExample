//
// Created by maxim on 19.04.2020.
//
#include "serializers.h"
#include "ctime"
#include "vector"

std::string getUserPublicInfoJson(User *user) {
    std::string resultDate = std::asctime(&user->registeredAt);
    resultDate.pop_back();
    std::string result = "{"
                         "\"id\": " + std::to_string(user->id) + "," +
                         "\"login\": \"" + user->login + "\"," +
                         "\"firstName\": \"" + user->firstName + "\"," +
                         "\"lastName\": \"" + user->lastName + "\"," +
                         "\"registeredAt\": \"" + resultDate + "\""
                                                               "}";

    return result;
};

std::string __onlyUsersForJson(std::vector<User *> users) {
    int len = users.size();
    std::string result;
    for (int i = 0; i < len; i++) {
        result += getUserPublicInfoJson(users[i]);
        if (len - i > 1) {
            result += ",";
        }
    }
    return result;
}


std::string getUserListJson(ListOfUsers *listOfUsers) {
    int page = listOfUsers->limit / LIMIT_AUTHORS_PER_PAGE;
    bool hasNext = listOfUsers->limit < listOfUsers->count;
    std::string resultData = "{"
                             "\"page\":" + std::to_string(page) + "," +
                             "\"count\":" + std::to_string(listOfUsers->count) + "," +
                             "\"hasNext\":" + std::to_string(hasNext) + "," +
                             "\"page\":" + std::to_string(page) + "," +
                             "\"users\": [" + __onlyUsersForJson(listOfUsers->users) + "]" +
                             "}";
    return resultData;
};