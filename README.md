# 📈 OptionPayoffAnalyzer

An advanced C++ and Python toolkit for **option pricing, Greeks analysis**, and **strategy payoff visualization**. Designed for developers, quants, and financial enthusiasts, this project combines **real-time data**, **financial modeling**, and **interactive plotting** in a modular, extensible codebase.

---

## 🧠 Key Features

- 📊 Real-time stock price fetch via Alpha Vantage API  
- 💰 Black-Scholes pricing of European options  
- 🧮 Calculation of all major Greeks (Delta, Gamma, Vega, Theta, Rho)  
- 🧾 ASCII and CSV-based reports of pricing and payoff  
- 📉 Strategy payoff simulation (Long Call, Put, Bull Call Spread, Straddle)  
- 📈 Interactive GUI with sliders (Jupyter Notebook using `ipywidgets`)  
- 🧩 CLI + Plotting tool for Python-based visualization  

---

## 🔧 Installation & Build Instructions

### 🔹 Prerequisites

- C++ Compiler (`g++`)
- Python 3.8+
- Internet access (for live data)
- Alpha Vantage API Key (free):  
  [https://www.alphavantage.co/support/#api-key](https://www.alphavantage.co/support/#api-key)

---

### 🔹 Build C++ Binary

```bash
g++ main.cpp -o options -lcurl
./options
```
---
###🔹 Install Python Dependencies
```bash
pip install -r requirements.txt
```
---
## 📂 File Structure
```bash
| File                         | Description                        |
|------------------------------|------------------------------------|
| `main.cpp`                   | C++ core logic                     |
| `black_scholes.h`            | Black-Scholes pricing + Greeks     |
| `stock_fetcher.h`            | Live stock price fetcher via API   |
| `json.hpp`                   | Nlohmann JSON parser               |
| `plot_payoff_diagram_extended.py` | Python static plot               |
| `OptionPayoffAnalyzer.ipynb` | Interactive Jupyter Notebook       |
| `option_result.csv`          | Output: Option prices              |
| `option_chart_data.csv`      | Output: Prices & Greeks by strike  |
| `payoff_diagram.csv`         | Output: P&L for multiple strategies|
| `requirements.txt`           | Python dependencies                |
```
---

## 🧱 C++ Files – Function Descriptions
main.cpp
Combines all logic and orchestrates the workflow:
- get_input_with_default() – Takes user input with default fallback
- ascii_chart_plot() – Plots call/put/delta values by strike using ASCII bars
- ascii_payoff_chart() – ASCII bar chart for strategy P&L across spot prices
- export_to_csv() – Writes current option values and Greeks to CSV
- export_chart_csv() – Writes strike-by-strike Greeks to CSV
- export_payoff_csv() – Writes strategy payoff data to CSV
- compute_payoff_diagram() – Calculates P&L for:
  - Long Call
  - Short Call
  -Long Put
  -Protective Put
---
black_scholes.h
Core financial model logic:
- norm_cdf(double x) – Standard normal cumulative distribution
- norm_pdf(double x) – Standard normal probability density
- black_scholes_with_greeks(...) – Returns struct OptionResult with:
  - call_price, put_price
  - delta, gamma, vega, theta, rho
---
stock_fetcher.h
Uses libcurl and json.hpp:
 - fetch_stock_price(symbol, api_key) – Hits Alpha Vantage API and extracts current stock price
---
## 🐍 Python Files – Function Descriptions
plot_payoff_diagram_extended.py
Visualizes payoff strategies with breakeven and extrema points.
- bull_call_spread(df, K1, K2) – Builds strategy with long call @K1, short call @K2
- long_straddle(df) – Adds call + put payoff at the same strike
- annotate_extrema(ax, x, y, label) – Marks max profit/loss with colored dots
- annotate_breakeven(ax, x, y, label) – Adds vertical lines where P&L crosses 0
- plot() – Main function to build chart with overlays and markers
---
OptionPayoffAnalyzer.ipynb
Interactive visualization using ipywidgets:
- Sliders: spot, strike1, strike2
- build_custom_strategies() – Calculates payoff curves
- plot_strategies() – Displays all curves in real-time with breakeven and spot line
---
## 📊 Output Files
File	Description
- option_result.csv	One-row summary of call/put and Greeks
- option_chart_data.csv	Call/put/delta/gamma/vega/theta/rho vs. strike
- payoff_diagram.csv	Strategy-wise profit/loss vs spot price
- payoff_diagram.png	Rendered chart from Python CLI
- option_prices.png, option_greeks.png	Python-generated price/Greek line charts
---
## 🧪 Example Use Case
```bash
./options
# → Enter "AAPL", expiry = 0.5 yrs, r = 0.01, sigma = 0.2
# ASCII payoff chart and CSVs generated

python plot_payoff_diagram_extended.py --spot 173 --strike1 160 --strike2 180
# → PNG chart saved with breakeven and extrema points
```
---
## 👨‍💻 Author
- Poulami Nandi
  Data Scientist | Quant Researcher | Options Enthusiast
  [LinkedIn] (https://www.linkedin.com/in/poulami-nandi-a8a12917b/)
---
## 📜 License
This project is licensed under the MIT License – free to use, modify, and contribute.
