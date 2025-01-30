import numpy as np
import matplotlib.pyplot as plt
import imageio

# Load data
with open("fsi_data.txt", "r") as f:
    raw_data = f.read().strip().split("\n\n")

frames = [np.loadtxt(data.split("\n")) for data in raw_data]

# Create GIF
filenames = []
for idx, frame in enumerate(frames):
    x = np.linspace(0, 1, len(frame))
    
    fig, ax = plt.subplots(figsize=(6, 3))
    ax.plot(x, frame[:, 0], label="Structure Displacement")
    ax.plot(x, frame[:, 1], label="Fluid Velocity", linestyle="dashed")
    
    ax.set_ylim(-0.2, 0.2)
    ax.legend()
    ax.set_title(f"Time Step {idx * 10}")
    
    filename = f"frames/frame_{idx}.png"
    plt.savefig(filename)
    plt.close()
    filenames.append(filename)

# Convert images to GIF
imageio.mimsave("fsi_simulation.gif", [imageio.imread(f) for f in filenames], fps=50)

print("GIF saved as fsi_simulation.gif")
