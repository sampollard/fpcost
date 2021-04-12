# fpcost

## Dependencies
- C++ compiler supporting `-std=c++-20` (Will work on c++-14 for now,
  but won't soon)
- MPFR

You can install these on Ubuntu with `apt install g++-10 libmpfr-dev`

## Building
There are two different ways to build this: a fast way and a slow way.

The "fast" way does not do any error checking and is purely for timing.
The slow way checks against an MPFR float and prints the errors.
Under the hood, this is using the macro `PRINT_ERROR`. That is,
whenever you're doing some printing or high-precision version, make
sure it is in a `PRINT_ERROR` `#ifdef` block.


You build the fast way with
```
make time
make run_time
```
The slow way with
```
make error
make run_error
```
