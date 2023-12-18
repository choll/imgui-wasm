#include "websocket.hpp"

#include <cassert>

void websocket::connect(const char* url)
{
    assert(ws_ == 0);

    EmscriptenWebSocketCreateAttributes attrs{url, nullptr, EM_TRUE};
    ws_ = emscripten_websocket_new(&attrs);

    emscripten_websocket_set_onopen_callback(
        ws_,
        this,
        [](int, const auto*, void* user)
        {
            // Open events do not carry any data other than the websocket
            // descriptor.
            return static_cast<websocket*>(user)->on_open();
        });

    emscripten_websocket_set_onerror_callback(
        ws_,
        this,
        [](int, const auto*, void* user)
        {
            // Error events do not carry any data other than the websocket
            // descriptor.
            return static_cast<websocket*>(user)->on_error();
        });

    emscripten_websocket_set_onclose_callback(
        ws_,
        this,
        [](int, const auto* ev, void* user)
        {
            return static_cast<websocket*>(user)->on_close(
                ev->wasClean, ev->code, ev->reason);
        });

    emscripten_websocket_set_onmessage_callback(
        ws_,
        this,
        [](int, const auto* ev, void* user)
        {
            return static_cast<websocket*>(user)->on_message(
                std::string_view{
                    reinterpret_cast<const char*>(ev->data),
                    ev->numBytes},
                ev->isText);
        });
}

websocket::~websocket()
{
    reset();
}
