//
// Created by faliszewskii on 05.03.24.
//

#ifndef OPENGL_SANDBOX_EVENTPUBLISHER_H
#define OPENGL_SANDBOX_EVENTPUBLISHER_H

#include <functional>
#include <vector>
#include <iostream>

namespace detail
{
    template <typename Event>
    class EventPublisher {
    public:
        typedef std::function<void(const Event&)> Handler;

        void subscribe(Handler &&handler) {
            handlers.push_back(std::move(handler));
        }

        void publish(const Event &event) {
            for (const auto &handler : handlers)
                handler(event);
        }

    private:
        std::vector<Handler> handlers;
    };

    template <typename T> struct traits : public traits<decltype(&T::operator())> {};
    template <typename C, typename R, typename A>
    struct traits<R(C::*)(const A&) const> {
        typedef A type;
    };
}

template <typename... Events>
class EventPublisher : detail::EventPublisher<Events>... {
public:
    template <typename Handler>
    void subscribe(Handler &&handler) {
        typedef typename detail::traits<Handler>::type Event;
        detail::EventPublisher<Event> &impl = *this;
        impl.subscribe(std::forward<Handler>(handler));
    }

    template <typename Event>
    void publish(const Event &event) {
        detail::EventPublisher<Event> &impl = *this;
        impl.publish(event);
    }
};

#endif //OPENGL_SANDBOX_EVENTPUBLISHER_H
