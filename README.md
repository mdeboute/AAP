# Optimal Firefighters Allocation

## [French report](./report.pdf)

## How to compile executable files?

```shell
mkdir -p build
cd build
cmake ..
make <target>
```

`<target>` can be one of the following:

- `test.out`: the test suite
- `mip.out`: the MIP solver
- `unit_test.out`: the unit test suite
- `all`: all of the above

## How to run executable files?

In the build directory:

```shell
./<exec>.out
```

You can add the flag `-h` or `--help` to get help on the command line arguments.

## How to run the benchmark script?

In the root directory:

```shell
./benchmark.sh
```

You can add the flag `-h` or `--help` to get help on the command line arguments.
