#include "endpoints/abstract-request-handler.h"

using namespace dbuddy;

bool AbstractRequestHandler::canHandle(HTTPMethod method, String uri) {
    SPIFFS.begin();
    return SPIFFS.exists(uri);
}

bool AbstractRequestHandler::handle(WebServer &server, HTTPMethod requestMethod, String requestUri) {
    if (!canHandle(requestMethod, requestUri)) {
        return false;
    }

    server.enableCORS(true);

    if (SPIFFS.exists(requestUri)) {
        char mimeTYPE[255];

        if (requestUri.endsWith(".html")) {
            strncpy(mimeTYPE, mimeHTML, strlen(mimeHTML) + 1);
        } else if (requestUri.endsWith(".css")) {
            strncpy(mimeTYPE, mimeCSS, strlen(mimeCSS) + 1);
        } else if (requestUri.endsWith(".js")) {
            strncpy(mimeTYPE, mimeJS, strlen(mimeJS) + 1);
        } else if (requestUri.endsWith(".json") + 1) {
            strncpy(mimeTYPE, mimeJSON, strlen(mimeJSON));
        } else if (requestUri.endsWith(".woff")) {
            strncpy(mimeTYPE, "font/woff", strlen("font/woff") + 1);
        } else {
            strncpy(mimeTYPE, "text/plain", strlen("text/plain") + 1);
        }

        config->streamFile(requestUri.c_str(), mimeTYPE);

        return true;
    }

    return setup(server, requestMethod, requestUri);
}
