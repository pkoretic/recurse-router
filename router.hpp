#ifndef RECURSE_ROUTER_HPP
#define RECURSE_ROUTER_HPP

#include <recurse.hpp>
#include <QString>
#include <QHash>
#include <QList>
#include <QPair>
#include <QRegularExpression>

namespace Module
{
    class Router
    {
    private:
        bool m_debug = false;

        void debug(QString message)
        {
            if (m_debug)
                std::cout << "(router debug) " << message.toStdString() << std::endl;
        }

        using HTTP_Method = QList<QPair<QString, Downstream>>;

        HTTP_Method HTTP_GET;
        HTTP_Method HTTP_PUT;
        HTTP_Method HTTP_POST;
        HTTP_Method HTTP_PATCH;
        HTTP_Method HTTP_DELETE;
        HTTP_Method HTTP_HEAD;
        HTTP_Method HTTP_OPTIONS;

        QHash<QString, HTTP_Method*> methods{
           { "GET", &HTTP_GET },
           { "PUT", &HTTP_PUT },
           { "POST", &HTTP_POST },
           { "PATCH", &HTTP_PATCH },
           { "DELETE", &HTTP_DELETE },
           { "HEAD", &HTTP_HEAD },
           { "OPTIONS", &HTTP_OPTIONS },
        };

        // https://tools.ietf.org/html/rfc3986#appendix-A
        QString path_to_regexp(QString path)
        {
            path.prepend("^");

            auto colon = path.indexOf(":");
            if(colon != -1)
            {
                auto question = path.indexOf("?", colon);
                if(question != -1)
                    path.replace(QRegularExpression("/:[A-z0-9]+\\?"), "(?:/([^/]+?))?(?:/(?=$))?");
                else
                    path.replace(QRegularExpression(":[A-z0-9]+"), "[^/]+?");
            }

            auto asterix = path.indexOf("*");
            if(asterix != -1)
                path.insert(asterix, ".");

            path.append("$");

            return path;
        }

    public:

        Router()
        {
            QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
            QRegExp debug_strings("(router)");

            if (debug_strings.indexIn(env.value("DEBUG")) != -1)
                m_debug = true;
        }

        void GET(QString path, Downstream next)
        {
            HTTP_GET.append(QPair<QString, Downstream>(path, next));
        }

        void PUT(QString path, Downstream next)
        {
            HTTP_PUT.append(QPair<QString, Downstream>(path, next));
        }

        void POST(QString path, Downstream next)
        {
            HTTP_POST.append(QPair<QString, Downstream>(path, next));
        }

        void PATCH(QString path, Downstream next)
        {
            HTTP_PATCH.append(QPair<QString, Downstream>(path, next));
        }

        void HEAD(QString path, Downstream next)
        {
            HTTP_HEAD.append(QPair<QString, Downstream>(path, next));
        }

        void DELETE(QString path, Downstream next)
        {
            HTTP_DELETE.append(QPair<QString, Downstream>(path, next));
        }

        void OPTIONS(QString path, Downstream next)
        {
            HTTP_OPTIONS.append(QPair<QString, Downstream>(path, next));
        }

        void ALL(QString path, Downstream next)
        {
           GET(path, next);
           PUT(path, next);
           POST(path, next);
           PATCH(path, next);
           HEAD(path, next);
           DELETE(path, next);
           OPTIONS(path, next);
        }

        auto routes()
        {
            return [this](auto &ctx, auto next)
            {
                auto request = ctx.request;

                if(!methods.contains(request.method))
                    return next();

                auto method = *(methods[request.method]);

                auto url = request.url;
                auto query = url.query();
                auto path = url.path();

                // call first route that matches
                for(auto pair : method)
                {
                    auto key = pair.first;

                    this->debug("trying path:" + key);

                    QRegularExpression regexp(this->path_to_regexp(key));
                    if(!regexp.isValid() || !regexp.match(path).hasMatch())
                        continue;

                    this->debug("path matched");

                    // parse parameters if any
                    auto colon = key.indexOf(":");
                    if(colon)
                    {
                        // we use QStringRef so we don't allocate on search
                        auto matchString = key.midRef(colon);
                        auto pathString = path.midRef(colon);

                        auto matches = matchString.split("/");
                        auto paths = pathString.split("/");

                        // match: /hello/:id/:/name
                        // request: /hello/1/test

                        for(int i = 0; i < matches.size(); i++)
                        {
                            auto match = matches.at(i);
                            colon = match.indexOf(":");
                            if(colon == -1)
                                continue;

                            auto paramKey = match.mid(colon + 1, match.indexOf("?") - 1);
                            auto paramValue = paths.at(i);

                            ctx.request.params[paramKey.toString()] = paramValue.toString();
                        }
                    }

                    auto call = pair.second;
                    call(ctx, next);

                    return;
                }

                // call next middleware if nothing matched
                next();
            };
        }
    };
}

#endif // RECURSE_ROUTER_HPP
