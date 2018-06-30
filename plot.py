import pandas as pd
import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation

df = pd.read_csv("output.data")

fig = plt.figure()

x_min = df["x"].min()
x_max = df["x"].max()
y_min = df["y"].min()
y_max = df["y"].max()



ax = plt.axes(xlim=(x_min-10, x_max+10), ylim=(y_min-10, y_max+10))
line, = ax.plot([], [], 'o', lw=1)
frame_text = ax.text(0.05, 0.9, '', transform=ax.transAxes)
frame_text.set_text("")

def animate(i):
    frame_data = df[df["ts"]==i]
    x = frame_data["x"]
    y = frame_data["y"]
    line.set_data(x,y)
    frame_text.set_text(i)

    return line, frame_text


anim = animation.FuncAnimation(fig, animate, 
                               frames=df["ts"].max(), interval=.5, blit=True)
anim.save('basic_animation.mp4', fps=30, extra_args=['-vcodec', 'libx264'])
plt.show()