#!/bin/zsh
ImageVersion="0.1.2"

docker run --rm --cap-add=SYS_PTRACE --security-opt seccomp=unconfined -v $PWD:/riflerange -it compenv:$ImageVersion /bin/bash
