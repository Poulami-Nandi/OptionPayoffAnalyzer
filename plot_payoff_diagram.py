import pandas as pd
import matplotlib.pyplot as plt

# Load the payoff CSV
df = pd.read_csv("payoff_diagram.csv")

# Plot all payoff strategies
plt.figure(figsize=(12, 6))
plt.plot(df["SpotPrice"], df["LongCall"], label="Long Call", linewidth=2)
plt.plot(df["SpotPrice"], df["ShortCall"], label="Short Call", linewidth=2)
plt.plot(df["SpotPrice"], df["LongPut"], label="Long Put", linewidth=2)
plt.plot(df["SpotPrice"], df["ProtectivePut"], label="Protective Put", linewidth=2)

# Add grid, title, labels, legend
plt.axhline(0, color='black', linestyle='--', linewidth=1)
plt.title("Option Strategy Payoff Diagrams at Expiry")
plt.xlabel("Spot Price at Expiry")
plt.ylabel("Profit / Loss ($)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("payoff_diagram.png")
plt.show()
