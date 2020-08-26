import sys, pygame
from pygame.locals import*

#import quad_tree

###################################################################

# GLOBALES

Screen_width  = 800
Screen_height = 800

Color_screen = (0,0,0)
Color_line   = (255,0,0)
Color_circle = (0,255,0)
circle_radius = 5

screen = pygame.display.set_mode((Screen_width,Screen_height))
pygame.display.set_caption('Deku Tree')

screen.fill(Color_screen)
pygame.display.flip()

#tree = quad_tree.quadtree(Screen_width,Screen_height)
points = []

###################################################################

def draw_subdivision_driver(tracks, x, y, w, h):

    pygame.draw.line(screen,Color_line,(x + w/2,y),(x+w/2,y+h))
    pygame.draw.line(screen,Color_line,(x,y+h/2),(x+w,y+h/2))

    for track in tracks:

        w = w/2
        h = h/2

        if(track == "NW"):
            x = x
            y = y
            pygame.draw.line(screen,Color_line,(x + w/2,y),(x+w/2,y+h))
            pygame.draw.line(screen,Color_line,(x,y+h/2),(x+w,y+h/2))
        elif(track == "NE"):
            x = x+w
            y = y
            pygame.draw.line(screen,Color_line,(x + w/2,y),(x+w/2,y+h))
            pygame.draw.line(screen,Color_line,(x,y+h/2),(x+w,y+h/2))
        elif(track == "SW"):
            x = x
            y = y+h
            pygame.draw.line(screen,Color_line,(x + w/2,y),(x+w/2,y+h))
            pygame.draw.line(screen,Color_line,(x,y+h/2),(x+w,y+h/2))
            print(x,y,w,h)
        elif(track == "SE"):
            x = x+w
            y = y+h
            pygame.draw.line(screen,Color_line,(x + w/2,y),(x+w/2,y+h))
            pygame.draw.line(screen,Color_line,(x,y+h/2),(x+w,y+h/2))

        pygame.display.flip()


def draw_subdivision(tracks):
    draw_subdivision_driver(tracks, 0, 0, Screen_width, Screen_height)


def insert(x,y):
    p = Point()
    p.center = (x,y)
    p.value = 0
    points.append(p)
    p.draw()


def clear_screen():
    screen.fill(Color_screen)
    pygame.display.flip()


def mouseClicked(pos):
    insert(pos[0], pos[1])



###################################################################

class Point():

    center = (0,0)
    value  = 0

    def draw(self):
        pygame.draw.circle(screen, Color_circle, self.center, circle_radius)
        pygame.display.flip()

###################################################################

def main():

    draw_subdivision(["SE","NW","NW"])

    while True:
        for events in pygame.event.get():
            if events.type == QUIT:
                sys.exit(0)

            if events.type == pygame.MOUSEBUTTONDOWN:
                  pos = pygame.mouse.get_pos()
                  mouseClicked(pos)


main()
