#ifndef RECURSE_ROUTER_HPP
#define RECURSE_ROUTER_HPP

// recurse router middleware definition
auto router(int foo)
{
    return [](auto req, auto &res, auto next) {
        qDebug() << req.data << req.headers << req.body;
        res = "done";
    };
}

#endif // RECURSE_ROUTER_HPP
