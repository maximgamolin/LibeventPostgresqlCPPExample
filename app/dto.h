//
// Created by maxim on 22.04.2020.
//
#include "string"
#include "vector"
#include "entities.h"
#include <sqlite3.h>
#include "postgresql/libpq-fe.h"

#ifndef TIWTTER_DTO_H
#define TIWTTER_DTO_H

struct CbContext {
    PGconn *db;
    RequestUser *user;
};

struct CreateUserDto {
    std::string login;
    std::string password;
    std::string rePassword;
    std::string firstName;
    std::string lastName;
};

struct LoginPasswordDto {
    std::string login;
    std::string password;
};
#endif //TIWTTER_DTO_H

