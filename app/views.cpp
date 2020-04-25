//
// Created by maxim on 19.04.2020.
//
#include "string"
#include "views.h"
#include "regex"
#include "dto.h"
#include "repository.h"
#include "memory"
#include "serializers.h"

void BaseView::notAllowed(struct evhttp_request *request, void *ctx) {
    // если такого урла нет
    struct evbuffer *evb = evbuffer_new();
    evhttp_add_header(request->output_headers, "Content-Type", "text/html");
    evhttp_send_reply(request, HTTP_BADMETHOD, "HTTP_BADMETHOD", evb);
    evbuffer_free(evb);
};

void notFound(evhttp_request *request, void *ctx) {
    // 404
    evbuffer *evb = evbuffer_new();
    evhttp_add_header(request->output_headers, "Content-Type", "application/json");
    evbuffer_add_printf(evb, "%s", "{}");
    evhttp_send_reply(request, HTTP_NOTFOUND, "HTTP_NOTFOUND", evb);
    evbuffer_free(evb);
}

void BaseView::asView(struct evhttp_request *request, void *ctx) {
    // дабвляем базу данных в контекст,
    // по ходу из-за каких-то ограничений приходится ее открывать/закрывать при каждом запросе
    sqlite3 *db;
    int rc;
    sqlite3_initialize();
    sqlite3_open("./test.db", &db);  // TODO путь в константы
    auto *context = (CbContext *) ctx;
    context->db = db;

    // обработка запроса в зависимости от типа
    switch (request->type) {
        case EVHTTP_REQ_GET:
            this->GET(request, context);
            break;
        case EVHTTP_REQ_POST:
            this->POST(request, context);
            break;
        case EVHTTP_REQ_DELETE:
            this->DELETE(request, context);
            break;
        default:
            this->notAllowed(request, context);
            break; // TODO залогировать нестандартное поведение
    }
    // закрываем базу
    sqlite3_close(db);
};

int AuthorDetailView::getAuthorIdFromURL(struct evhttp_request *request) {
    std::string path = evhttp_uri_get_path(request->uri_elems);
    int authorId = 0;

    try {
        std::regex re("^\\/author\\/([0-9]+)\\/$");
        std::smatch match;
        if (std::regex_search(path, match, re) && match.size() > 1) {
            //TODO хрупкий код, atoi может бросить ошибку
            authorId = std::stoi(match.str(1));
        } else {
            // TODO залогировать
        }
    } catch (std::regex_error &e) {
        // TODO залогировать
    }
    return authorId;
};

void AuthorDetailView::GET(struct evhttp_request *request, void *ctx) {
    auto *context = (CbContext *) ctx;
    // получаем id автора
    int authorId = this->getAuthorIdFromURL(request);
    evbuffer *evb = evbuffer_new();
    if (authorId == 0) {
        // если authorId кривой
        notFound(request, ctx);
        return;
    }
    // извлекаем его из базы
    std::unique_ptr<AuthorRepository> authorRepository(new AuthorRepository(context->db));
    User *author = authorRepository->getOnlyAuthorPublicInfoById(long(authorId));
    if (author == nullptr) {
        // если на нашло автора
        notFound(request, ctx);
        return;
    }
    std::string serializedAuthor = getUserPublicInfoJson(author);
    evhttp_add_header(request->output_headers, "Content-Type", "application/json");

    evbuffer_add_printf(evb, "%s", serializedAuthor.c_str());
    evhttp_send_reply(request, HTTP_OK, "HTTP_OK", evb);
    evbuffer_free(evb);
    delete author;
};

void AuthorDetailView::POST(struct evhttp_request *request, void *ctx) {

};

void AuthorDetailView::DELETE(struct evhttp_request *request, void *ctx) {

};