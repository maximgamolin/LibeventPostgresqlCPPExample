//
// Created by maxim on 19.04.2020.
//
#include <evhttp.h>

#ifndef TIWTTER_VIEWS_H
#define TIWTTER_VIEWS_H

void notFound(evhttp_request *request, void *ctx);

class BaseView {
public:
    virtual void GET(evhttp_request *request, void *ctx) = 0;

    virtual void POST(evhttp_request *request, void *ctx) = 0;

    virtual void DELETE(evhttp_request *request, void *ctx) = 0;

    virtual void notAllowed(evhttp_request *request, void *ctx);

    virtual void asView(evhttp_request *request, void *ctx);
};

class AuthorDetailView : public BaseView {
private:
    int getAuthorIdFromURL(struct evhttp_request *request);

public:
    void GET(evhttp_request *request, void *ctx);

    void POST(evhttp_request *request, void *ctx);

    void DELETE(evhttp_request *request, void *ctx);
};

#endif //TIWTTER_VIEWS_H



