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
        ctx.response.send("Hello World " + ctx.request.getParam("user"));
    });

	app.listen();
}
```

### Parameters


Parameters are saved in QHash `params` property inside **Recurse** `Request` class.
To fetch values use `Request::getParam(key)` helper function:

```
ctx.request.getParam("name")
```

Standard, required params:

```
#include "router.hpp"

int main(int argc, char *argv[])
{
    Recurse app(argc, argv);

    Module::Router router;

    // matches /hello/john
    router.GET("/hello/:name", [](auto &ctx, auto /* next */)
    {
        ctx.response.send("hello world " + ctx.request.getParam("name");
    });

	app.listen();
}

```

Optional params

```

    // matches both /hi and /hi/johnny requests
    router.GET("/hi/:user?", [](auto &ctx, auto /* next */)
    {
        ctx.response.send("hi " + ctx.request.getParam("name");
    });
```

Universal, catch-all parameter

```
    // matches all GET calls (those that are not catched before hand)
    router.GET("*", [](auto &ctx, auto /* next */)
    {
        ctx.response.send("hi all");
    });

```

catch-all with Router.ALL, matches everything
```
    // matches all calls (those that are not catched before hand)
    router.ALL("*", [](auto &ctx, auto /* next */)
    {
        ctx.response.send("all matched, all catched");
    });

```
