import pygloo
import numpy as np
import os
import ray
import time
import shutil
import torch
import pytest

import ray.experimental.internal_kv as internal_kv
from ray._private.gcs_utils import GcsClient


class MyMockCustomStore:
    def __init__(self):
        gcs_address = ray.worker._global_node.gcs_address
        self._gcs_client = GcsClient(address=gcs_address, nums_reconnect_retry=0)
        internal_kv._initialize_internal_kv(self._gcs_client)

    def set(self, key: str, data: bytes) -> bool:
        ret = internal_kv._internal_kv_put(key, data)
        return ret

    def get(self, key: str) -> bytes:
        ret = internal_kv._internal_kv_get(key)
        return ret

    def wait(self, keys: list):
        while(True):
            all_exist = True
            for key in keys:
                result = internal_kv._internal_kv_exists(key)
                if not result:
                    all_exist = False
                    break
            if all_exist:
                return True
            time.sleep(1)

    def del_keys(self, keys: list):
        for key in keys:
            ok = internal_kv._internal_kv_del(key)
            if not ok:
                return False
        return True

@ray.remote(num_cpus=1)
class Sender:
    def __init__(self):
        rank = 0
        world_size = 2
        self._context = pygloo.rendezvous.Context(rank, world_size)
        attr = pygloo.transport.tcp.attr("localhost")
        dev = pygloo.transport.tcp.CreateDevice(attr)
        real_store = MyMockCustomStore()
        custom_store = pygloo.rendezvous.CustomStore(real_store)
        self._context.connectFullMesh(custom_store, dev)

    def do_send(self):
        sendbuf = np.array([[1,2,3],[1,2,3]], dtype=np.float32)
        sendptr = sendbuf.ctypes.data
        pygloo.send(self._context, sendptr, sendbuf.size, pygloo.glooDataType_t.glooFloat32, 1)
        return True


@ray.remote(num_cpus=1)
class Recver:
    def __init__(self):
        rank = 1
        world_size = 2
        self._context = pygloo.rendezvous.Context(rank, world_size)
        attr = pygloo.transport.tcp.attr("localhost")
        dev = pygloo.transport.tcp.CreateDevice(attr)
        real_store = MyMockCustomStore() 
        custom_store = pygloo.rendezvous.CustomStore(real_store)
        self._context.connectFullMesh(custom_store, dev)

    def do_recv(self):
        recvbuf = np.zeros((2, 3), dtype=np.float32)
        recvptr = recvbuf.ctypes.data

        data_size = recvbuf.size if isinstance(recvbuf, np.ndarray) else recvbuf.numpy().size
        datatype = pygloo.glooDataType_t.glooFloat32
        peer = 0

        pygloo.recv(self._context, recvptr, data_size, datatype, peer)
        return recvbuf


def test_basic():
    ray.init(num_cpus=6)

    sender = Sender.remote()
    recver = Recver.remote()
    fn1 = sender.do_send.remote()
    fn2 = recver.do_recv.remote()

    a, b = ray.get([fn1, fn2])
    assert a
    expected = [[1, 2, 3], [1, 2, 3]]
    assert len(b) == 2
    assert len(b[0]) == 3
    assert len(b[1]) == 3


if __name__ == "__main__":
    sys.exit(pytest.main(["-v", __file__]))
