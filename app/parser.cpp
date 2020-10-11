//
// Created by maxim on 27.04.2020.
//

#include "parser.h"
#include <jansson.h>
#include "dto.h"

LoginPasswordDto *createLoginPasswordDtoFromJson(const std::string &loginPasswordRaw) {
    // TODO чистить строку в опасности sql-injection
    json_error_t error;
    json_t *loginPasswordJson = json_loads(loginPasswordRaw.c_str(), 0, &error);
    auto *loginPasswordDto = new LoginPasswordDto;
    if (!loginPasswordJson) {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        return nullptr;
    }
    //получаем логин
    json_t *login = json_object_get(loginPasswordJson, "login");
    if (!json_is_string(login)) {
        fprintf(stderr, "error: login is not a string\n");
        json_decref(loginPasswordJson);
        return nullptr;
    }
    loginPasswordDto->login = json_string_value(login);
    delete login;
    //получаем пароль
    json_t *password = json_object_get(loginPasswordJson, "password");
    if (!json_is_string(password)) {
        fprintf(stderr, "error: password is not a string\n");
        json_decref(loginPasswordJson);
        return nullptr;
    }
    loginPasswordDto->password = json_string_value(password);
    delete password;
    json_decref(loginPasswordJson);
    return loginPasswordDto;
};

CreateUserDto *createUserDtoFromJson(const std::string &createUserJsonStr) {
    // TODO чистить строку в опасности sql-injection
    json_error_t error;
    json_t *createUserJson = json_loads(createUserJsonStr.c_str(), 0, &error);
    auto *createUserDto = new CreateUserDto;
    if (!createUserJson) {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        return nullptr;
    }
    //получаем логин
    json_t *login = json_object_get(createUserJson, "login");
    if (!json_is_string(login)) {
        fprintf(stderr, "error: login is not a string\n");
        json_decref(createUserJson);
        return nullptr;
    }
    createUserDto->login = json_string_value(login);
    delete login;
    //получаем пароль
    json_t *password = json_object_get(createUserJson, "password");
    if (!json_is_string(password)) {
        fprintf(stderr, "error: password is not a string\n");
        json_decref(createUserJson);
        return nullptr;
    }
    createUserDto->password = json_string_value(password);
    delete password;
    //получаем повтор пароля
    json_t *rePassword = json_object_get(createUserJson, "rePassword");
    if (!json_is_string(rePassword)) {
        fprintf(stderr, "error: rePassword is not a string\n");
        json_decref(createUserJson);
        return nullptr;
    }
    createUserDto->rePassword = json_string_value(rePassword);
    delete rePassword;
    //получаем имя
    json_t *firstName = json_object_get(createUserJson, "firstName");
    if (!json_is_string(firstName)) {
        fprintf(stderr, "error: firstName is not a string\n");
        json_decref(createUserJson);
        return nullptr;
    }
    createUserDto->firstName = json_string_value(firstName);
    delete firstName;
    //получаем фамилию
    json_t *lastName = json_object_get(createUserJson, "lastName");
    if (!json_is_string(lastName)) {
        fprintf(stderr, "error: lastName is not a string\n");
        json_decref(createUserJson);
        return nullptr;
    }
    createUserDto->lastName = json_string_value(lastName);
    delete lastName;

    return createUserDto;
}

TweetDto *createTweetDtoFromJson(const std::string &tweetRaw) {
    // TODO чистить строку в опасности sql-injection
    json_error_t error;
    json_t *createTweetJson = json_loads(tweetRaw.c_str(), 0, &error);
    if (!createTweetJson) {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        return nullptr;
    };
    auto tweetDto = new TweetDto;
    //получаем заголовок
    json_t *title = json_object_get(createTweetJson, "title");
    if (!json_is_string(title)) {
        fprintf(stderr, "title: is not a string");
        json_decref(createTweetJson);
        return nullptr;
    }
    tweetDto->title = json_string_value(title);
    delete title;
    //получаем тело
    json_t *body = json_object_get(createTweetJson, "body");
    if (!json_is_string(body)) {
        fprintf(stderr, "body: is not a string");
        json_decref(createTweetJson);
        return nullptr;
    }
    tweetDto->body = json_string_value(body);
    delete body;

    return tweetDto;
}