#ifndef CHOLL_IMGUI_WASM_WEBSOCKET_HPP
#define CHOLL_IMGUI_WASM_WEBSOCKET_HPP

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <utility>

#include <emscripten/websocket.h>

class websocket
{
public:
    websocket() = default;
    ~websocket();

    websocket(const websocket&) = delete;
    websocket& operator=(const websocket&) = delete;

    void connect(const char* url);

    void send(const void* buf, std::uint32_t nbytes)
    {
        emscripten_websocket_send_binary(ws_, const_cast<void*>(buf), nbytes);
    }

    void send(std::string_view str)
    {
        send(str.data(), str.size());
    }
/*
    EM_BOOL open()
    {
        status_ = "Connected";
        return EM_TRUE;
    }

    EM_BOOL error()
    {
        status_ = "Error";
        return EM_TRUE;
    }

    EM_BOOL close(bool clean, int code, const char* reason)
    {
        status_ = "Disconnected";
        return EM_TRUE;
    }

    EM_BOOL message(const std::uint8_t* data, std::size_t nbytes, bool text)
    {
        return EM_TRUE;
    }
*/
    const char* status()
    {
        return status_.c_str();
    }

    void reset()
    {
        if (auto ws = std::exchange(ws_, 0))
            emscripten_websocket_delete(ws);
    }

    std::function<EM_BOOL()> on_open;
    std::function<EM_BOOL()> on_error;
    std::function<EM_BOOL(bool clean, int code, const char* reason)> on_close;
    std::function<EM_BOOL(std::string_view buf, bool text)> on_message;

private:
    EMSCRIPTEN_WEBSOCKET_T ws_{};
    std::string status_;
};

#endif
