#pragma once

namespace pg
{
  namespace detail
  {
    template<typename T, typename Time, typename F, typename ... Args>
    inline T integrateEuler(const T& y, const Time dx, F f, const Args&... args)
    {
      return y + f(args...) * dx;
    }
  }

#if defined(PG_INTEGRATOR_RUNGE_KUTTA_POW4)

  #error "Runge-Kutta integrator not implemented"

#elif defined(PG_INTEGRATOR_HEUN)

  template<typename Y, typename Time, typename F>
  inline Y integrate(const Y& yn, const Time dx, F f, const Y& x)
  {
    return yn + (1 / 2) * (f(x) + f(detail::integrateEuler(y, dx, [](const Y& x) {
      return x;
    }, x))) * dx;
  }

#else

  template<typename Y, typename Time, typename F, typename ... Args>
  inline Y integrate(const Y& yn, const Time dx, F f, const Args&... args)
  {
    return detail::integrateEuler(yn, dx, f, args...);
  }

#endif

  template<typename Y, typename Time>
  inline Y integrate(const Y& yn, const Time dx, const Y& x)
  {
    return integrate(yn, dx, [](const Y& x) {
      return x;
    }, x);
  }
}