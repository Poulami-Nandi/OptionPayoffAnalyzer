import pandas as pd
import matplotlib.pyplot as plt

# Load payoff data
df = pd.read_csv("payoff_diagram.csv")

# ----- Custom Strategy Definitions -----
# Bull Call Spread: Long Call @ K1, Short Call @ K2
def bull_call_spread(df, lower_strike, higher_strike):
    lc = df[df["SpotPrice"] == lower_strike]["LongCall"].values[0]
    sc = df[df["SpotPrice"] == higher_strike]["ShortCall"].values[0]
    df["BullCallSpread"] = df["LongCall"] - lc + df["ShortCall"] - sc
    return df

# Long Straddle: Long Call + Long Put (at same strike)
def long_straddle(df):
    df["Straddle"] = df["LongCall"] + df["LongPut"]
    return df

# ----- Breakeven & Extrema Marker -----
def annotate_extrema(ax, x, y, label):
    idx_max = y.idxmax()
    idx_min = y.idxmin()
    ax.plot(x[idx_max], y[idx_max], 'go', label=f'Max Profit ({label})')
    ax.plot(x[idx_min], y[idx_min], 'ro', label=f'Max Loss ({label})')

def annotate_breakeven(ax, x, y, label):
    for i in range(1, len(y)):
        if y[i-1] * y[i] <= 0:  # sign change = breakeven
            be_x = (x[i] + x[i-1]) / 2
            ax.axvline(be_x, color='gray', linestyle='--', linewidth=1)
            ax.text(be_x, 0, f'{label} BE', rotation=90, fontsize=8, ha='right')

# ----- Build Strategies -----
df = bull_call_spread(df, df["SpotPrice"].min() + 15, df["SpotPrice"].min() + 25)
df = long_straddle(df)

# ----- Plot Payoff Chart -----
plt.figure(figsize=(14, 8))
x = df["SpotPrice"]

# Plot strategies
strategies = {
    "Long Call": df["LongCall"],
    "Short Call": df["ShortCall"],
    "Long Put": df["LongPut"],
    "Protective Put": df["ProtectivePut"],
    "Bull Call Spread": df["BullCallSpread"],
    "Straddle": df["Straddle"]
}

for name, series in strategies.items():
    plt.plot(x, series, label=name, linewidth=2)
    annotate_breakeven(plt.gca(), x, series, name)
    annotate_extrema(plt.gca(), x, series, name)

# Chart formatting
plt.axhline(0, color='black', linestyle='--', linewidth=1)
plt.title("Option Strategy Payoff Diagrams at Expiry")
plt.xlabel("Spot Price at Expiry")
plt.ylabel("Profit / Loss ($)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("payoff_diagram_extended.png")
plt.show()
