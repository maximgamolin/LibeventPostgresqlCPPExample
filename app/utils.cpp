//
// Created by maxim on 19.04.2020.
//

#include "utils.h"
#include "string"
#include "regex"
#include <evhttp.h>


int getAuthorIdFromUrl(struct evhttp_request *request, const char *regexp) {
    std::string path = evhttp_uri_get_path(request->uri_elems);
    int authorId = 0;

    try {
        std::regex re(regexp);
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