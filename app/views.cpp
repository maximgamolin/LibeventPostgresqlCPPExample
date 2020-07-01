//
// Created by maxim on 19.04.2020.
//
#include <iostream>
#include "string"
#include "views.h"
#include "regex"
#include "dto.h"
#include "repository.h"
#include "memory"
#include "serializers.h"
#include "parser.h"
#include "const.h"
#include "utils.h"
#include "middleware.h"


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

    // обработка запроса в зависимости от типа
    switch (request->type) {
        case EVHTTP_REQ_GET:
            this->GET(request, ctx);
            break;
        case EVHTTP_REQ_POST:
            this->POST(request, ctx);
            break;
        case EVHTTP_REQ_DELETE:
            this->DELETE(request, ctx);
            break;
        default:
            this->notAllowed(request, ctx);
            break; // TODO залогировать нестандартное поведение
    }
};

void BaseView::GET(evhttp_request *request, void *ctx) {
    this->notAllowed(request, ctx);
}

void BaseView::POST(evhttp_request *request, void *ctx) {
    this->notAllowed(request, ctx);
}

void BaseView::DELETE(evhttp_request *request, void *ctx) {
    this->notAllowed(request, ctx);
}

void AuthorDetailView::GET(struct evhttp_request *request, void *ctx) {
    auto *context = (CbContext *) ctx;
    // получаем id автора
    int authorId = getAuthorIdFromUrl(request, "^\\/author\\/([0-9]+)\\/$");
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
    std::string serializedAuthor = serializeUserPublicInfoJson(author);
    evhttp_add_header(request->output_headers, "Content-Type", "application/json");

    evbuffer_add_printf(evb, "%s", serializedAuthor.c_str());
    evhttp_send_reply(request, HTTP_OK, "HTTP_OK", evb);
    evbuffer_free(evb);
    delete author;
};

void AuthorDetailView::DELETE(struct evhttp_request *request, void *ctx) {
    auto *context = (CbContext *) ctx;
    // получаем id автора
    int authorId = getAuthorIdFromUrl(request, "^\\/author\\/([0-9]+)\\/$");
    evbuffer *evb = evbuffer_new();
    if (authorId == 0) {
        // если authorId кривой
        notFound(request, ctx);
        return;
    }
    std::unique_ptr<AuthorRepository> authorRepository(new AuthorRepository(context->db));
    int resultOfDelete = authorRepository->deleteUserById(authorId);\
    if (resultOfDelete) {
        notFound(request, ctx);
        return;
    }
    evhttp_add_header(request->output_headers, "Content-Type", "application/json");

    evbuffer_add_printf(evb, "{}");
    evhttp_send_reply(request, HTTP_OK, "HTTP_OK", evb);
    evbuffer_free(evb);
};

void AuthroListView::GET(evhttp_request *request, void *ctx) {
    evbuffer *evb = evbuffer_new();
    auto *context = (CbContext *) ctx;
    int pageNum = 1;
    int offset, limit;
    struct evkeyvalq uri_params;
    evhttp_parse_query(request->uri, &uri_params);
    const char *rawPageNums = evhttp_find_header(&uri_params, "page");
    if (rawPageNums != nullptr) {
        //TODO atoi может упасть
        pageNum = atoi(rawPageNums);
    }
    limit = pageNum * LIMIT_AUTHORS_PER_PAGE;
    offset = limit - LIMIT_AUTHORS_PER_PAGE;

    std::unique_ptr<AuthorRepository> authorRepository(new AuthorRepository(context->db));
    ListOfUsers *listOfUsers = authorRepository->getListOfUsers(offset, limit);
    std::string serializedAuthorList = serializeUserListJson(listOfUsers);
    evhttp_add_header(request->output_headers, "Content-Type", "application/json");

    evbuffer_add_printf(evb, "%s", serializedAuthorList.c_str());
    evhttp_send_reply(request, HTTP_OK, "HTTP_OK", evb);
    evbuffer_free(evb);
    delete listOfUsers;
}

void AuthroListView::POST(struct evhttp_request *request, void *ctx) {
    auto *context = (CbContext *) ctx;
    evbuffer *evb = evbuffer_new();
    size_t len = evbuffer_get_length(request->input_buffer);
    auto *rawCreateUserJson = (char *) malloc(len + 1);
    rawCreateUserJson[len] = '\0';  // remove garbage;
    evbuffer_copyout(request->input_buffer, rawCreateUserJson, len);
    std::string createUserJson = rawCreateUserJson;
    std::cout << std::string(createUserJson) << std::endl;
    CreateUserDto *createUserDto = createUserDtoFromJson(std::string(createUserJson));
    //TODO завезти валидацию и проверку пароля
    std::unique_ptr<AuthorRepository> authorRepository(new AuthorRepository(context->db));
    User *author = authorRepository->registerUser(createUserDto);
    std::string serializedAuthor = serializeUserPublicInfoJson(author);
    evhttp_add_header(request->output_headers, "Content-Type", "application/json");

    evbuffer_add_printf(evb, "%s", serializedAuthor.c_str());
    evhttp_send_reply(request, HTTP_OK, "HTTP_OK", evb);
    evbuffer_free(evb);
    delete author;

};

void LoginView::invalidLoginOrPassword(evhttp_request *request, void *ctx) {
    evbuffer *evb = evbuffer_new();
    evhttp_add_header(request->output_headers, "Content-Type", "application/json");

    evbuffer_add_printf(evb, R"({"error": "Unknown user"})");
    evhttp_send_reply(request, 401, "HTTP_Unauthorized", evb);
    evbuffer_free(evb);
}

void LoginView::POST(evhttp_request *request, void *ctx) {
    auto *context = (CbContext *) ctx;
    evbuffer *evb = evbuffer_new();
    size_t len = evbuffer_get_length(request->input_buffer);
    auto *loginPassRawJson = (char *) malloc(len + 1);
    loginPassRawJson[len] = '\0';  // remove garbage;
    evbuffer_copyout(request->input_buffer, loginPassRawJson, len);
    auto *loginPasswordDto = createLoginPasswordDtoFromJson(loginPassRawJson);
    std::unique_ptr<AuthorRepository> authorRepository(new AuthorRepository(context->db));
    unsigned int userId = authorRepository->getUserIdByLoginPassword(
            loginPasswordDto->login,
            loginPasswordDto->password
    );
    if (userId == 0) {
        this->invalidLoginOrPassword(request, ctx);
        return;
    }
    std::unique_ptr<SessionRepository> sessionRepository(new SessionRepository(context->db));
    std::string token = AuthMiddleware::_buildCookie(request->remote_host);
    sessionRepository->setSessionByUserId(userId, token);
    evhttp_add_header(request->output_headers, "Content-Type", "application/json");
    evbuffer_add_printf(evb, "{}");
    evhttp_send_reply(request, HTTP_OK, "HTTP_OK", evb);
    evbuffer_free(evb);
};

void LogoutView::GET(evhttp_request *request, void *ctx) {
    auto *context = (CbContext *) ctx;
    evbuffer *evb = evbuffer_new();
    std::string token = AuthMiddleware::_buildCookie(request->remote_host);
    std::unique_ptr<SessionRepository> sessionRepository(new SessionRepository(context->db));
    sessionRepository->removeSessionByToken(token);
    evhttp_add_header(request->output_headers, "Content-Type", "application/json");
    evbuffer_add_printf(evb, "{}");
    evhttp_send_reply(request, HTTP_OK, "HTTP_OK", evb);
    evbuffer_free(evb);
};

void AuthorTweetView::GET(evhttp_request *request, void *ctx) {
    auto *context = (CbContext *) ctx;
    evbuffer *evb = evbuffer_new();
    int authorId = getAuthorIdFromUrl(request, "^\\/author\\/([0-9]+)\\/tweets\\/$");
    struct evkeyvalq uri_params;
    evhttp_parse_query(request->uri, &uri_params);
    int pageNum = 1;
    const char *rawPageNums = evhttp_find_header(&uri_params, "page");
    if (rawPageNums != nullptr) {
        //TODO atoi может упасть
        pageNum = atoi(rawPageNums);
    }
    int limit = pageNum * LIMIT_TWEETS_PER_PAGE;
    int offset = limit - LIMIT_TWEETS_PER_PAGE;
    std::unique_ptr<TweetRepository> tweetRepository(new TweetRepository(context->db));
    auto *listOfTweets = tweetRepository->getAllUserTweets(authorId, offset, limit);
    std::string resultListOfTweets = serializeTweetList(listOfTweets);
    evhttp_add_header(request->output_headers, "Content-Type", "application/json");

    evbuffer_add_printf(evb, "%s", resultListOfTweets.c_str());
    evhttp_send_reply(request, HTTP_OK, "HTTP_OK", evb);
    evbuffer_free(evb);
}