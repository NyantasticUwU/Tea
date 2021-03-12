# Tea
Tea is an interpreted programming language that is based on execution speed and simplicity. Yes, an interpreted \
language that is built for speed. Crazy right? Maybe.



## Logo
![Tea Logo](https://pixy.org/src/478/4785394.png)
Logo author: [Marquitta Spagnolo](https://pixy.org/author/Marquitta_Spagnolo/)



## Syntax
```py
# Make a string of text
# The standard library Tea::IO::Print snippet requires a string with the name tsPrint
# The ts prefix stands for "to snippet"
string tsPrint "Hello, world!\n"

# Print to stdout
# As of now tea's standard library only consists of snippets
# All standard library snippets reside in the Tea namespace
# Some snippets will return with new variables, prefixed with fs (from snippet)
call snippet Tea::IO::Print

# Remove a variable from memory
delete string tsPrint

# Exit with code 0 (arg count - arg count)
# The emplace keyword is used to emplace variables
# argc is a predefined variable that stands for arg count
emplace exit {argc} - {argc}
```
Tea has very strict syntax as of now. So strict to the point that even some forms of whitespace can cause your \
program to run into an error. I do plan on changing this in the future though, so that Tea can be even more user \
friendly. :)



## Libraries used
* [CLI11 1.9.1](https://github.com/CLIUtils/CLI11/)
* [fmt 7.1.3](https://github.com/fmtlib/fmt/)
* [spdlog 1.8.0](https://github.com/gabime/spdlog/)
