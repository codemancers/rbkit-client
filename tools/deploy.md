## Making Linux builds

### Background Information

The key thing is, to run the deploy script which should copy
dependencies of application itself to lib folder, but we are still
left with the dependencies of `libqxcb.so` and those can be checked
via `ldd -r -d libqxcb.so`.

What I found was libQDBUS was missing from the list of libraries being
copied and hence that must be copied for making the deployment work.

More information can be found on, https://qt-project.org/forums/viewthread/38695

### Using Ruby script here to make a linux build

First navigate to the directory where local binary has been built. In many cases it
would be somewhere - `$HOME/build-RbkitClient-Desktop-xxx` or whatever is your location
of shadow build.

1. Go to `rbit-app` folder of shadow build directory and check if you have `RbkitClient` executable present.
2. Once there, specify using script here to make a build:

```
~/nomads/rbkit-client/tools/deployqt.rb RbkitClient
```

This should give us a `rbkit.tar.gz` which can be now deployed on any Linux.
