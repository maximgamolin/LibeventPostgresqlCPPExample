//
// Created by maxim on 18.04.2020.
//

#include "repository.h"
#include "sqlite3.h"
#include "string"

AuthorRepository::AuthorRepository(sqlite3 *db) {
    this->db = db;
}

User *AuthorRepository::getOnlyAuthorPublicInfoById(long int authorId) {
    std::string q = "SELECT "
                    "id, login, password, first_name, last_name, registered_at "
                    "FROM "
                    "user "
                    "WHERE "
                    "id = " + std::to_string(authorId) + ";";
    sqlite3_stmt *ppStmt = nullptr;
    // Запрос в базу
    int rc = sqlite3_prepare_v2(this->db, q.c_str(), -1, &ppStmt, nullptr);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(this->db));
        return nullptr;
    }
    // Собираем сущность пользователя
    User *user = new User;
    while (sqlite3_step(ppStmt) != SQLITE_DONE) {
        int num_cols = sqlite3_column_count(ppStmt);
        for (int i = 0; i < num_cols; i++) {
            switch (i) {
                case USER_TABLE_ID:
                    user->id = sqlite3_column_int(ppStmt, i);
                    break;
                case USER_TABLE_LOGIN:
                    user->login = (const char *) sqlite3_column_text(ppStmt, i);
                    break;
                case USER_TABLE_FIRST_NAME:
                    user->firstName = (const char *) sqlite3_column_text(ppStmt, i);
                    break;
                case USER_TABLE_LAST_NAME:
                    user->lastName = (const char *) sqlite3_column_text(ppStmt, i);
                case USER_TABLE_REGISTERED_AT:
                    strptime(
                            (const char *) sqlite3_column_text(ppStmt, i),
                            "%Y-%m-%d %H:%M:%S",
                            &user->registeredAt
                    );
                    break;
                default:
                    // TODO добавить логгирование нестандартного поведения
                    break;
            }
        }
        break; // TODO допилить обработку когда возвращается несколько или 0 пользователей, логгирование
    };
    sqlite3_finalize(ppStmt);
    return user;
}

//INSERT INTO "user" (login, password, first_name, last_name )
//VALUES ('bob', 'alligator43', 'bob', 'kelso');