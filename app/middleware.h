//
// Created by maxim on 13.05.2020.
//

#ifndef TIWTTER_MIDDLEWARE_H
#define TIWTTER_MIDDLEWARE_H

class BaseMiddleware {
public:
    virtual void execute(evhttp_request *request, void *ctx) = 0;
};

class AuthMiddleware : public BaseMiddleware {

public:
    void execute(evhttp_request *request, void *ctx) override;

    static std::string _buildCookie(const char *remoteHost);

private:
    std::string _gerOrSetCookie(evhttp_request *request);
};

#endif //TIWTTER_MIDDLEWARE_H
