#pragma once

namespace pg
{
  namespace detail
  {
    template<typename Time, typename F, typename Y, typename ... Args>
    inline Y integrateEuler(const Y& y, const Time dx, F f, const Args&... args)
    {
      return y + f(args...) * dx;
    }
  }

#if defined(PG_INTEGRATOR_RUNGE_KUTTA_POW4)

  template<typename Time, typename F, typename Y>
  inline Y integrate(const Y& y, const Time dx, F f, const Y& x)
  {
    const auto k0 = f(x);
    const auto k1 = f(x + k0 / 2.f * dx);
    const auto k2 = f(x + k1 / 2.f * dx);
    const auto k3 = f(x + k2 * dx);

    return y + (dx / 6.f) * (k0 + k1 + k2 + k3);
  }

#elif defined(PG_INTEGRATOR_HEUN)

  template<typename Time, typename F, typename Y>
  inline Y integrate(const Y& y, const Time dx, F f, const Y& x)
  {
    return y + (1.f / 2.f) * (f(x) + f(detail::integrateEuler(x, dx, [](const Y& x) {
      return x;
    }, x))) * dx;
  }

#else

  template<typename Time, typename F, typename Y, typename ... Args>
  inline Y integrate(const Y& y, const Time dx, F f, const Args&... args)
  {
    return detail::integrateEuler(y, dx, f, args...);
  }

#endif

  template<typename Time, typename Y>
  inline Y integrate(const Y& y, const Time dx, const Y& x)
  {
    return integrate(y, dx, [](const Y& x) {
      return x;
    }, x);
  }
}