#include <iostream>
#include <evhttp.h>
#include <regex.h>
#include <fstream>
#include "app/views.h"
#include "app/dto.h"
#include "app/middleware.h"
#include "map"
#include "postgresql/libpq-fe.h"

void pathFinder(struct evhttp_request *request, void *ctx) {
    printf("Request from: %s:%d URI: %s\n", request->remote_host, request->remote_port, request->uri);
    regex_t authorDetailPattern;
    regmatch_t pm;
    int regerr;
    const char *path = evhttp_uri_get_path(request->uri_elems);
    //urls
    std::map<std::string, BaseView *> urls = {
            {std::string(R"(^\/author\/[0-9]+\/?$)"), new AuthorDetailView},
            {std::string("^\\/author\\/?$"),          new AuthroListView},
            {std::string("^\\/login\\/?$"),           new LoginView}
    };
    //middleware
    BaseMiddleware *middleware[1] = {
            new AuthMiddleware,
    };
    for (auto &middlewareItem : middleware) {
        middlewareItem->execute(request, ctx);
    };
    auto it = urls.begin();
    for (int i = 0; it != urls.end(); it++, i++) {
        regcomp(&authorDetailPattern, it->first.c_str(), REG_EXTENDED);
        regerr = regexec(&authorDetailPattern, path, 0, &pm, 0);
        if (regerr == 0) {
            it->second->asView(request, ctx);
            return;
        };
    };

    notFound(request, ctx);
};

static void
processNotice(void *arg, const char *message) {


}

static void
clearRes(PGresult *res) {
    PQclear(res);
    res = nullptr;
}

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
    std::string migrations;

    ev_base = event_init();

    ev_http = evhttp_new(ev_base);
    if (evhttp_bind_socket(ev_http, "0.0.0.0", (u_short) 5555)) {
        printf("Failed to bind 0.0.0.0:5555\n");
        exit(1);
    }
    // Содаем базу и применяем миграции TODO вынести в флаг
    migrations = getMigrations();
    PGconn *conn = PQconnectdb("user=twitter password=twitter host=127.0.0.1 dbname=twitter");
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cout << PQerrorMessage(conn) << std::endl;
        return 1;
    }

    PQsetNoticeProcessor(conn, processNotice, nullptr); // какой-то костыль
    PGresult *res = PQexec(conn, migrations.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cout << PQerrorMessage(conn) << std::endl;
        return 1;
    }
    clearRes(res);
    //Готовим контекс для колбека
    auto *cbContext = new CbContext;
    cbContext->db = conn;
    //Запускаем калбек
    evhttp_set_gencb(ev_http, pathFinder, cbContext);
    event_base_dispatch(ev_base);
    //тушим базу TODO (отлавливать ctrl+c)
    clearRes(res);
    return 0;
}
