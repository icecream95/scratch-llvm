# scratch-llvm
[![Travis CI build status](https://api.travis-ci.org/icecream95/scratch-llvm.svg?branch=master)](https://travis-ci.org/icecream95/scratch-llvm) [![license](https://img.shields.io/github/license/icecream95/scratch-llvm.svg)](https://github.com/icecream95/scratch-llvm/blob/master/LICENSE)


(WIP) Scratch LLVM front and backend

~~NOTE: I started this before looking at the size of the llvm documentation, and I'm busy working on other stuff, so it might be a while until anything appears here~~

I'm back to working on this! More stuff will appear soon...

~~Currently, there seems to be a problem with clang (or with GCC) where clang doesn't carry through members of std::function to my derived class, so it might be a while fixing this.
I could switch to GCC, but this is an llvm backend, so anything from the "other camp" is going to be a last resort.~~

I've fixed the problem with Clang - it looks like I accidentally used a
c++17 feature without realising it. I just added -std=c++1z to the CMake line.

I'm currently trying to find out what the earliest version of Clang that supports the feature I used is...