# pygloo

Pygloo provides Python bindings for [gloo](https://github.com/facebookincubator/gloo). 
It is implemented using [pybind11](https://github.com/pybind/pybind11).

It is currenlty used in [Ray for collective communication](https://github.com/ray-project/ray/tree/master/python/ray/util/collective) between CPUs. 


## Requirements
```python
Python >= 3.6
```

## Installation
### Install From Wheels
We provide prepackaged Python wheels (`manylinux_2_24_x86_64`). To install from wheels:
```python
pip install pygloo
```

### Building from source
One can build pygloo from source if none of released wheels fit with the development environment.

Pygloo uses [Bazel](https://github.com/bazelbuild/bazel) to automatically manange dependencies and compilation.
To compile from source, install Bazel>=2.0.0 following the [Bazel installation guide](https://docs.bazel.build/versions/master/install.html).
After installing Bazel, build and install pygloo following this command:
```python
python setup.py install
```

## Testing
Pygloo uses [Ray](https://github.com/ray-project/ray) to create multiple, distributed processes for collective communication tests. See `tests` directory.

## Example
An example for allreduce.
```python
import os
import ray
import pygloo
import numpy as np

@ray.remote(num_cpus=1)
def test_allreduce(rank, world_size, fileStore_path):
    '''
    rank  # Rank of this process within list of participating processes
    world_size  # Number of participating processes
    fileStore_path # The path to create filestore
    '''
    context = pygloo.rendezvous.Context(rank, world_size)
    # Prepare device and store for rendezvous
    attr = pygloo.transport.tcp.attr("localhost")
    dev = pygloo.transport.tcp.CreateDevice(attr)
    fileStore = pygloo.rendezvous.FileStore(fileStore_path)
    store = pygloo.rendezvous.PrefixStore(str(world_size), fileStore)

    context.connectFullMesh(store, dev)

    sendbuf = np.array([[1,2,3],[1,2,3]], dtype=np.float32)
    recvbuf = np.zeros_like(sendbuf, dtype=np.float32)
    sendptr = sendbuf.ctypes.data
    recvptr = recvbuf.ctypes.data

    pygloo.allreduce(context, sendptr, recvptr,
                    sendbuf.size, pygloo.glooDataType_t.glooFloat32,
                    pygloo.ReduceOp.SUM, pygloo.allreduceAlgorithm.RING)

if __name__ == "__main__":
    ray.init(num_cpus=6)
    world_size = 2
    fileStore_path = f"{ray.worker._global_node.get_session_dir_path()}" + "/collective/gloo/rendezvous"
    os.makedirs(fileStore_path)
    ray.get([test_allreduce.remote(rank, world_size, fileStore_path) for rank in range(world_size)])
```


## License
Gloo is licensed under the Apache License, Version 2.0.