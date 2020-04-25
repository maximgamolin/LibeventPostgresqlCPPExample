#include <iostream>
#include <evhttp.h>
#include <regex.h>
#include <fstream>
#include "memory"
#include "app/views.h"
#include "sqlite3.h"
#include "app/dto.h"

void pathFinder(struct evhttp_request *request, void *ctx) {
    printf("Request from: %s:%d URI: %s\n", request->remote_host, request->remote_port, request->uri);
    regex_t authorDetailPattern;
    int err, regerr;

    regcomp(&authorDetailPattern, "^\\/author\\/?[0-9]*\\/?$", REG_EXTENDED);


    const char *path = evhttp_uri_get_path(request->uri_elems);
    regmatch_t pm;
    //TODO обработка нескольких уролов в цикле
    regerr = regexec(&authorDetailPattern, path, 0, &pm, 0);
    if (regerr == 0) {
        std::unique_ptr<AuthorDetailView> authorDetailView(new AuthorDetailView());
        authorDetailView->asView(request, ctx);
        return;
    };

    notFound(request, ctx);
};

std::string getMigrations() {
    std::string result;
    std::ifstream file("migrations/init.sql");
    std::string line;
    while (std::getline(file, line)) {
        result += line;
    }
    return result;
}

int main(int argc, char *argv[]) {
    struct event_base *ev_base;
    struct evhttp *ev_http;
    sqlite3 *db;
    int rc;
    std::string migrations;
    char *zErrMsg = nullptr;

    ev_base = event_init();

    ev_http = evhttp_new(ev_base);
    if (evhttp_bind_socket(ev_http, "localhost", (u_short) 5555)) {
        printf("Failed to bind localhost:5555\n");
        exit(1);
    }
    // Содаем базу и применяем миграции TODO вынести в флаг
    migrations = getMigrations();
    sqlite3_initialize();
    rc = sqlite3_open("./test.db", &db); // TODO путь в константы
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }
    rc = sqlite3_exec(db, migrations.c_str(), nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Migrations applied successfully\n");
    }
    sqlite3_close(db);
    //Готовим контекс для колбека
    auto *cbContext = new CbContext;
    //Запускаем калбек
    evhttp_set_gencb(ev_http, pathFinder, cbContext);
    event_base_dispatch(ev_base);
    return 0;
}
