#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <utility>
#include <algorithm>
#include "black_scholes.h"
#include "stock_fetcher.h"

// ===== Helper Types =====
template <typename T>
T get_input_with_default(const std::string& prompt, T default_val) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    if (input.empty()) return default_val;
    try {
        return static_cast<T>(std::stod(input));
    } catch (...) {
        return default_val;
    }
}

struct PayoffPoint {
    int spot_price;
    double long_call;
    double short_call;
    double long_put;
    double protective_put;
};

// ===== CSV Export =====
void export_to_csv(const std::string& filename, const OptionResult& res, double S, double K, double T, double r, double sigma) {
    std::ofstream file(filename);
    file << "Spot Price,Strike Price,Time to Expiry,Risk-Free Rate,Volatility\n";
    file << S << "," << K << "," << T << "," << r << "," << sigma << "\n\n";
    file << "Call Price,Put Price,Delta,Gamma,Vega,Theta,Rho\n";
    file << res.call_price << "," << res.put_price << ","
         << res.delta << "," << res.gamma << ","
         << res.vega << "," << res.theta << ","
         << res.rho << "\n";
    file.close();
    std::cout << "Saved option result to " << filename << "\n";
}

void export_chart_csv(const std::string& filename, const std::vector<std::pair<int, OptionResult>>& data) {
    std::ofstream file(filename);
    file << "Strike,CallPrice,PutPrice,Delta,Gamma,Vega,Theta,Rho\n";
    for (const auto& [strike, res] : data) {
        file << strike << "," << res.call_price << "," << res.put_price << ","
             << res.delta << "," << res.gamma << "," << res.vega << ","
             << res.theta << "," << res.rho << "\n";
    }
    file.close();
    std::cout << "Saved full chart data to " << filename << "\n";
}

void export_payoff_csv(const std::string& filename, const std::vector<PayoffPoint>& data) {
    std::ofstream file(filename);
    file << "SpotPrice,LongCall,ShortCall,LongPut,ProtectivePut\n";
    for (const auto& p : data) {
        file << p.spot_price << "," << p.long_call << "," << p.short_call
             << "," << p.long_put << "," << p.protective_put << "\n";
    }
    file.close();
    std::cout << "Saved payoff diagram to " << filename << "\n";
}

// ===== ASCII Chart =====
void ascii_chart_plot(const std::string& title, const std::vector<std::pair<int, double>>& data) {
    std::cout << "\n--- " << title << " ---\n";
    for (const auto& [strike, value] : data) {
        int bar_len = static_cast<int>(value);
        std::cout << "K=" << std::setw(4) << strike << " | ";
        for (int i = 0; i < bar_len; ++i) std::cout << "*";
        std::cout << " (" << std::fixed << std::setprecision(2) << value << ")\n";
    }
}

void ascii_payoff_chart(const std::string& title, const std::vector<std::pair<int, double>>& data) {
    std::cout << "\n--- " << title << " ---\n";
    for (const auto& [S, profit] : data) {
        int offset = 30;
        int val = static_cast<int>(profit);
        std::cout << "S=" << std::setw(4) << S << " | ";
        if (val >= 0) {
            std::cout << std::string(offset, ' ') << std::string(val, '*') << " ($" << profit << ")\n";
        } else {
            std::cout << std::string(offset + val, ' ') << std::string(-val, '-') << " ($" << profit << ")\n";
        }
    }
}

// ===== Payoff Computation =====
std::vector<PayoffPoint> compute_payoff_diagram(double strike, double premium, int range = 30) {
    std::vector<PayoffPoint> results;

    for (int S = static_cast<int>(strike - range); S <= static_cast<int>(strike + range); ++S) {
        double lc = std::max(0.0, S - strike) - premium;
        double sc = premium - std::max(0.0, S - strike);
        double lp = std::max(0.0, strike - S) - premium;
        double pp = (S - strike) + lp;  // protective put = stock + long put

        results.push_back({S, lc, sc, lp, pp});
    }

    return results;
}

// ===== Main Program =====
int main() {
    std::string ticker, api_key;
    std::cout << "Enter API Key for Alpha Vantage: ";
    std::getline(std::cin, api_key);

    std::cout << "Enter Stock Symbol (e.g., AAPL): ";
    std::getline(std::cin, ticker);

    double S = fetch_stock_price(ticker, api_key);
    if (S <= 0) {
        std::cerr << "Failed to fetch stock price.\n";
        return 1;
    }
    std::cout << "Live Stock Price for " << ticker << ": $" << S << "\n";

    double K = get_input_with_default("Enter Strike Price (K) [default: spot+5]: ", S + 5);
    double T = get_input_with_default("Enter Time to Expiry (in years) [default: 0.5]: ", 0.5);
    double r = get_input_with_default("Enter Risk-Free Rate (as decimal, e.g. 0.05) [default: 0.01]: ", 0.01);
    double sigma = get_input_with_default("Enter Volatility (as decimal, e.g. 0.2) [default: 0.2]: ", 0.2);

    OptionResult res = black_scholes_with_greeks(S, K, T, r, sigma);

    std::cout << "\n--- Option Pricing ---\n";
    std::cout << "Call Price: $" << res.call_price << "\n";
    std::cout << "Put Price : $" << res.put_price << "\n";

    std::cout << "\n--- Greeks ---\n";
    std::cout << "Delta: " << res.delta << "\n";
    std::cout << "Gamma: " << res.gamma << "\n";
    std::cout << "Vega : " << res.vega << "\n";
    std::cout << "Theta: " << res.theta << "\n";
    std::cout << "Rho  : " << res.rho << "\n";

    export_to_csv("option_result.csv", res, S, K, T, r, sigma);

    // ==== Chart Data ====
    std::vector<std::pair<int, OptionResult>> chart_data;
    for (int k = static_cast<int>(S - 20); k <= static_cast<int>(S + 20); k += 5) {
        OptionResult res_k = black_scholes_with_greeks(S, k, T, r, sigma);
        chart_data.push_back({k, res_k});
    }

    std::vector<std::pair<int, double>> call_prices, put_prices, deltas;
    for (const auto& [k, res_k] : chart_data) {
        call_prices.push_back({k, res_k.call_price});
        put_prices.push_back({k, res_k.put_price});
        deltas.push_back({k, res_k.delta * 100});
    }

    ascii_chart_plot("Call Price vs Strike Price", call_prices);
    ascii_chart_plot("Put Price vs Strike Price", put_prices);
    ascii_chart_plot("Delta (x100) vs Strike Price", deltas);
    export_chart_csv("option_chart_data.csv", chart_data);

    // ==== Payoff Diagrams ====
    std::vector<PayoffPoint> payoff_data = compute_payoff_diagram(K, res.call_price);

    std::vector<std::pair<int, double>> long_call_plot, short_call_plot, long_put_plot, protective_put_plot;
    for (const auto& p : payoff_data) {
        long_call_plot.push_back({p.spot_price, p.long_call});
        short_call_plot.push_back({p.spot_price, p.short_call});
        long_put_plot.push_back({p.spot_price, p.long_put});
        protective_put_plot.push_back({p.spot_price, p.protective_put});
    }

    ascii_payoff_chart("Long Call Payoff", long_call_plot);
    ascii_payoff_chart("Short Call Payoff", short_call_plot);
    ascii_payoff_chart("Long Put Payoff", long_put_plot);
    ascii_payoff_chart("Protective Put Payoff", protective_put_plot);
    export_payoff_csv("payoff_diagram.csv", payoff_data);

    return 0;
}
