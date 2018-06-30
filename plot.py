import pandas as pd
import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation

data = pd.read_csv("output.data")

fig = plt.figure()
ax = plt.axes(xlim=(0, 2), ylim=(-2, 2))
line, = ax.plot([], [], 'o', lw=2)


def animate(i):
    ax.text(,10,10,"Timestamp: %d"%(i))
    frame_data = data[data["ts"]==i]
    x = frame_data["x"]
    y = frame_data["y"]
    line.set_data(x,y)
    return line,


anim = animation.FuncAnimation(fig, animate, 
                               frames=data["ts"].max(), interval=1, blit=True)
#anim.save('basic_animation.mp4', fps=30, extra_args=['-vcodec', 'libx264'])
plt.show()