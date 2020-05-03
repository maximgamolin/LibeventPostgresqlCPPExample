//
// Created by maxim on 18.04.2020.
//

#include "sqlite3.h"
#include "entities.h"
#include "dto.h"

#ifndef TIWTTER_REPOSITORY_H
#define TIWTTER_REPOSITORY_H
enum ut {
    USER_TABLE_ID, USER_TABLE_LOGIN, USER_TABLE_PASSWORD,
    USER_TABLE_FIRST_NAME, USER_TABLE_LAST_NAME, USER_TABLE_REGISTERED_AT
};

class AuthorRepository {
private:
    PGconn *db;
public:
    explicit AuthorRepository(PGconn *db);

    User *getOnlyAuthorPublicInfoById(long int authorId);

    User *registerUser(CreateUserDto *createUserDto);
};

#endif //TIWTTER_REPOSITORY_H

