# ChicOS(S)

This is a college Operating Systems class project, meanwhile a playground for us to taste the feeling of being a kernel programmer (roughly). For now, we do not have the instructions for what to follow, so, until then, this project will only handle some utilities and default operations we think a OS must implement. Later on, this will become (as we believe) a (not) fully functional OS simulator.

To test the project for yourself, simply run:

```sh

# for debug mode
cmake -DCMAKE_BUILD_TYPE=DEBUG -S . -B target/ 

# for release mode
cmake -DCMAKE_BUILD_TYPE=RELEASE  -S . -B target/ 

cmake --build target/

./target/ChicOS

```
