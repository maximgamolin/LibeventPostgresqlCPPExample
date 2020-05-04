//
// Created by maxim on 18.04.2020.
//

#include <iostream>
#include "repository.h"
#include "string"

AuthorRepository::AuthorRepository(PGconn *db) {
    this->db = db;
}

User *AuthorRepository::getOnlyAuthorPublicInfoById(long int authorId) {
    std::string q = "SELECT "
                    "id, login, password, first_name, last_name, registered_at "
                    "FROM "
                    "author "
                    "WHERE "
                    "id = " + std::to_string(authorId) + ";";
    PGresult *res = PQexec(this->db, q.c_str());
    User *user = new User;
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        return nullptr;
    }
    int nrows = PQntuples(res);
    // TODO обработать строки больше одной и 0 строк
    for (int i = 0; i < nrows; i++) {
        user->id = atoi(PQgetvalue(res, i, USER_TABLE_ID));
        user->login = PQgetvalue(res, i, USER_TABLE_LOGIN);
        user->firstName = PQgetvalue(res, i, USER_TABLE_FIRST_NAME);
        user->lastName = PQgetvalue(res, i, USER_TABLE_LAST_NAME);
        strptime(
                (const char *) PQgetvalue(res, i, USER_TABLE_REGISTERED_AT),
                "%Y-%m-%d %H:%M:%S",
                &user->registeredAt
        );
    }
    return user;
};


User *AuthorRepository::registerUser(CreateUserDto *createUserDto) {
    // TODO Хранить хеш от пароля, а не пароль
    std::string q = "INSERT INTO \"author\" (login, password, first_name, last_name ) "
                    "VALUES ('" + createUserDto->login + "','"
                    + createUserDto->password + "','"
                    + createUserDto->firstName + "','"
                    + createUserDto->lastName + "') "
                                                "RETURNING id, login, password, first_name, last_name, registered_at ;";
    PGresult *res = PQexec(this->db, q.c_str());
    User *user = new User;
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        return nullptr;
    }
    int nrows = PQntuples(res);
    // TODO обработать строки больше одной и 0 строк
    for (int i = 0; i < nrows; i++) {
        //TODO хрупкий код atoi может бросить ошибку
        user->id = atoi(PQgetvalue(res, i, USER_TABLE_ID));
        user->login = PQgetvalue(res, i, USER_TABLE_LOGIN);
        user->firstName = PQgetvalue(res, i, USER_TABLE_FIRST_NAME);
        user->lastName = PQgetvalue(res, i, USER_TABLE_LAST_NAME);
        strptime(
                (const char *) PQgetvalue(res, i, USER_TABLE_REGISTERED_AT),
                "%Y-%m-%d %H:%M:%S",
                &user->registeredAt
        );
    }
    return user;
};

int AuthorRepository::deleteUserById(long int authorId) {
    std::string q = "DELETE FROM \"author\" WHERE id = " + std::to_string(authorId) + ";";
    PGresult *res = PQexec(this->db, q.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
        return 1;
    return 0;
};