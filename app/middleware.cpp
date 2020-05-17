//
// Created by maxim on 13.05.2020.
//
#include <evhttp.h>
#include "dto.h"
#include "middleware.h"
#include "repository.h"
#include "memory"

void AuthMiddleware::execute(evhttp_request *request, void *ctx) {
    //add user by cookie into context
    auto *context = (CbContext *) ctx;
    std::unique_ptr<AuthorRepository> authorRepository(new AuthorRepository(context->db));
    std::string userCookie = this->_gerOrSetCookie(request);
    context->user = authorRepository->getRequestUserBySession(userCookie);
}

std::string AuthMiddleware::_gerOrSetCookie(evhttp_request *request) {
    std::string userCooke;
    std::string validCookieBody = AuthMiddleware::_buildCookie(request->remote_host);
    const char *rawUserCooke = evhttp_find_header(request->input_headers, "Cookie");
    if (rawUserCooke != nullptr) {
        userCooke = *rawUserCooke;
    } else {
        userCooke = "";
    }
    if (validCookieBody != userCooke) {
        evhttp_add_header(request->output_headers, "Set-Cookie", ("token=" + validCookieBody).c_str());
    }
    return validCookieBody;
};

std::string AuthMiddleware::_buildCookie(const char *remoteHost) {
    //maximum unsafe method, but i dont want install some crypto libs for sha, md5 or something like this
    std::string salt = "nUfgH9yJ";
    return salt + remoteHost + salt;
}