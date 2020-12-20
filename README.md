# Tea
Tea is an interpreted programming language that is based on execution speed and simplicity. Yes, an interpreted \
language that is built for speed. Crazy right? Maybe.



## Logo
![Tea Logo](https://pixy.org/src/478/4785394.png)
Logo author: [Marquitta Spagnolo](https://pixy.org/author/Marquitta_Spagnolo/)



## Syntax
```py
# Make a string of text
string helloWorld "Hello, world!"

# Make a system call
emplace system "echo {helloWorld}"

# Exit with code 0 (arg count - arg count)
emplace exit {argc} - {argc}
```
Tea has very strict syntax as of now. So strict to the point that even some forms of whitespace can cause your \
program to run into an error. I do plan on changing this in the future though, so that Tea can be even more user \
friendly. :)



## Libraries used
* [fmt 7.1.2](https://github.com/fmtlib/fmt/)
* [spdlog 1.8.0](https://github.com/gabime/spdlog/)
