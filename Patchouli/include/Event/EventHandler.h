#pragma once

#include "PatchouliPch.h"
#include "Event/Event.h"
#include "Event/EventManager.h"

namespace Patchouli
{
    class EventHandlerBase : public PObject
    {
    public:
        EventHandlerBase(EventCallback&& callback)
            : callback(std::move(callback))
        {
        }

        virtual ~EventHandlerBase() = default;

        // Operator overload to invoke the event callback
        void operator()(Ref<Event> event) const
        {
            callback(event);
        }

    protected:
        EventCallback callback; // Callback function to handle events
    };

    template <TypeEvent... Args>
    class EventHandlerImpl;

    template <TypeEvent E>
    class EventHandlerImpl<E> : public EventHandlerBase
    {
    public:
        EventHandlerImpl(EventCallback&& callback)
            : EventHandlerBase(std::move(callback))
        {
        }
    };

    template <TypeEvent E, TypeEvent... Rest>
    class EventHandlerImpl<E, Rest...> : public EventHandlerImpl<Rest...>
    {
    public:
        EventHandlerImpl(EventCallback&& callback)
            : EventHandlerImpl<Rest...>(std::move(callback))
        {
        }
    };

    namespace _EventHandlerInternal {
        template <TypeEvent... Seq>
        struct Sequence {
            using type = EventHandlerImpl<Seq...>;
        };

        template <typename S1, typename S2>
        struct SequenceExtension;

        template <typename... Seq1, typename... Seq2>
        struct SequenceExtension<Sequence<Seq1...>, Sequence<Seq2...>> {
            using type = Sequence<Seq1..., Seq2...>;
        };

        template <typename T1, typename T2>
        struct compare {
            static constexpr bool less = T1::getStaticType() < T2::getStaticType();
        };

        template <typename P, typename S>
        struct SequencePartition;

        template <typename P, typename T>
        struct SequencePartition<P, Sequence<T>> {
            using less = std::conditional_t<compare<T, P>::less, Sequence<T>, Sequence<>>;
            using greater = std::conditional_t<!compare<T, P>::less, Sequence<T>, Sequence<>>;
        };

        template <typename P, typename T, typename...Others>
        struct SequencePartition<P, Sequence<T, Others...>> {
            using less = std::conditional_t<
                compare<T, P>::less,
                typename SequenceExtension<Sequence<T>, typename SequencePartition<P, Sequence<Others...>>::less>::type,
                typename SequencePartition<P, Sequence<Others...>>::less
            >;
            using greater = std::conditional_t <
                !compare<T, P>::less,
                typename SequenceExtension<Sequence<T>, typename SequencePartition<P, Sequence<Others...>>::greater>::type,
                typename SequencePartition<P, Sequence<Others...>>::greater
            >;
        };

        template <typename... Ts>
        struct TemplateSort {
        private:
            template <typename S>
            struct Partition;

            template <typename T>
            struct Partition<Sequence<T>> {
                using type = Sequence<T>;
            };

            template <>
            struct Partition<Sequence<>> {
                using type = Sequence<>;
            };

            template <typename Pivot, typename... Rest>
            struct Partition<Sequence<Pivot, Rest...>> {
                using less = typename SequencePartition<Pivot, Sequence<Rest...>>::less;
                using greater = typename SequencePartition<Pivot, Sequence<Rest...>>::greater;
                using type = typename SequenceExtension<
                    typename SequenceExtension<typename Partition<less>::type, Sequence<Pivot>>::type,
                    typename Partition<greater>::type
                >::type;
            };

        public:
            using type = typename Partition<Sequence<Ts...>>::type;
        };
    }
    template <TypeEvent... E>
    using EventHandler = _EventHandlerInternal::TemplateSort<E...>::type::type;
}