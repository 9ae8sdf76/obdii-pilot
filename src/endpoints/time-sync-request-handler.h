#pragma once

#include <WebServer.h>
#include <ConfigManager.h>
#include "abstract-request-handler.h"

namespace dbuddy {
    class TimeSyncRequestHandler : public AbstractRequestHandler {
    public:
        explicit TimeSyncRequestHandler(ConfigManager * manager) : AbstractRequestHandler(manager) {};

        bool canHandle(HTTPMethod method, String uri) final;
        bool setup(WebServer &server, HTTPMethod requestMethod, String requestUri) final;
    };
}
