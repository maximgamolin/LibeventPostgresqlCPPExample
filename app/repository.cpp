//
// Created by maxim on 18.04.2020.
//

#include <vector>
#include "repository.h"
#include "string"

BaseRepository::BaseRepository(PGconn *db) {
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

ListOfUsers *AuthorRepository::getListOfUsers(int offset, int limit) {
    auto *listOfUsers = new ListOfUsers;
    int usersInArray;
    listOfUsers->offset = offset;
    listOfUsers->limit = limit;
    PGresult *res = PQexec(this->db, "SELECT COUNT(*) FROM \"author\"");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        return nullptr;
    }
    listOfUsers->count = atoi(PQgetvalue(res, 0, 0));
    std::string q = "SELECT "
                    "id, login, password, first_name, last_name, registered_at "
                    "FROM "
                    "author "
                    "OFFSET " + std::to_string(offset) + " LIMIT " + std::to_string(limit) + ";";
    PQclear(res);
    res = PQexec(this->db, q.c_str());
    int nrows = PQntuples(res);
    for (int i = 0; i < nrows; i++) {
        usersInArray = listOfUsers->users.size();
        listOfUsers->users.push_back(new User);
        listOfUsers->users[usersInArray]->id = atoi(PQgetvalue(res, i, USER_TABLE_ID));
        listOfUsers->users[usersInArray]->login = PQgetvalue(res, i, USER_TABLE_LOGIN);
        listOfUsers->users[usersInArray]->firstName = PQgetvalue(res, i, USER_TABLE_FIRST_NAME);
        listOfUsers->users[usersInArray]->lastName = PQgetvalue(res, i, USER_TABLE_LAST_NAME);
        strptime(
                (const char *) PQgetvalue(res, i, USER_TABLE_REGISTERED_AT),
                "%Y-%m-%d %H:%M:%S",
                &listOfUsers->users[usersInArray]->registeredAt
        );
    }
    return listOfUsers;
}

RequestUser *AuthorRepository::getRequestUserBySession(std::string token) {
    auto *user = new RequestUser;
    std::string q = "SELECT "
                    "u.id, u.login, u.password, u.first_name, u.last_name, u.registered_at "
                    "FROM "
                    "session s "
                    "JOIN "
                    "author u "
                    "ON "
                    "s.user_id = u.id "
                    "WHERE "
                    "s.token = '" + token + "';";
    PGresult *res = PQexec(this->db, q.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        return nullptr;
    }
    int nrows = PQntuples(res);
    if (nrows < 1) {
        user->isAnonymous = true;
        return user;
    }
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
}

unsigned int AuthorRepository::getUserIdByLoginPassword(const std::string &login, const std::string &pass) {
    unsigned int result = 0;
    std::string q = "SELECT "
                    "u.id "
                    "FROM "
                    "author u "
                    "WHERE "
                    "u.login = '" + login + "' AND u.password = '" + pass + "';";
    PGresult *res = PQexec(this->db, q.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        return 0;
    }
    int nrows = PQntuples(res);
    for (int i = 0; i < nrows; i++) {
        //TODO хрупкий код atoi может бросить ошибку
        result = atoi(PQgetvalue(res, i, USER_TABLE_ID));
    }
    return result;
}

void SessionRepository::setSessionByUserId(unsigned int userId, const std::string &token) {
    std::string q = "INSERT INTO session (token,"
                    "                     user_id) "
                    "VALUES ('" + token + "', " + std::to_string(userId) + ") ON conflict (user_id)"
                                                                           "DO UPDATE "
                                                                           "SET token = '" + token + "';";
    PQexec(this->db, q.c_str());
}

void SessionRepository::removeSessionByToken(const std::string token) {
    std::string q = "DELETE FROM session WHERE token = '" + token + "';";
    PQexec(this->db, q.c_str());
}

ListOfTweets *TweetRepository::getAllUserTweets(long userId, int offset, int limit) {
    auto *listOfTweets = new ListOfTweets;
    listOfTweets->limit = limit;
    listOfTweets->offset = offset;
    std::string q = "SELECT COUNT(*) FROM \"tweet\" "
                    "WHERE tw.user_id = " + std::to_string(userId);
    PGresult *count_res = PQexec(this->db, q.c_str());
    if (PQresultStatus(count_res) != PGRES_TUPLES_OK) {
        return nullptr;
    }
    listOfTweets->count = atoi(PQgetvalue(count_res, 0, 0));
    q = "SELECT tw.id, tw.title, tw.body, tw.created_at "
        "FROM tweet tw "
        "WHERE tw.user_id = " + std::to_string(userId) + " " +
        "OFFSET " + std::to_string(offset) + " LIMIT " + std::to_string(limit) + ";";
    PGresult *val_res = PQexec(this->db, q.c_str());
    if (PQresultStatus(val_res) != PGRES_TUPLES_OK) {
        return nullptr;
    }
    int nrows = PQntuples(val_res);
    // TODO обработать строки больше одной и 0 строк
    listOfTweets->tweets = new std::vector<Tweet *>();
    for (int i = 0; i < nrows; i++) {
        auto *tweet = new Tweet;
        //TODO хрупкий код atoi может бросить ошибку
        tweet->id = atoi(PQgetvalue(val_res, i, TWEET_ID));
        tweet->title = PQgetvalue(val_res, i, TWEET_TITLE);
        tweet->body = PQgetvalue(val_res, i, TWEET_BODY);
        strptime(
                (const char *) PQgetvalue(val_res, i, TWEET_CREATED_AT),
                "%Y-%m-%d %H:%M:%S",
                &tweet->createdAt
        );
        listOfTweets->tweets->push_back(tweet);
    }
    return listOfTweets;
}

ListOfTweets *TweetRepository::getAllTweets(int offset, int limit) {
    auto *listOfTweets = new ListOfTweets;
    listOfTweets->limit = limit;
    listOfTweets->offset = offset;
    std::string q = "SELECT COUNT(*) FROM \"tweet\" ";
    PGresult *count_res = PQexec(this->db, q.c_str());
    if (PQresultStatus(count_res) != PGRES_TUPLES_OK) {
        return nullptr;
    }
    listOfTweets->count = atoi(PQgetvalue(count_res, 0, 0));
    q = "SELECT tw.id, tw.title, tw.body, tw.created_at "
        "FROM tweet tw "
        "OFFSET " + std::to_string(offset) + " LIMIT " + std::to_string(limit) + ";";
    PGresult *val_res = PQexec(this->db, q.c_str());
    if (PQresultStatus(val_res) != PGRES_TUPLES_OK) {
        return nullptr;
    }
    int nrows = PQntuples(val_res);
    listOfTweets->tweets = new std::vector<Tweet *>();
    for (int i = 0; i < nrows; i++) {
        auto *tweet = new Tweet;
        //TODO хрупкий код atoi может бросить ошибку
        tweet->id = atoi(PQgetvalue(val_res, i, TWEET_ID));
        tweet->title = PQgetvalue(val_res, i, TWEET_TITLE);
        tweet->body = PQgetvalue(val_res, i, TWEET_BODY);
        strptime(
                (const char *) PQgetvalue(val_res, i, TWEET_CREATED_AT),
                "%Y-%m-%d %H:%M:%S",
                &tweet->createdAt
        );
        listOfTweets->tweets->push_back(tweet);
    }
    return listOfTweets;
}

void TweetRepository::createUserTweet(TweetDto *tweetDto, long userId) {
    std::string q = "INSERT INTO tweet (title, body, user_id) VALUES ('"
                    + tweetDto->title + "', '" + tweetDto->body + "', " + std::to_string(userId) + ");";
    PQexec(this->db, q.c_str());  // TODO отловить ошибки
}

void TweetRepository::deleteUserTweet(long tweetId, long userId) {
    std::string q = "DELETE FROM tweet WHERE user_id = " + std::to_string(userId)
                    + " AND  ID = " + std::to_string(tweetId) + ";";
    PQexec(this->db, q.c_str());  // TODO отловить ошибки
};