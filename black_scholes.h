#ifndef BLACK_SCHOLES_H
#define BLACK_SCHOLES_H

#include <cmath>

double norm_cdf(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2));
}

double norm_pdf(double x) {
    return std::exp(-0.5 * x * x) / std::sqrt(2 * M_PI);
}

struct OptionResult {
    double call_price;
    double put_price;
    double delta;
    double gamma;
    double vega;
    double theta;
    double rho;
};

OptionResult black_scholes_with_greeks(
    double S, double K, double T, double r, double sigma, bool is_call = true
) {
    OptionResult res;
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);

    res.call_price = S * norm_cdf(d1) - K * std::exp(-r * T) * norm_cdf(d2);
    res.put_price = K * std::exp(-r * T) * norm_cdf(-d2) - S * norm_cdf(-d1);
    res.delta = is_call ? norm_cdf(d1) : (norm_cdf(d1) - 1);
    res.gamma = norm_pdf(d1) / (S * sigma * std::sqrt(T));
    res.vega  = S * norm_pdf(d1) * std::sqrt(T);
    res.theta = - (S * norm_pdf(d1) * sigma) / (2 * std::sqrt(T)) -
                 (is_call ? r * K * std::exp(-r * T) * norm_cdf(d2)
                          : -r * K * std::exp(-r * T) * norm_cdf(-d2));
    res.rho   = is_call ? K * T * std::exp(-r * T) * norm_cdf(d2)
                        : -K * T * std::exp(-r * T) * norm_cdf(-d2);
    return res;
}

#endif
