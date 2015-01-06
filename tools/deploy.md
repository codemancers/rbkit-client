## Making Linux builds

The key thing is, to run the deploy script which should copy
dependencies of application itself to lib folder, but we are still
left with the dependencies of `libqxcb.so` and those can be checked
via `ldd -r -d libqxcb.so`.

What I found was libQDBUS was missing from the list of libraries being
copied and hence that must be copied for making the deployment work.

More information can be found on, https://qt-project.org/forums/viewthread/38695
