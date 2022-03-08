#ifndef CPP_EXTENSION_METHOD_MIXIN_H
#define CPP_EXTENSION_METHOD_MIXIN_H

#include <vector>
#include <algorithm>
#include <functional>
#include <string>

#define MIXIN_DEFAULT(mixin_name, base_name)              \
    template<typename...Arg> \
    explicit mixin_name(Arg &&...arg)                     \
    :base_name(std::forward<Arg>(arg)...) {} \
    operator base_name() { \
        return static_cast<base_name>(*this); \
    } \
    mixin_name() = default; \
    ~mixin_name() = default; \
    mixin_name(const base_name &v) : base_name(v) {} \
    mixin_name(base_name &&v) : base_name(v) {} \
    mixin_name(const mixin_name &v) = default; \
    mixin_name(mixin_name &&) = default; \
    mixin_name &operator=(const mixin_name &v) = default;

namespace Mixin {
    namespace detail {
        template<
                typename Base,
                typename Super,
                template<typename, typename> class MixinHead,
                template<typename, typename> class ...MixinTail
        >
        struct ChainedSpec {
            using inner_type = typename ChainedSpec<Base, Super, MixinTail...>::type;
            using type = MixinHead<inner_type, Super>;
        };


        template<typename Base, typename Super, template<typename, typename> class MixinHead>
        struct ChainedSpec<Base, Super, MixinHead> {
            using type = MixinHead<Base, Super>;
        };

        template<typename Base, typename Super, template<typename, typename> class ...Mixin_>
        using MixinImpl = typename ChainedSpec<Base, Super, Mixin_...>::type;

        template<typename Base, template<typename, typename> class ...Mixin_>
        struct mixin : MixinImpl<Base, mixin<Base, Mixin_...>, Mixin_...> {
            template<typename T>
            mixin(std::initializer_list<T> list)
                    :MixinImpl<Base, mixin<Base, Mixin_...>, Mixin_...>(std::forward<std::initializer_list<T>>(list)) {}

            template<typename ...Arg>
            mixin(Arg &&... arg)
                    :MixinImpl<Base, mixin<Base, Mixin_...>, Mixin_...>(std::forward<Arg>(arg)...) {}
        };
    }


    template<template<typename, typename> class ...Mixin_>
    struct mix {
        template<typename Base>
        inline static constexpr auto in(Base base) {
            return static_cast<detail::mixin<Base, Mixin_...>>(base);
        }

        template<typename Base>
        using in_class = typename detail::mixin<Base, Mixin_...>;
    };
};

namespace IterablePatch {
    template<typename Base, typename Super>
    struct ForEach : Base {
        MIXIN_DEFAULT(ForEach, Base)

        using value_type = typename Base::value_type;

        auto for_each(std::function<void(value_type &)> f) {
            std::for_each(this->begin(), this->end(), f);
            return static_cast<Super>(*this);
        }
    };

    template<typename Base, typename Super>
    struct Map : Base {
        MIXIN_DEFAULT(Map, Base)

        using value_type = typename Base::value_type;

        Super map(std::function<value_type(value_type &)> f) {
            Super res;
            for (auto &v: *this) {
                res.emplace_back(f(v));
            }
            return res;
        }
    };

    template<typename Base, typename Super>
    struct Filter : Base {
        MIXIN_DEFAULT(Filter, Base)

        using value_type = typename Base::value_type;

        Super filter(std::function<bool(value_type &)> f) {
            Super res;
            for (auto &v: *this) {
                if (f(v)) {
                    res.emplace_back(v);
                }
            }
            return res;
        }
    };

    template<typename Base, typename Super>
    struct ForEachIndexed : Base {
        MIXIN_DEFAULT(ForEachIndexed, Base)

        using value_type = typename Base::value_type;

        void for_each_indexed(std::function<void(size_t, value_type &)> f) {
            for (size_t i = 0; i < this->size(); ++i) {
                f(i, this->operator[](i));
            }
        }
    };
};

#endif //CPP_EXTENSION_METHOD_MIXIN_H
