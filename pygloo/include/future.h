#include <chrono>
#include <memory>
#include <gloo/transport/unbound_buffer.h>

namespace pygloo {
namespace future {

using UnboundBuffer = gloo::transport::UnboundBuffer;

enum class Op : std::uint8_t {
  SEND = 0,
  RECV,
  UNUSED,
};

class Future {
public:
 Future(std::unique_ptr<UnboundBuffer> gloo_buffer, Op op);
 ~Future();

 // Not a threadsafe.
 bool Wait(std::chrono::milliseconds timeout);

private:
 /// Disable copy constructor because it needs to accept unique_ptr.
 Future(const Future& other) = delete;

 /// Private Attributes.
 std::unique_ptr<UnboundBuffer> gloo_buffer_;
 Op op_;
};

} // namespace future
} // namespace pygloo
