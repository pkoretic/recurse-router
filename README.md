[![License](http://img.shields.io/:license-mit-blue.svg)](https://github.com/xwalk/recurse-router/blob/master/LICENSE)

#Router

# Example


```
#include "router/router.hpp"

int main(int argc, char *argv[])
{
    Recurse app(argc, argv);

    Module::Router router;

    router.GET("/hello/:user", [](auto &ctx, auto /* next */)
    {
        ctx.response.send("hello world " + ctx.params["user"]);
    });


	app.listen();
}
```
