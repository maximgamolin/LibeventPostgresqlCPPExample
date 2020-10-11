//
// Created by maxim on 19.04.2020.
//
#include <evhttp.h>

#ifndef TIWTTER_VIEWS_H
#define TIWTTER_VIEWS_H

void notFound(evhttp_request *request, void *ctx);

class BaseView {
public:
    virtual void GET(evhttp_request *request, void *ctx);

    virtual void POST(evhttp_request *request, void *ctx);

    virtual void DELETE(evhttp_request *request, void *ctx);

    static void notAllowed(evhttp_request *request, void *ctx);

    void asView(evhttp_request *request, void *ctx);
};

class AuthorDetailView : public BaseView {
public:
    void GET(evhttp_request *request, void *ctx) override;

    void DELETE(evhttp_request *request, void *ctx) override;
};

class AuthroListView : public BaseView {
    void GET(evhttp_request *request, void *ctx) override;

    void POST(evhttp_request *request, void *ctx) override;
};


class LoginView : public BaseView {
private:
    void invalidLoginOrPassword(evhttp_request *request, void *ctx);

public:
    void POST(evhttp_request *request, void *ctx) override;
};


class LogoutView : public BaseView {
public:
    void GET(evhttp_request *request, void *ctx) override;
};


class AuthorTweetView : public BaseView {
public:
    void GET(evhttp_request *request, void *ctx) override;
};

class TweetView : public BaseView {
public:
    void GET(evhttp_request *request, void *ctx) override;

    void POST(evhttp_request *request, void *ctx) override;
};

class TweetDetailView : public BaseView {
public:
    void DELETE(evhttp_request *request, void *ctx) override;
};

#endif //TIWTTER_VIEWS_H



