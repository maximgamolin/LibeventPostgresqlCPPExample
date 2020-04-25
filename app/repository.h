//
// Created by maxim on 18.04.2020.
//

#include "sqlite3.h"
#include "entities.h"

#ifndef TIWTTER_REPOSITORY_H
#define TIWTTER_REPOSITORY_H
enum ut {
    USER_TABLE_ID, USER_TABLE_LOGIN, USER_TABLE_PASSWORD,
    USER_TABLE_FIRST_NAME, USER_TABLE_LAST_NAME, USER_TABLE_REGISTERED_AT
};

class AuthorRepository {
private:
    sqlite3 *db;
public:
    explicit AuthorRepository(sqlite3 *db);

    User *getOnlyAuthorPublicInfoById(long int authorId);

};

#endif //TIWTTER_REPOSITORY_H

