#Router

[![License](http://img.shields.io/:license-mit-blue.svg)](https://github.com/xwalk/recurse-router/blob/master/LICENSE)

Recurse router middleware

Supports:

 * GET
 * PUT
 * POST
 * PATCH
 * DELETE
 * HEAD
 * OPTIONS
 * ALL - matches all routes


### Example

```
#include "router.hpp"

int main(int argc, char *argv[])
{
    Recurse app(argc, argv);

    Module::Router router;

    router.GET("/hello/:user", [](auto &ctx, auto /* next */)
    {
        ctx.response.send("Hello World " + ctx.request.params["user"]);
    });

	app.listen();
}
```

### Parameters

```
#include "router.hpp"

int main(int argc, char *argv[])
{
    Recurse app(argc, argv);

    Module::Router router;

    // matches /hello/john
    router.GET("/hello/:name", [](auto &ctx, auto /* next */)
    {
        ctx.response.send("hello world " + ctx.request.params["name"]);
    });

    // matches both /hi and /hi/johnny requests
    router.GET("/hi/:user?", [](auto &ctx, auto /* next */)
    {
        ctx.response.send("hi " + ctx.request.params["name"]);
    });

    // matches all GET calls (those that are not catched above)
    router.GET("*", [](auto &ctx, auto /* next */)
    {
        ctx.response.send("hi " + ctx.request.params["name"]);
    });

    // matches all calls (those that are not catched above)
    router.ALL("*", [](auto &ctx, auto /* next */)
    {
        ctx.response.send("nothing matched");
    });

	app.listen();
}
```
