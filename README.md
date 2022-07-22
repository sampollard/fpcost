# FPCost: Scalable Error Analysis for Floating-Point Program Optimization

This is a repository of various tools with the goal of improving
floating-point error analysis, but also cost analysis. Again, it
probably should have been named something other than FPCost, but
it's locked in a PDF now (see the Cross-Referencing section). So, you
can think of it in two parts: error analysis, and cost analysis

## Error Analysis

The main contribution here is a method to generate FPTaylor. The main
directories here are `cclose`, `ccorrect`, and `search`.

## Cost Analysis
This is the code contained in the `src/cost` directory. This code runs
performance _and_ error results for the particularly nasty sections of
floating-point such as subnormals.

### Dependencies
- C++ compiler supporting `-std=c++-20` (Will work on c++-14 for now,
  but won't at some point)
- MPFR

You can install these on Ubuntu with `apt install g++-10 libmpfr-dev`

### Building
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

## Other Works-in-Progress
The directories `haskell` and `coq` were works in progress which didn't
go as far as I wanted. Basically, the `haskell` one was supposed to be a
domain-specific language which was written in a Matlab-like language,
which could generate floating-point code in C, with annotations in a
language like ACSL. As of now, this essentially does nothing, though
perhaps was an inspiration for my future work doing this in Frama-C -
stay tuned.

The `coq` directory was supposed to be a domain-specific language to
represent floating-point programs which one could write small programs,
and then prove their correctness inside Coq. This did not get very far
either. For future references, I recommend the fantastic work by Appel,
Bindel, Kellison, Cohen, and Carstens, [Verinum](https://verinum.org/).

## Cross-Referencing, History, and (Open) Access

This repository contains source code used to generate much of the data
for Chapter V of Samuel D. Pollard's PhD Dissertation,
[When Does a Bit Matter? Techniques for Verifying the Correctness of
Assembly Languages and Floating-Point
Programs](sampollard.github.io/research/artifacts/pollard_dissertation.pdf).
If that link is broken, the [University of Oregon Department
Database](https://www.cs.uoregon.edu/Reports/PHD-202106-Pollard.pdf) may
be longer-lived. If _that_ link is broken, then the [University of
Oregon](https://scholarsbank.uoregon.edu/xmlui/handle/1794/26652)
website may work. Keep in mind that the UO link has a small error with a
missing figure. So if possible, prefer the first, then second links.

The actual code and data generated and analyzed for the dissertation can
be found on [Zenodo](https://doi.org/10.5281/zenodo.4745336) for the
foreseeable future.

The dissertation code will henceforth be kept in the `dissertation`
branch and will not change. In retrospect, it would have been better to
make it a tag in Git, but the text of the dissertation refers to the
dissertation branch.
