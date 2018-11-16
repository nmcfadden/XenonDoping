### Build a Docker container with decay0

Use the `Dockerfile_decay0` to build up a Docker container with cernlib and decay0:
```shell
$ sudo docker build -f Dockerfile_decay0 . -t decay0
```
Then run it with
```shell
$ sudo docker run --rm decay0
```
N.B. write the output file in an external mounted volume! See Docker docs for details.

### Compile decay0 on your system

Use `Dockerfile_decay0` as a reference. First compile and install the patched cernlib packages then compile `decay0.for` with `gfortran` and link with the cernlib libraries. You can use `Makedecay0` as a skeleton script.
