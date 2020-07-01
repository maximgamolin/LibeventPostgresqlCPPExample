//
// Created by maxim on 19.04.2020.
//
#include "entities.h"
#include "string"
#include "const.h"

#ifndef TIWTTER_SERIALIZERS_H
#define TIWTTER_SERIALIZERS_H

std::string serializeUserPublicInfoJson(User *user);

std::string serializeUserListJson(ListOfUsers *listOfUsers);

std::string serializeTweetList(ListOfTweets *listOfTweets);

#endif //TIWTTER_SERIALIZERS_H



