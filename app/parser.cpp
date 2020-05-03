//
// Created by maxim on 27.04.2020.
//

#include "parser.h"
#include <jansson.h>
#include "dto.h"

CreateUserDto *createUserDtoFromJson(std::string createUserJsonStr) {
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