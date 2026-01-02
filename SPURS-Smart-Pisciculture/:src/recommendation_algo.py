"""
SPURS: Smart Pisciculture User Recommendation System
Module: Breed-Specific Recommendation Engine
Author: Kesava Satish Boppana

Description:
This script simulates the decision-making logic of the SPURS system.
It takes real-time water quality data and compares it against a 
knowledge base of 12 fish species (derived from project datasheets) 
to provide actionable recommendations.
"""

class FishSpecies:
    def __init__(self, name, temp_range, ph_range, do_range):
        self.name = name
        self.temp_range = temp_range # (min, max)
        self.ph_range = ph_range     # (min, max)
        self.do_range = do_range     # (min, max)

# Knowledge Base (Sample of the 12 species from datasheets)
species_db = {
    "Rohu":    FishSpecies("Rohu",    (25.0, 30.0), (6.5, 8.5), (4.0, 10.0)),
    "Catla":   FishSpecies("Catla",   (24.0, 29.0), (6.5, 8.0), (4.5, 9.5)),
    "Mrigal":  FishSpecies("Mrigal",  (26.0, 31.0), (7.0, 8.5), (4.0, 9.0)),
    "Tilapia": FishSpecies("Tilapia", (20.0, 35.0), (5.5, 9.0), (3.0, 10.0))
}

def analyze_water_quality(selected_fish, temp, ph, do):
    """
    Analyzes sensor data against breed-specific thresholds.
    Returns: Recommendation String
    """
    if selected_fish not in species_db:
        return "Error: Unknown Species"
    
    fish = species_db[selected_fish]
    alerts = []

    # Temperature Logic
    if temp < fish.temp_range[0]:
        alerts.append(f"CRITICAL: Temp too low for {fish.name}. Turn on Heater.")
    elif temp > fish.temp_range[1]:
        alerts.append(f"WARNING: Temp too high for {fish.name}. Increase Aeration.")

    # pH Logic
    if ph < fish.ph_range[0]:
        alerts.append(f"CRITICAL: Water Acidic. Add Lime (CaO).")
    elif ph > fish.ph_range[1]:
        alerts.append(f"CRITICAL: Water Alkaline. Add Gypsum.")

    # Dissolved Oxygen Logic
    if do < fish.do_range[0]:
        alerts.append(f"EMERGENCY: Low Oxygen. Activate Aerators Immediately.")

    if not alerts:
        return f"Status Optimal for {fish.name}. No action needed."
    else:
        return " | ".join(alerts)

# --- Test Case Simulation ---
if __name__ == "__main__":
    # Simulating sensor input from ESP32
    current_temp = 32.5  # High temp
    current_ph = 6.2     # Acidic
    current_do = 3.8     # Low Oxygen
    target_fish = "Rohu"

    print(f"--- Analysis for {target_fish} ---")
    print(f"Sensor Readings: Temp={current_temp}C, pH={current_ph}, DO={current_do}mg/L")
    recommendation = analyze_water_quality(target_fish, current_temp, current_ph, current_do)
    print(f"System Output: {recommendation}")