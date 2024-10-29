#!/bin/bash

# Assuming in docker "quay.io/pypa/manylinux2014_x86_64".

PYGLOO_VERSION="0.2.1"
# 定义 Python 版本数组
PYTHON_VERSIONS=("cp38-cp38" "cp39-cp39" "cp310-cp310" "cp311-cp311" "cp312-cp312")

# 遍历每个版本
for VERSION in "${PYTHON_VERSIONS[@]}"; do
    # 设置 Python 路径
    PYTHON_BIN_PATH="/opt/python/${VERSION}/bin/python"
    
    export PYTHON_BIN_PATH=$PYTHON_BIN_PATH 
    # 执行构建和修复
    $PYTHON_BIN_PATH setup.py bdist_wheel
    auditwheel repair --plat manylinux2014_x86_64 "dist/pygloo-${PYGLOO_VERSION}-${VERSION}-linux_x86_64.whl"
    
    # 清理构建环境
    bazel clean --expunge
    $PYTHON_BIN_PATH setup.py clean --all
done