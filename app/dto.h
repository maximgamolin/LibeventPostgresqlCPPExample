//
// Created by maxim on 22.04.2020.
//
#include "string"
#include "vector"
#include <sqlite3.h>

#ifndef TIWTTER_DTO_H
#define TIWTTER_DTO_H

struct CbContext {
    sqlite3 *db;
};

struct CreateUserDto {
    std::string login;
    std::string password;
    std::string rePassword;
    std::string firstName;
    std::string lastName;
};

#endif //TIWTTER_DTO_H

