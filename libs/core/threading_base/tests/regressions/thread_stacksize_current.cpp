//  Copyright (c) 2020 Mikael Simberg
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// This test checks that no thread has thread_stacksize::current as its actual
// stacksize. thread_stacksize::current can be used as input when creating a
// thread, but it should always be converted to something between
// thread_stacksize::minimal and thread_stacksize::maximal when a thread has been
// created.

#include <hpx/hpx_init.hpp>

#include <hpx/modules/async_local.hpp>
#include <hpx/modules/testing.hpp>
#include <hpx/modules/threading_base.hpp>

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

void test(hpx::threads::thread_stacksize stacksize)
{
    hpx::execution::parallel_executor exec(stacksize);
    hpx::execution::parallel_executor exec_current(
        hpx::threads::thread_stacksize::current);

    hpx::async(exec, [&exec_current, stacksize]() {
        // This thread should have the stack size stacksize; it has been
        // explicitly set in the executor.
        hpx::threads::thread_stacksize self_stacksize =
            hpx::threads::get_self_stacksize_enum();
        HPX_TEST_EQ(self_stacksize, stacksize);
        HPX_TEST_NEQ(self_stacksize, hpx::threads::thread_stacksize::current);

        hpx::async(exec_current, [stacksize]() {
            // This thread should also have the stack size stacksize; it has
            // been inherited size from the parent thread.
            hpx::threads::thread_stacksize self_stacksize =
                hpx::threads::get_self_stacksize_enum();
            HPX_TEST_EQ(self_stacksize, stacksize);
            HPX_TEST_NEQ(
                self_stacksize, hpx::threads::thread_stacksize::current);
        }).get();
    }).get();
}

int hpx_main()
{
    for (hpx::threads::thread_stacksize stacksize =
             hpx::threads::thread_stacksize::minimal;
         stacksize < hpx::threads::thread_stacksize::maximal;
         stacksize = static_cast<hpx::threads::thread_stacksize>(
             static_cast<std::size_t>(stacksize) + 1))
    {
        test(stacksize);
    }

    return hpx::finalize();
}

int main(int argc, char** argv)
{
    std::vector<std::string> schedulers = {"local", "local-priority-fifo",
        "local-priority-lifo", "static", "static-priority", "abp-priority-fifo",
        "abp-priority-lifo", "shared-priority"};
    for (auto const& scheduler : schedulers)
    {
        hpx::init_params iparams;
        iparams.cfg = {"--hpx:queuing=" + std::string(scheduler)};
        std::cout << iparams.cfg[0] << std::endl;
        hpx::init(argc, argv, iparams);
    }

    return hpx::util::report_errors();
}
