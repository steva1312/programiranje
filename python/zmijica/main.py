import random
import math
import tkinter as tk
from tkinter import messagebox
import pygame
pygame.init()

width = 500
win = pygame.display.set_mode((width, width))
pygame.display.set_caption('Zmijica')
rows = 20
gap = width // rows

def message_box(subject, content):
    root = tk.Tk()
    root.attributes("-topmost", True)
    root.withdraw()
    messagebox.showinfo(subject, content)
    root.destroy()

def get_combinations():
    combinations = []
    every_combination = []
    for i in range(rows):
        combinations.append(i * gap)
    for i in range(len(combinations)):
        for combination in combinations:
            combo = (combinations[i], combination)
            every_combination.append(combo)
    return every_combination
get_combinations()

class Cube:
    def __init__(self, color, x, y, is_head):
        self.color = color
        self.width = gap
        self.x = x
        self.y = y
        self.dirx = 0
        self.diry = 0
        self.is_head = is_head

    def draw(self):
        pygame.draw.rect(win, self.color, (self.x, self.y, gap, gap))

    def move(self):
        global gameover
        if (self.x == 0 and self.dirx == -1) \
        or (self.x == width - gap and self.dirx == 1) \
        or (self.y == 0 and self.diry == -1) \
        or (self.y == width - gap and self.diry == 1):
            gameover = True
            return
        if gameover: return
        self.x += gap * self.dirx
        self.y += gap * self.diry

    def get_cords(self):
        return self.x, self.y

    def self_crash(self, cubes):
        global gameover
        for cube in cubes:
            if self.x == cube.x and self.y == cube.y and not cube.is_head:
                gameover = True
                self.x += gap * self.dirx * -1
                self.y += gap * self.diry * -1


class Food:
    def __init__(self):
        self.color = (0,255,0)
        self.cx = []
        self.cy = []
        self.x = 0
        self.y = 0

    def draw(self):
        pygame.draw.rect(win, self.color, (self.x, self.y, gap, gap))

    def set_cords(self, combinations):
        cords = combinations[ math.floor( random.random() * len(combinations) ) ]
        self.x = cords[0]
        self.y = cords[1]

    def eat_check(self, cubex, cubey):
        if self.x == cubex and self.y == cubey:
            return True


class Snake:
    def __init__(self):
        self.body_color = (255,0,0)
        self.head_color = (255,150,70)
        self.start_pos = width // 2
        self.body = [Cube(self.head_color, self.start_pos, self.start_pos, True)]
        self.breakpoints = []
        self.combinations = []
        self.food = Food()
        self.find_combinations()

    def move(self):
        global gameover
        for cube in self.body:
            for breackpoint in self.breakpoints:
                if cube.x == breackpoint[0][0] and cube.y == breackpoint[0][1]:
                    cube.dirx = breackpoint[1][0]
                    cube.diry = breackpoint[1][1]
                    if self.body[-1].get_cords() == breackpoint[0]:
                        self.breakpoints.remove(breackpoint)
            cube.move()
            if cube.is_head: cube.self_crash(self.body)
            cube.draw()

        if self.food.eat_check(self.body[0].x, self.body[0].y): self.eat()
        self.food.draw()

    def change_direction(self, dirx, diry):
        self.breakpoints.append((self.body[0].get_cords(), (dirx, diry)))

    def eat(self):
        x = self.body[-1].x + self.body[-1].dirx * -1 * gap
        y = self.body[-1].y + self.body[-1].diry * -1 * gap
        self.body.append(Cube(self.body_color, x, y, False))
        self.body[-1].dirx = self.body[-2].dirx
        self.body[-1].diry = self.body[-2].diry
        self.find_combinations()

    def find_combinations(self):
        self.combinations = get_combinations()
        for cube in self.body:
            combo = (cube.x, cube.y)
            self.combinations.remove(combo)
        self.food.set_cords(self.combinations)

    def reset(self):
        global gameover
        self.body = [Cube(self.head_color, self.start_pos, self.start_pos, True)]
        self.breakpoints = []
        self.combinations = []
        self.find_combinations()
        gameover = False


def draw_grid():
    cords = 0
    for k in range(rows):
        cords += gap
        pygame.draw.line(win, (255,255,255), (cords, 0), (cords, width))
        pygame.draw.line(win, (255,255,255), (0, cords), (width, cords))

def update_screen():
    win.fill((0,0,0))
    snake.move()
    draw_grid()
    pygame.display.update()

snake = Snake()
gameover = False
run = True
while run:
    pygame.time.delay(75)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False

    if not gameover:
        keys = pygame.key.get_pressed()
        if keys[pygame.K_UP] or keys[pygame.K_w]:
            snake.change_direction(0, -1)
        elif keys[pygame.K_DOWN] or keys[pygame.K_s]:
            snake.change_direction(0, 1)
        elif keys[pygame.K_LEFT] or keys[pygame.K_a]:
            snake.change_direction(-1, 0)
        elif keys[pygame.K_RIGHT] or keys[pygame.K_d]:
            snake.change_direction(1, 0)

    update_screen()

    if gameover:
        message_box('Game Over', 'Your score is ' + str(len(snake.body) - 1))
        snake.reset()

pygame.quit()