#pragma once

class BaseProxy
{
    protected:
        bool isProxyStarted = false;
    public:
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual void handle(uint8_t *data, size_t size) = 0;
};
