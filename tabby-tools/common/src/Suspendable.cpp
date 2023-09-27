#include "Suspendable.hpp"

using namespace common;

Suspendable::Suspendable() :
    is_suspended_{false},
    lock_{}
{}

Suspendable::~Suspendable()
{}

Suspendable::Suspendable(const Suspendable& to_copy):
    is_suspended_{to_copy.is_suspended_},
    lock_{}
{}

Suspendable::Suspendable(Suspendable&& to_move):
    is_suspended_{to_move.is_suspended_},
    lock_{}
{}

Suspendable& Suspendable::operator=(const Suspendable& to_copy_assign)
{
    {
        std::lock_guard<std::mutex> guard{lock_};
        is_suspended_ = to_copy_assign.is_suspended_;
    }
    return *this;
}

Suspendable& Suspendable::operator=(Suspendable&& to_move_assign)
{
    {
        std::lock_guard<std::mutex> guard{lock_};
        is_suspended_ = to_move_assign.is_suspended_;
    }
    return *this;
}

void Suspendable::suspend()
{
    std::lock_guard<std::mutex> guard{lock_};
    is_suspended_ = true;
}

void Suspendable::resume()
{
    std::lock_guard<std::mutex> guard{lock_};
    is_suspended_ = false;
}

bool Suspendable::is_suspended()
{
    std::lock_guard<std::mutex> guard{lock_};
    return is_suspended_;
}