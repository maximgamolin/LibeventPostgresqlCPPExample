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

class BaseRepository {
protected:
    PGconn *db;
public:
    explicit BaseRepository(PGconn *db);
};

class AuthorRepository : public BaseRepository {
public:
    using BaseRepository::BaseRepository;

    User *getOnlyAuthorPublicInfoById(long int authorId);

    User *registerUser(CreateUserDto *createUserDto);

    RequestUser *getRequestUserBySession(std::string token);

    ListOfUsers *getListOfUsers(int offset, int limit);

    int deleteUserById(long int authorId);

    unsigned int getUserIdByLoginPassword(const std::string &login, const std::string &pass);
};


class SessionRepository : public BaseRepository {
public:
    using BaseRepository::BaseRepository;

    void setSessionByUserId(unsigned int userId, const std::string &token);
};

#endif //TIWTTER_REPOSITORY_H

