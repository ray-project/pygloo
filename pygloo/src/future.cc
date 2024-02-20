#include <cassert>
#include <future.h>
#include "gloo/transport/unbound_buffer.h"

namespace pygloo {
namespace future {

Future::Future(std::unique_ptr<UnboundBuffer> gloo_buffer, Op op) : gloo_buffer_(std::move(gloo_buffer)), op_(op) {}

Future::~Future() {}

bool Future::Wait(std::chrono::milliseconds timeout) {
  if (op_ == Op::SEND) {
    return gloo_buffer_->waitSend(timeout);
  } else if (op_ == Op::RECV) {
    return gloo_buffer_->waitRecv(timeout);
  } else {
    // this should never happen.
    assert(false);
  }
}

} // namespace future
} // namespace pygloo
