//
// Created by maxim on 27.04.2020.
//

#include "dto.h"

#ifndef TIWTTER_PARSER_H
#define TIWTTER_PARSER_H

CreateUserDto *createUserDtoFromJson(const std::string &createUserJson);

LoginPasswordDto *createLoginPasswordDtoFromJson(const std::string &loginPasswordRaw);

TweetDto *createTweetDtoFromJson(const std::string &tweetRaw);
#endif //TIWTTER_PARSER_H
