#pragma once

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
        // Template struct to compare two TypeEvent types
        template <TypeEvent T1, TypeEvent T2>
        struct compare {
            static constexpr bool less = T1::getStaticType() < T2::getStaticType();
        };

        // Template struct representing a sequence of TypeEvent types
        template <TypeEvent... Seq>
        struct Sequence {
            using handler_type = EventHandlerImpl<Seq...>;
        };

        // Template struct to extend a sequence by appending another sequence
        template <typename S1, typename S2>
        struct SequenceExtend;

        // Specialization for extending two sequences
        template <TypeEvent... Seq1, TypeEvent... Seq2>
        struct SequenceExtend<Sequence<Seq1...>, Sequence<Seq2...>> {
            using type = Sequence<Seq1..., Seq2...>;
        };

        // Template struct to append a TypeEvent to a sequence
        template <typename Seq, TypeEvent T>
        struct SequenceAppend;

        // Specialization for appending a TypeEvent to a sequence
        template <TypeEvent... Seq, TypeEvent T>
        struct SequenceAppend<Sequence<Seq...>, T> {
            using type = Sequence<Seq..., T>;
        };

        // Template struct to partition a sequence based on a pivot TypeEvent
        template <TypeEvent Pivot, typename Seq>
        struct SequencePartition;

        // Specialization for partitioning a sequence with a single TypeEvent
        template <TypeEvent Pivot, TypeEvent T>
        struct SequencePartition<Pivot, Sequence<T>> {
            using less = std::conditional_t<compare<T, Pivot>::less, Sequence<T>, Sequence<>>;
            using greater = std::conditional_t<!compare<T, Pivot>::less, Sequence<T>, Sequence<>>;
        };

        // Specialization for partitioning a sequence with multiple TypeEvents
        template <TypeEvent Pivot, TypeEvent First, TypeEvent...Rest>
        struct SequencePartition<Pivot, Sequence<First, Rest...>> {
            using less = std::conditional_t<
                compare<First, Pivot>::less,
                typename SequenceAppend<
                    typename SequencePartition<Pivot, Sequence<Rest...>>::less,
                    First
                >::type,
                typename SequencePartition<Pivot, Sequence<Rest...>>::less
            >;

            using greater = std::conditional_t<
                !compare<First, Pivot>::less,
                typename SequenceAppend<
                    typename SequencePartition<Pivot, Sequence<Rest...>>::greater,
                    First
                >::type,
                typename SequencePartition<Pivot, Sequence<Rest...>>::greater
            >;
        };

        // Template struct for quick sorting a sequence of TypeEvents
        template <TypeEvent... E>
        struct QuickSort {
        private:
            // Template struct for inner operations during sorting
            template <typename S>
            struct SortPass;

            // Specialization for an empty sequence
            template <>
            struct SortPass<Sequence<>> {
                using type = Sequence<>;
            };

            // Specialization for a single TypeEvent in the sequence
            template <TypeEvent T>
            struct SortPass<Sequence<T>> {
                using type = Sequence<T>;
            };

            // Specialization for a sequence with multiple TypeEvents
            template <TypeEvent Pivot, TypeEvent... Rest>
            struct SortPass<Sequence<Pivot, Rest...>> {
                using less = typename SequencePartition<Pivot, Sequence<Rest...>>::less;
                using greater = typename SequencePartition<Pivot, Sequence<Rest...>>::greater;
                using type = typename SequenceExtend<
                    typename SequenceAppend<typename SortPass<less>::type, Pivot>::type,
                    typename SortPass<greater>::type
                >::type;
            };

        public:
            using sorted = typename SortPass<Sequence<E...>>::type;
        };
    }

    // The template parameters here are unordered, meaning that
    // EventHandler<A, B, C> is equivalent to EventHandler<B, C, A>
    template <TypeEvent... E>
    using EventHandler = typename _EventHandlerInternal::QuickSort<E...>::sorted::handler_type;
}