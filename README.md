rbkit-client
============


# Development

## Requirements

You need Qt on your system in order to build the project. It's recommended to
install Qt Creator which is an IDE for Qt projects.

## Qt build issue on OSX Mavericks

Qt binary distribution compiles with `-stdlib=libstdc++` for std lib,
where as Xcode 5 on Mavericks compiles with `-stdlib=libc++`. So when you
are using libraries compiled outside of Qt in the app, the linker will not
be able to resolve the symbols in the stdlib. Fix this by doing to following.
Go to `Projects` tab in Qt and under `Build Steps`, expand the qmake step and
look for `-spec` argument. You need to add a flag `QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9`
in the `qmake.conf` file of that spec.

For example:

My qmake command is

```bash
qmake /Users/emil/OpenSource/rbkit-client/RbkitClient.pro -r -spec macx-g++
CONFIG+=debug CONFIG+=x86_64 CONFIG+=declarative_debug CONFIG+=qml_debug
```

Since value of `-spec` is macx-g++, I'll need to add a line with
`QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9` in `~/Qt5.2.1/5.2.1/clang_64/mkspecs/macx-g++/qmake.conf`

# Releasing a new version

### Create a dmg for OSX platform
Update the VERSION value in RbkitClient.pro file to reflect the new version.
Run `macdeployqt RbkitClient.app -dmg` where `RbKitClient.app` is the OSX deployment
created by Qt Creator when you built the project locally. Doing this gives you
a `dmg` file.

### Tag and draft a release
Github allows you to draft releases and attach files along with the releases.
[Click here](https://github.com/code-mancers/rbkit-client/releases/new) to draft
a release. Releases always happen from the master branch. Tag the commit with
the version to be released. We follow [semantic versioning](http://semver.org/).
Upload the dmg file on the release draft and submit.
