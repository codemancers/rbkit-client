# http://danny-pope.com/?p=86

COFFEESCRIPT_FILES += $$PWD/src/*.coffee

# Define the compiler. The actual compilation is done in the 'commands' section.
CoffeeMaker.input = COFFEESCRIPT_FILES
CoffeeMaker.targetdir = $$PWD/src
CoffeeMaker.output = $$CoffeeScriptCompiler.targetdir\${QMAKE_FILE_IN_BASE}${QMAKE_FILE_EXT}
CoffeeMaker.commands = coffee --bare --compile --output $$PWD/src $$PWD/src
CoffeeMaker.CONFIG += no_link_no_clean
CoffeeMaker.variable_out = PRE_TARGETDEPS

# Add the compiler to the list of 'extra compilers'.
QMAKE_EXTRA_COMPILERS += CoffeeMaker

RESOURCES += $$PWD/web.qrc
