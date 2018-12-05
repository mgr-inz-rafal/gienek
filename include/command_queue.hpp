#pragma once

#include <mutex>
#include <optional>
#include <queue>

namespace gienek {

template <typename CmdType>
class command_queue {
    std::mutex _access_mutex;
    std::queue<CmdType> q;

  public:
    void enq(const CmdType& cmd) {
        std::lock_guard lock(_access_mutex);
        q.push(cmd);
    }
    std::optional<CmdType> deq() {
        std::lock_guard lock(_access_mutex);
        if (!q.empty()) {
            auto c = q.front();
            q.pop();
            return c;
        }
        return std::nullopt;
    }
};

} // namespace gienek
