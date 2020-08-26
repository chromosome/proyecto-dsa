import sys, pygame
from pygame.locals import*
import numpy as np

import math

def rotate(origin, point, angle):
    """
    Rotate a point counterclockwise by a given angle around a given origin.

    The angle should be given in radians.
    """
    ox, oy = origin
    px, py = point

    qx = ox + math.cos(angle) * (px - ox) - math.sin(angle) * (py - oy)
    qy = oy + math.sin(angle) * (px - ox) + math.cos(angle) * (py - oy)
    return qx, qy

import quad_tree

###################################################################

# data = np.genfromtxt("test.csv", 
# 					 dtype=np.double, 
# 					 delimiter=';', 
# 					 skip_header=1,
# 					 usecols=(5, 6),
# 					 max_rows=20000)

# print(data)

data = np.empty((1,2),dtype=np.double)
# print(data.shape)

with open("worldcitiespop_fixed.csv") as f:
	f.readline()
	count = 0
	for line in f:
		line = line.strip().split(';')
		line = line[7].split(',')
		x, y = float(line[0]), float(line[1])
		data = np.append(data, ((x, y),), axis=0)
		count += 1
		if count > 2000:
			break

def insert_data(tree, data):
	for d in data:
		# print(d)
		x = d[0]*scalex + Screen_width/2
		y = Screen_height/2 - d[1]*scaley
		insert(x, y)


# GLOBALES

lat, lon = 90, 180
# tree = quad_tree.quad_tree(lat, lon)
tree = quad_tree.quad_tree(lat, lon)

Screen_width  = 1024
Screen_height = 720

scalex = Screen_width/(lat*2)
scaley = Screen_height/(lon*2)

Color_screen = (0,0,0)
Color_line   = (255,0,0)
Color_circle = (0,255,0)
circle_radius = 2

screen = pygame.display.set_mode((Screen_width, Screen_height))

screen.blit(pygame.transform.flip(screen, False, False), (0, 0))
screen.blit(pygame.transform.rotate(screen, 45), (0, 0))

pygame.display.set_caption('Deku Tree')

screen.fill(Color_screen)
pygame.display.flip()

points = []

###################################################################

def draw_subdivision_driver(tracks, x, y, w, h):

	for track in tracks:

		if track != 'ORIGIN':
			pygame.draw.line(screen,Color_line,(x + w/2,y),(x+w/2,y+h))
			pygame.draw.line(screen,Color_line,(x,y+h/2),(x+w,y+h/2))
			# print(x,y,w,h)

		if(track == "NW"):
			w = w/2
			h = h/2

		elif(track == "NE"):
			w = w/2
			h = h/2
			x = x+w

		elif(track == "SW"):
			w = w/2
			h = h/2
			y = y+h

		elif(track == "SE"):
			w = w/2
			h = h/2
			x = x+w
			y = y+h

		# pygame.display.flip()


def draw_subdivision(tracks):
	draw_subdivision_driver(tracks, 0, 0, Screen_width, Screen_height)


def insert(x,y):
	p = Point()
	p.center = (int(x),int(y))
	p.value = 0
	p.draw()
	x_ = (x - Screen_width/2)/scalex
	y_ = (Screen_height/2 - y)/scaley
	tree.insert(x_, y_)
	tracks = tree.track(x_, y_)
	draw_subdivision(tracks)




def clear_screen():
	screen.fill(Color_screen)
	pygame.display.flip()


def mouseClicked(pos):
	p = Point()
	# print(pos)
	insert(pos[0], pos[1])
	p.center = (int(pos[0]),int(pos[1]))
	p.value = 0
	# points.append(p)
	p.draw()



###################################################################

class Point():

	center = (0,0)
	value  = 0

	def draw(self):
		pygame.draw.circle(screen, Color_circle, self.center, circle_radius)
		pygame.display.flip()

###################################################################

def main():
	# insert_data(tree, data)
	while True:
		for events in pygame.event.get():
			if events.type == QUIT:
				sys.exit(0)

			if events.type == pygame.K_ESCAPE:
				sys.exit(0)

			if events.type == pygame.MOUSEBUTTONUP:
				  pos = pygame.mouse.get_pos()
				  mouseClicked(pos)


if __name__ == '__main__':
	main()
