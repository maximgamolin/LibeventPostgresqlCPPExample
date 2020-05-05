//
// Created by maxim on 19.04.2020.
//
#include "entities.h"
#include "string"
#include "const.h"

#ifndef TIWTTER_SERIALIZERS_H
#define TIWTTER_SERIALIZERS_H

std::string getUserPublicInfoJson(User *user);

std::string getUserListJson(ListOfUsers *listOfUsers);
#endif //TIWTTER_SERIALIZERS_H



