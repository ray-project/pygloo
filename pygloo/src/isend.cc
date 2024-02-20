#include <collective.h>
#include <gloo/types.h>
#include <iostream>
namespace pygloo {

template <typename T>
std::shared_ptr<future::Future> isend(const std::shared_ptr<gloo::Context> &context, intptr_t sendbuf,
          size_t size, int peer, uint32_t tag) {
  if (context->rank == peer)
    throw std::runtime_error(
        "peer equals to current rank. Please specify other peer values.");

  auto inputBuffer = context->createUnboundBuffer(
      reinterpret_cast<T *>(sendbuf), size * sizeof(T));

  constexpr uint8_t kSendRecvSlotPrefix = 0x09;
  gloo::Slot slot = gloo::Slot::build(kSendRecvSlotPrefix, tag);

  inputBuffer->send(peer, slot);
  return std::make_shared<future::Future>(std::move(inputBuffer), future::Op::SEND);
}

std::shared_ptr<future::Future> isend_wrapper(const std::shared_ptr<gloo::Context> &context,
                  intptr_t sendbuf, size_t size, glooDataType_t datatype,
                  int peer, uint32_t tag) {
  switch (datatype) {
  case glooDataType_t::glooInt8:
    return isend<int8_t>(context, sendbuf, size, peer, tag);
    break;
  case glooDataType_t::glooUint8:
    return isend<uint8_t>(context, sendbuf, size, peer, tag);
    break;
  case glooDataType_t::glooInt32:
    return isend<int32_t>(context, sendbuf, size, peer, tag);
    break;
  case glooDataType_t::glooUint32:
    return isend<uint32_t>(context, sendbuf, size, peer, tag);
    break;
  case glooDataType_t::glooInt64:
    return isend<int64_t>(context, sendbuf, size, peer, tag);
    break;
  case glooDataType_t::glooUint64:
    return isend<uint64_t>(context, sendbuf, size, peer, tag);
    break;
  case glooDataType_t::glooFloat16:
    return isend<gloo::float16>(context, sendbuf, size, peer, tag);
    break;
  case glooDataType_t::glooFloat32:
    return isend<float_t>(context, sendbuf, size, peer, tag);
    break;
  case glooDataType_t::glooFloat64:
    return isend<double_t>(context, sendbuf, size, peer, tag);
    break;
  default:
    throw std::runtime_error("Unhandled dataType");
  }
}
} // namespace pygloo
