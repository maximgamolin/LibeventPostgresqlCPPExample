//
// Created by maxim on 19.04.2020.
//

#include "serializers.h"
#include "ctime"

std::string getUserPublicInfoJson(User *user) {
    std::string resultDate = std::asctime(&user->registeredAt);
    resultDate.pop_back();
    std::string result = "{"
                         "\"id\": " + std::to_string(user->id) + ","
                                                                 "\"login\": \"" + user->login + "\","
                                                                                                 "\"firstName\": \"" +
                         user->firstName + "\","
                                           "\"lastName\": \"" + user->lastName + "\","
                                                                                 "\"registeredAt\": \"" + resultDate +
                         "\""
                         "}";

    return result;
};