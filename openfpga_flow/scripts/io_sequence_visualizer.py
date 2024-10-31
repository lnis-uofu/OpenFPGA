"""
=========================================
Represetes IO Sequence in OpenFPGA Engine
=========================================

This example demonstrates the ``OpenFPGA_Arch`` class which parses the
`VPR` and `OpenFPGA` Architecture file and provides logical information.

.. image:: ../../../examples/OpenFPGA_basic/_sample_io_sequence.svg
   :width: 60%
   :align: center

Author: Ganesh Gore

"""

import math
import svgwrite
from svgwrite.container import Group


def draw_connections(width, height, connections):
    """
    Draw connection sequence
    """
    dwg = svgwrite.Drawing()

    DRAW_WIDTH = (width + 2) * SCALE
    DRAW_HEIGHT = (height + 2) * SCALE
    # set user coordinate space
    dwg.viewbox(width=DRAW_WIDTH, height=DRAW_HEIGHT, miny=-1 * DRAW_HEIGHT)

    dwg_main = Group(id="Main", transform="scale(1,-1)")
    dwg.add(dwg_main)

    for w in range(1, width + 2):
        dwg_main.add(dwg.line((w * SCALE, SCALE), (w * SCALE, (height + 1) * SCALE), stroke="red"))

    for h in range(1, height + 2):
        dwg_main.add(dwg.line((SCALE, h * SCALE), ((width + 1) * SCALE, h * SCALE), stroke="red"))

    path = "M "
    for point in connections:
        path += " %d %d " % ((point[0] + 0.5) * SCALE, (point[1] + 0.5) * SCALE)

    dwg_main.add(dwg.path(path, stroke="blue", fill="none", stroke_width="2px"))
    dwg.saveas("_sample_io_sequence.svg", pretty=True)


SCALE = 20
FPGA_WIDTH = 40
FPGA_HEIGHT = 15

W = max(FPGA_WIDTH, FPGA_HEIGHT)
W2 = math.floor(W / 2) + 1

connections = []
xmin, xmax = 1, FPGA_WIDTH
ymin, ymax = 1, FPGA_HEIGHT

while (xmin < xmax) and (ymin < ymax):
    print(xmin, ymin, end=" -> ")
    print(xmax, ymax)

    x = xmin
    for y in range(ymin, ymax + 1):
        connections.append((x, y))
    y = ymax
    for x in range(xmin, xmax + 1):
        connections.append((x, y))

    x = xmax
    for y in range(ymin, ymax + 1)[::-1]:
        connections.append((x, y))

    y = ymin
    for x in range(xmin, xmax + 1)[::-1][:-1]:
        connections.append((x, y))

    xmin += 1
    ymin += 1
    xmax -= 1
    ymax -= 1


if FPGA_HEIGHT % 2 == 1:  # if height is odd
    if ymin == ymax:  # if touching vertically
        y = ymin
        for x in range(xmin, xmax + 1):
            connections.append((x, y))


if FPGA_WIDTH % 2 == 1:  # if width is odd
    if xmin == xmax:  # if touching horizontally
        x = xmin
        for y in range(ymin, ymax + 1):
            connections.append((x, y))

# print(connections)
if connections:
    draw_connections(FPGA_WIDTH, FPGA_HEIGHT, connections)
else:
    # Dummy draw
    draw_connections(FPGA_WIDTH, FPGA_HEIGHT, [(1, 1)])
