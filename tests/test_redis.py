import numpy as np
import ray
import torch
import pygloo

@ray.remote(num_cpus=1)
def test_redis(rank, world_size, redis_ip_address, redis_port, redis_password):
    '''
    rank  # Rank of this process within list of participating processes
    world_size  # Number of participating processes
    '''
    context = pygloo.rendezvous.Context(rank, world_size);

    attr = pygloo.transport.tcp.attr("localhost")
    # Perform rendezvous for TCP pairs
    dev = pygloo.transport.tcp.CreateDevice(attr)

    redisStore = pygloo.rendezvous.RedisStore(redis_ip_address, redis_port)

    redisStore.authorize(redis_password)
    store = pygloo.rendezvous.PrefixStore("default", redisStore)

    context.connectFullMesh(store, dev)

    print("Using RedisStore rendezvous, connect successful!!")

    sendbuf = np.array([[1,2,3],[1,2,3]], dtype=np.float32)
    recvbuf = np.zeros_like(sendbuf, dtype=np.float32)
    sendptr = sendbuf.ctypes.data
    recvptr = recvbuf.ctypes.data

    data_size = sendbuf.size if isinstance(sendbuf, np.ndarray) else sendbuf.numpy().size
    datatype = pygloo.glooDataType_t.glooFloat32
    op = pygloo.ReduceOp.SUM
    algorithm = pygloo.allreduceAlgorithm.RING

    pygloo.allreduce(context, sendptr, recvptr, data_size, datatype, op, algorithm)

    print(f"rank {rank} sends {sendbuf}, receives {recvbuf}")


@ray.remote(num_cpus=1)
def test_multiGroup(rank, world_size, redis_ip_address, redis_port, redis_password):
    '''
    test the multiGroup without prefixStore
    rank  # Rank of this process within list of participating processes
    world_size  # Number of participating processes
    '''
    groups = [f"multiGroup{i}" for i in range(3)]
    contexts = {}
    for group_name in groups:
        context = pygloo.rendezvous.Context(rank, world_size);

        attr = pygloo.transport.tcp.attr("localhost")
        # Perform rendezvous for TCP pairs
        dev = pygloo.transport.tcp.CreateDevice(attr)

        redisStore = pygloo.rendezvous.RedisStore(redis_ip_address, redis_port)

        redisStore.authorize(redis_password)

        context.connectFullMesh(redisStore, dev)
        if rank == 0:
            keys = []
            keys += [f"rank_{i}" for i in range(world_size)]
            keys += [f"{i}" for i in range(world_size)]
            redisStore.delKeys(keys)
        contexts[group_name] = context
    print("Using RedisStore rendezvous, connect successful!!")

    for group_name in groups:
        context = contexts[group_name]
        sendbuf = np.array([[1,2,3],[1,2,3]], dtype=np.float32)
        recvbuf = np.zeros_like(sendbuf, dtype=np.float32)
        sendptr = sendbuf.ctypes.data
        recvptr = recvbuf.ctypes.data

        data_size = sendbuf.size if isinstance(sendbuf, np.ndarray) else sendbuf.numpy().size
        datatype = pygloo.glooDataType_t.glooFloat32
        op = pygloo.ReduceOp.SUM
        algorithm = pygloo.allreduceAlgorithm.RING

        pygloo.allreduce(context, sendptr, recvptr, data_size, datatype, op, algorithm)

        print(f"rank {rank} sends {sendbuf}, receives {recvbuf}")

if __name__ == "__main__":
    ray.init(num_cpus=6)
    world_size = 2
    fileStore_path = f"{ray.worker._global_node.get_session_dir_path()}" + "/collective/gloo/rendezvous"
    redis_address = ray.worker._global_node.redis_address
    redis_ip, redis_port = redis_address.split(":")
    redis_password = ray.worker._global_node.redis_password
    print(f"redis_address is {redis_ip}, the port is {redis_port}, redis_password is {redis_password}")
    ray.get([test_redis.remote(i, world_size, redis_ip, int(redis_port), redis_password) for i in range(world_size)])

    ray.get([test_multiGroup.remote(i, world_size, redis_ip, int(redis_port), redis_password) for i in range(world_size)])
