#include <gloo/config.h>
#include <rendezvous.h>

#include <gloo/rendezvous/context.h>
#include <gloo/rendezvous/file_store.h>
#include <gloo/rendezvous/hash_store.h>
#include <gloo/rendezvous/prefix_store.h>

#if GLOO_USE_REDIS
#include <gloo/rendezvous/redis_store.h>
#endif

namespace pygloo {
namespace rendezvous {

void def_rendezvous_module(pybind11::module &m) {
  pybind11::module rendezvous =
      m.def_submodule("rendezvous", "This is a rendezvous module");

  pybind11::class_<gloo::rendezvous::Context, gloo::Context,
                   std::shared_ptr<gloo::rendezvous::Context>>(rendezvous,
                                                               "Context")
      .def(pybind11::init<int, int, int>(), pybind11::arg("rank") = nullptr,
           pybind11::arg("size") = nullptr, pybind11::arg("base") = 2)
      .def("connectFullMesh", &gloo::rendezvous::Context::connectFullMesh);

  pybind11::class_<gloo::rendezvous::Store,
                   std::shared_ptr<gloo::rendezvous::Store>>(rendezvous,
                                                             "Store")
      .def("set", &gloo::rendezvous::Store::set)
      .def("get", &gloo::rendezvous::Store::get);

  pybind11::class_<gloo::rendezvous::FileStore, gloo::rendezvous::Store,
                   std::shared_ptr<gloo::rendezvous::FileStore>>(rendezvous,
                                                                 "FileStore")
      .def(pybind11::init<const std::string &>())
      .def("set", &gloo::rendezvous::FileStore::set)
      .def("get", &gloo::rendezvous::FileStore::get);

  pybind11::class_<gloo::rendezvous::HashStore, gloo::rendezvous::Store,
                   std::shared_ptr<gloo::rendezvous::HashStore>>(rendezvous,
                                                                 "HashStore")
      .def(pybind11::init([]() { return new gloo::rendezvous::HashStore(); }))
      .def("set", &gloo::rendezvous::HashStore::set)
      .def("get", &gloo::rendezvous::HashStore::get);

  pybind11::class_<gloo::rendezvous::PrefixStore, gloo::rendezvous::Store,
                   std::shared_ptr<gloo::rendezvous::PrefixStore>>(
      rendezvous, "PrefixStore")
      .def(pybind11::init<const std::string &, gloo::rendezvous::Store &>())
      .def("set", &gloo::rendezvous::PrefixStore::set)
      .def("get", &gloo::rendezvous::PrefixStore::get);

#if GLOO_USE_REDIS
  class RedisStoreWithAuth : public gloo::rendezvous::RedisStore {
  public:
    RedisStoreWithAuth(const std::string &host, int port)
        : gloo::rendezvous::RedisStore(host, port){};
    using gloo::rendezvous::RedisStore::check;
    using gloo::rendezvous::RedisStore::get;
    using gloo::rendezvous::RedisStore::redis_;
    using gloo::rendezvous::RedisStore::set;
    using gloo::rendezvous::RedisStore::wait;

    void authorize(std::string redis_password) {
      void *ptr =
          (redisReply *)redisCommand(redis_, "auth %b", redis_password.c_str(),
                                     (size_t)redis_password.size());

      if (ptr == nullptr) {
        GLOO_THROW_IO_EXCEPTION(redis_->errstr);
      }
      redisReply *reply = static_cast<redisReply *>(ptr);
      if (reply->type == REDIS_REPLY_ERROR) {
        GLOO_THROW_IO_EXCEPTION("Error: ", reply->str);
      }
      freeReplyObject(reply);
    }
    void execCommand(std::string command) {
      void *ptr = (redisReply *)redisCommand(redis_, "%b", command.c_str(),
                                             (size_t)command.size());

      if (ptr == nullptr) {
        GLOO_THROW_IO_EXCEPTION(redis_->errstr);
      }
      redisReply *reply = static_cast<redisReply *>(ptr);
      if (reply->type == REDIS_REPLY_ERROR) {
        GLOO_THROW_IO_EXCEPTION("Error: ", reply->str);
      }
      freeReplyObject(reply);
    }
  };

  pybind11::class_<gloo::rendezvous::RedisStore, gloo::rendezvous::Store,
                   std::shared_ptr<gloo::rendezvous::RedisStore>>(rendezvous,
                                                                  "_RedisStore")
      .def(pybind11::init<const std::string &, int>())
      .def("set", &gloo::rendezvous::RedisStore::set)
      .def("get", &gloo::rendezvous::RedisStore::get);

  pybind11::class_<RedisStoreWithAuth, gloo::rendezvous::RedisStore,
                   gloo::rendezvous::Store,
                   std::shared_ptr<RedisStoreWithAuth>>(rendezvous,
                                                        "RedisStore")
      .def(pybind11::init<const std::string &, int>())
      .def("set", &RedisStoreWithAuth::set)
      .def("get", &RedisStoreWithAuth::get)
      .def("authorize", &RedisStoreWithAuth::authorize)
      .def("execCommand", &RedisStoreWithAuth::execCommand);

#endif
}
} // namespace rendezvous
} // namespace pygloo