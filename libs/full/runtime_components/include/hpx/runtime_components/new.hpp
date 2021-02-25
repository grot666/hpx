//  Copyright (c) 2007-2021 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file new.hpp

#pragma once

#include <hpx/config.hpp>
#include <hpx/actions_base/traits/is_client.hpp>
#include <hpx/actions_base/traits/is_distribution_policy.hpp>
#include <hpx/async_base/launch_policy.hpp>
#include <hpx/components/client_base.hpp>
#include <hpx/components_base/server/create_component.hpp>
#include <hpx/components_base/traits/is_component.hpp>
#include <hpx/futures/future.hpp>
#include <hpx/naming_base/id_type.hpp>
#include <hpx/runtime_components/create_component_helpers.hpp>
#include <hpx/runtime_components/default_distribution_policy.hpp>
#include <hpx/type_support/lazy_enable_if.hpp>

#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <vector>

#if defined(DOXYGEN)
namespace hpx {
    /// \brief Create one or more new instances of the given Component type
    /// on the specified locality.
    ///
    /// This function creates one or more new instances of the given Component
    /// type on the specified locality and returns a future object for the
    /// global address which can be used to reference the new component
    /// instance.
    ///
    /// \param locality  [in] The global address of the locality where the
    ///                  new instance should be created on.
    /// \param vs        [in] Any number of arbitrary arguments (passed by
    ///                  value, by const reference or by rvalue reference)
    ///                  which will be forwarded to the constructor of
    ///                  the created component instance.
    ///
    /// \note    This function requires to specify an explicit template
    ///          argument which will define what type of component(s) to
    ///          create, for instance:
    ///          \code
    ///              hpx::future<hpx::id_type> f =
    ///                 hpx::new_<some_component>(hpx::find_here(), ...);
    ///              hpx::id_type id = f.get();
    ///          \endcode
    ///
    /// \returns The function returns different types depending on its use:\n
    ///          * If the explicit template argument \a Component represents a
    ///          component type (<code>traits::is_component<Component>::value</code>
    ///          evaluates to true), the function will return an \a hpx::future
    ///          object instance which can be used to retrieve the global
    ///          address of the newly created component.
    ///          * If the explicit template argument \a Component represents a
    ///          client side object (<code>traits::is_client<Component>::value</code>
    ///          evaluates to true), the function will return a new instance
    ///          of that type which can be used to refer to the newly created
    ///          component instance.
    ///
    template <typename Component, typename... Ts>
    <unspecified> new_(id_type const& locality, Ts&&... vs);

    /// \brief Create one new instance of the given Component type on the
    /// current locality.
    ///
    /// This function creates one new instance of the given Component
    /// type on the current locality and returns a future object for the
    /// global address which can be used to reference the new component
    /// instance.
    ///
    /// \param vs        [in] Any number of arbitrary arguments (passed by
    ///                  value, by const reference or by rvalue reference)
    ///                  which will be forwarded to the constructor of
    ///                  the created component instance.
    ///
    /// \note    This function requires to specify an explicit template
    ///          argument which will define what type of component(s) to
    ///          create, for instance:
    ///          \code
    ///              hpx::future<hpx::id_type> f =
    ///                 hpx::local_new<some_component>(...);
    ///              hpx::id_type id = f.get();
    ///          \endcode
    ///
    /// \returns The function returns different types depending on its use:\n
    ///          * If the explicit template argument \a Component represents a
    ///          component type (<code>traits::is_component<Component>::value</code>
    ///          evaluates to true), the function will return an \a hpx::future
    ///          object instance which can be used to retrieve the global
    ///          address of the newly created component. If the first argument
    ///          is `hpx::launch::sync` the function will directly return an
    ///          `hpx::id_type`.
    ///          * If the explicit template argument \a Component represents a
    ///          client side object (<code>traits::is_client<Component>::value</code>
    ///          evaluates to true), the function will return a new instance
    ///          of that type which can be used to refer to the newly created
    ///          component instance.
    ///
    /// \note    The difference of this function to \a hpx::new_ is that it can
    ///          be used in cases where the supplied arguments are non-copyable
    ///          and non-movable. All operations are guaranteed to be local
    ///          only.
    ///
    template <typename Component, typename... Ts>
    <unspecified> local_new(Ts&&... vs);

    /// \brief Create multiple new instances of the given Component type on the
    /// specified locality.
    ///
    /// This function creates multiple new instances of the given Component type
    /// on the specified locality and returns a future object for the
    /// global address which can be used to reference the new component
    /// instance.
    ///
    /// \param locality  [in] The global address of the locality where the
    ///                  new instance should be created on.
    /// \param count     [in] The number of component instances to create
    /// \param vs        [in] Any number of arbitrary arguments (passed by
    ///                  value, by const reference or by rvalue reference)
    ///                  which will be forwarded to the constructor of
    ///                  the created component instance.
    ///
    /// \note    This function requires to specify an explicit template
    ///          argument which will define what type of component(s) to
    ///          create, for instance:
    ///          \code
    ///              hpx::future<std::vector<hpx::id_type> > f =
    ///                 hpx::new_<some_component[]>(hpx::find_here(), 10, ...);
    ///              hpx::id_type id = f.get();
    ///          \endcode
    ///
    /// \returns The function returns different types depending on its use:\n
    ///          * If the explicit template argument \a Component
    ///          represents an array of a component type (i.e. \a Component[],
    ///          where <code>traits::is_component<Component>::value</code>
    ///          evaluates to true), the function will return an \a hpx::future
    ///          object instance which holds a std::vector<hpx::id_type>, where
    ///          each of the items in this vector is a global address of one
    ///          of the newly created components.
    ///          * If the explicit template argument \a Component
    ///          represents an array of a client side object type (i.e. \a Component[],
    ///          where <code>traits::is_client<Component>::value</code>
    ///          evaluates to true), the function will return an \a hpx::future
    ///          object instance which holds a std::vector<hpx::id_type>, where
    ///          each of the items in this vector is a client side instance of
    ///          the given type, each representing one of the newly created
    ///          components.
    ///
    template <typename Component, typename... Ts>
    <unspecified> new_(id_type const& locality, std::size_t count, Ts&&... vs);

    /// \brief Create one or more new instances of the given Component type
    /// based on the given distribution policy.
    ///
    /// This function creates one or more new instances of the given Component
    /// type on the localities defined by the given distribution policy and
    /// returns a future object for global address which can be used to reference
    /// the new component instance(s).
    ///
    /// \param policy    [in] The distribution policy used to decide where to
    ///                  place the newly created.
    /// \param vs        [in] Any number of arbitrary arguments (passed by
    ///                  value, by const reference or by rvalue reference)
    ///                  which will be forwarded to the constructor of
    ///                  the created component instance.
    ///
    /// \note    This function requires to specify an explicit template
    ///          argument which will define what type of component(s) to
    ///          create, for instance:
    ///          \code
    ///              hpx::future<hpx::id_type> f =
    ///                 hpx::new_<some_component>(hpx::default_layout, ...);
    ///              hpx::id_type id = f.get();
    ///          \endcode
    ///
    /// \returns The function returns different types depending on its use:\n
    ///          * If the explicit template argument \a Component represents a
    ///          component type (<code>traits::is_component<Component>::value</code>
    ///          evaluates to true), the function will return an \a hpx::future
    ///          object instance which can be used to retrieve the global
    ///          address of the newly created component.
    ///          * If the explicit template argument \a Component represents a
    ///          client side object (<code>traits::is_client<Component>::value</code>
    ///          evaluates to true), the function will return a new instance
    ///          of that type which can be used to refer to the newly created
    ///          component instance.
    ///
    template <typename Component, typename DistPolicy, typename... Ts>
    <unspecified> new_(DistPolicy const& policy, Ts&&... vs);

    /// \brief Create multiple new instances of the given Component type on the
    /// localities as defined by the given distribution policy.
    ///
    /// This function creates multiple new instances of the given Component type
    /// on the localities defined by the given distribution policy and returns
    /// a future object for the global address which can be used to reference
    /// the new component instance.
    ///
    /// \param policy    [in] The distribution policy used to decide where to
    ///                  place the newly created.
    /// \param count     [in] The number of component instances to create
    /// \param vs        [in] Any number of arbitrary arguments (passed by
    ///                  value, by const reference or by rvalue reference)
    ///                  which will be forwarded to the constructor of
    ///                  the created component instance.
    ///
    /// \note    This function requires to specify an explicit template
    ///          argument which will define what type of component(s) to
    ///          create, for instance:
    ///          \code
    ///              hpx::future<std::vector<hpx::id_type> > f =
    ///                 hpx::new_<some_component[]>(hpx::default_layout, 10, ...);
    ///              hpx::id_type id = f.get();
    ///          \endcode
    ///
    /// \returns The function returns different types depending on its use:\n
    ///          * If the explicit template argument \a Component
    ///          represents an array of a component type (i.e. \a Component[],
    ///          where <code>traits::is_component<Component>::value</code>
    ///          evaluates to true), the function will return an \a hpx::future
    ///          object instance which holds a std::vector<hpx::id_type>, where
    ///          each of the items in this vector is a global address of one
    ///          of the newly created components.\n
    ///          * If the explicit template argument \a Component
    ///          represents an array of a client side object type (i.e. \a Component[],
    ///          where <code>traits::is_client<Component>::value</code>
    ///          evaluates to true), the function will return an \a hpx::future
    ///          object instance which holds a std::vector<hpx::id_type>, where
    ///          each of the items in this vector is a client side instance of
    ///          the given type, each representing one of the newly created
    ///          components.
    ///
    template <typename Component, typename DistPolicy, typename... Ts>
    <unspecified> new_(DistPolicy const& policy, std::size_t count, Ts&&... vs);
}    // namespace hpx

#else

namespace hpx { namespace components {
    ///////////////////////////////////////////////////////////////////////////
    namespace detail {
        // create a single instance of a component
        template <typename Component>
        struct new_component
        {
            using type = hpx::future<hpx::id_type>;

            template <typename... Ts>
            static type call(hpx::id_type const& locality, Ts&&... vs)
            {
                return components::create_async<Component>(
                    locality, std::forward<Ts>(vs)...);
            }

            template <typename DistPolicy, typename... Ts>
            static type call(DistPolicy const& policy, Ts&&... vs)
            {
                return policy.template create<Component>(
                    std::forward<Ts>(vs)...);
            }
        };

        // create multiple component instances
        template <typename Component>
        struct new_component<Component[]>
        {
            using type = hpx::future<std::vector<hpx::id_type>>;

            template <typename... Ts>
            static type call(
                hpx::id_type const& locality, std::size_t count, Ts&&... vs)
            {
                return components::bulk_create_async<Component>(
                    locality, count, std::forward<Ts>(vs)...);
            }

            template <typename DistPolicy, typename... Ts>
            static type call(
                DistPolicy const& policy, std::size_t count, Ts&&... vs)
            {
                using bulk_locality_result =
                    typename DistPolicy::bulk_locality_result;

                hpx::future<std::vector<bulk_locality_result>> f =
                    policy.template bulk_create<Component>(
                        count, std::forward<Ts>(vs)...);

                return f.then(launch::sync,
                    [count](hpx::future<std::vector<bulk_locality_result>>&& f)
                        -> std::vector<hpx::id_type> {
                        std::vector<hpx::id_type> result;
                        result.reserve(count);

                        for (bulk_locality_result& r : f.get())
                        {
                            std::move(r.second.begin(), r.second.end(),
                                std::back_inserter(result));
                        }
                        return result;
                    });
            }
        };

        ///////////////////////////////////////////////////////////////////////
        template <typename Component>
        struct local_new_component
        {
            using type = hpx::future<hpx::id_type>;

            template <typename... Ts>
            static type call(Ts&&... ts)
            {
                using component_type = typename Component::wrapping_type;

                hpx::id_type id(components::server::create<component_type>(
                                    std::forward<Ts>(ts)...),
                    hpx::id_type::managed);
                return hpx::make_ready_future(std::move(id));
            }
        };

        template <typename Component>
        struct local_new_component<Component[]>
        {
            using type = hpx::future<std::vector<hpx::id_type>>;

            template <typename... Ts>
            static type call(std::size_t count, Ts&&... ts)
            {
                using component_type = typename Component::wrapping_type;

                std::vector<hpx::id_type> result;
                result.reserve(count);

                for (std::size_t i = 0; i != count; ++i)
                {
                    result.push_back(hpx::id_type(
                        components::server::create<component_type>(ts...),
                        hpx::id_type::managed));
                }

                return hpx::make_ready_future(result);
            }
        };

        // same as above, just fully synchronous
        template <typename Component>
        struct local_new_component_sync
        {
            using type = hpx::id_type;

            template <typename... Ts>
            static type call(Ts&&... ts)
            {
                using component_type = typename Component::wrapping_type;

                hpx::id_type id(components::server::create<component_type>(
                                    std::forward<Ts>(ts)...),
                    hpx::id_type::managed);

                return id;
            }
        };

        template <typename Component>
        struct local_new_component_sync<Component[]>
        {
            using type = std::vector<hpx::id_type>;

            template <typename... Ts>
            static type call(std::size_t count, Ts&&... ts)
            {
                using component_type = typename Component::wrapping_type;

                std::vector<hpx::id_type> result;
                result.reserve(count);

                for (std::size_t i = 0; i != count; ++i)
                {
                    result.push_back(hpx::id_type(
                        components::server::create<component_type>(ts...),
                        hpx::id_type::managed));
                }

                return result;
            }
        };
    }    // namespace detail

    ///////////////////////////////////////////////////////////////////////////
    template <typename Component, typename... Ts>
    inline typename util::lazy_enable_if<
        traits::is_component_or_component_array<Component>::value,
        detail::new_component<Component>>::type
    new_(id_type const& locality, Ts&&... vs)
    {
        if (naming::get_locality_id_from_id(locality) ==
            agas::get_locality_id())
        {
            return detail::local_new_component<Component>::call(
                std::forward<Ts>(vs)...);
        }

        return detail::new_component<Component>::call(
            locality, std::forward<Ts>(vs)...);
    }

    template <typename Component, typename DistPolicy, typename... Ts>
    inline typename util::lazy_enable_if<
        traits::is_component_or_component_array<Component>::value &&
            traits::is_distribution_policy<DistPolicy>::value,
        detail::new_component<Component>>::type
    new_(DistPolicy const& policy, Ts&&... vs)
    {
        return detail::new_component<Component>::call(
            policy, std::forward<Ts>(vs)...);
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace detail {

        // create a single instance of a component
        template <typename Client>
        struct new_client
        {
            using type = Client;
            using component_type = typename Client::server_component_type;

            template <typename... Ts>
            static type call(hpx::id_type const& locality, Ts&&... vs)
            {
                return make_client<Client>(
                    components::create_async<component_type>(
                        locality, std::forward<Ts>(vs)...));
            }

            template <typename DistPolicy, typename... Ts>
            static type call(DistPolicy const& policy, Ts&&... vs)
            {
                return make_client<Client>(
                    policy.template create<component_type>(
                        std::forward<Ts>(vs)...));
            }
        };

        // create multiple client instances
        template <typename Client>
        struct new_client<Client[]>
        {
            using type = hpx::future<std::vector<Client>>;
            using component_type = typename Client::server_component_type;

            template <typename... Ts>
            static type call(Ts&&... vs)
            {
                return new_component<component_type[]>::call(
                    std::forward<Ts>(vs)...)
                    .then(hpx::launch::sync,
                        [](hpx::future<std::vector<hpx::id_type>>&& v)
                            -> std::vector<Client> {
                            return make_clients<Client>(v.get());
                        });
            }
        };

        ///////////////////////////////////////////////////////////////////////
        template <typename Client>
        struct local_new_client
        {
            using type = Client;
            using component_type =
                typename Client::server_component_type::wrapping_type;

            template <typename... Ts>
            static type call(Ts&&... ts)
            {
                hpx::id_type id(components::server::create<component_type>(
                                    std::forward<Ts>(ts)...),
                    hpx::id_type::managed);
                return make_client<Client>(std::move(id));
            }
        };

        template <typename Client>
        struct local_new_client<Client[]>
        {
            using type = hpx::future<std::vector<Client>>;
            using component_type =
                typename Client::server_component_type::wrapping_type;

            template <typename... Ts>
            static type call(Ts&&... ts)
            {
                return local_new_component<component_type[]>::call(
                    std::forward<Ts>(ts)...)
                    .then(hpx::launch::sync,
                        [](hpx::future<std::vector<hpx::id_type>>&& v)
                            -> std::vector<Client> {
                            return make_clients<Client>(v.get());
                        });
            }
        };
    }    // namespace detail

    ///////////////////////////////////////////////////////////////////////////
    template <typename Client, typename... Ts>
    inline typename util::lazy_enable_if<
        traits::is_client_or_client_array<Client>::value,
        detail::new_client<Client>>::type
    new_(id_type const& locality, Ts&&... vs)
    {
        if (naming::get_locality_id_from_id(locality) ==
            agas::get_locality_id())
        {
            return detail::local_new_client<Client>::call(
                std::forward<Ts>(vs)...);
        }

        return detail::new_client<Client>::call(
            locality, std::forward<Ts>(vs)...);
    }

    template <typename Client, typename DistPolicy, typename... Ts>
    inline typename util::lazy_enable_if<
        traits::is_client_or_client_array<Client>::value &&
            traits::is_distribution_policy<DistPolicy>::value,
        detail::new_client<Client>>::type
    new_(DistPolicy const& policy, Ts&&... vs)
    {
        return detail::new_client<Client>::call(
            policy, std::forward<Ts>(vs)...);
    }

    ///////////////////////////////////////////////////////////////////////////
    // Same as above, but just on this locality. This does not go through an
    // action, that means that the constructor arguments can be non-copyable
    // and non-movable.
    template <typename Component>
    inline typename util::lazy_enable_if<
        traits::is_component_or_component_array<Component>::value,
        detail::local_new_component<Component>>::type
    local_new()
    {
        return detail::local_new_component<Component>::call();
    }

    template <typename Component, typename T1, typename... Ts>
    inline typename util::lazy_enable_if<
        traits::is_component_or_component_array<Component>::value &&
            !std::is_same<typename std::decay<T1>::type,
                launch::sync_policy>::value,
        detail::local_new_component<Component>>::type
    local_new(T1&& t1, Ts&&... ts)
    {
        return detail::local_new_component<Component>::call(
            std::forward<T1>(t1), std::forward<Ts>(ts)...);
    }

    template <typename Component, typename... Ts>
    inline typename util::lazy_enable_if<
        traits::is_component_or_component_array<Component>::value,
        detail::local_new_component_sync<Component>>::type
    local_new(launch::sync_policy, Ts&&... ts)
    {
        return detail::local_new_component_sync<Component>::call(
            std::forward<Ts>(ts)...);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Client, typename... Ts>
    inline typename util::lazy_enable_if<
        traits::is_client_or_client_array<Client>::value,
        detail::local_new_client<Client>>::type
    local_new(Ts&&... ts)
    {
        return detail::local_new_client<Client>::call(std::forward<Ts>(ts)...);
    }
}}    // namespace hpx::components

namespace hpx {

    using hpx::components::local_new;
    using hpx::components::new_;
}    // namespace hpx

#endif
