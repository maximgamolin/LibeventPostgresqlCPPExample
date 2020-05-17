//
// Created by maxim on 18.04.2020.
//
#include "string"
#include "vector"

#ifndef TIWTTER_ENTITIES_H
#define TIWTTER_ENTITIES_H
struct Tweet {
    long int id;
    std::string title;
    std::string body;
    tm createdAt;
};

struct User {
    long int id;
    std::string login;
    std::string password;
    std::string firstName;
    std::string lastName;
    tm registeredAt;
    std::vector<Tweet> tweets;
};


struct ListOfUsers {
    int offset;
    int limit;
    int count;
    std::vector<User *> users;
};


struct RequestUser {
    long int id;
    std::string login;
    std::string firstName;
    std::string lastName;
    tm registeredAt;
    bool isAnonymous = false;
};
#endif //TIWTTER_ENTITIES_H



