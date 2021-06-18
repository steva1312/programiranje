import pygame
from tkinter import *
from tkinter.filedialog import askopenfilename, asksaveasfilename
from tkinter import messagebox

# inition
pygame.init()
WIDTH = 700
HEIGHT = 500
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Stevino Crtanje (Untitled)")

# colors
WHITE = (255,255,255)
BLACK = (0,0,0)
GRAY = (211,211,211)
BLUE = (0,0,255)
GREEN = (0,255,0)
RED = (255,0,0)
PURPLE = (255,0,255)
YELLOW = (255,255,0)
ORANGE = (255,150,0)
BROWN = (139,69,19)
DARK_GRAY = (169, 169, 169)

# functions
def chechk_if_clicked(start_x, start_y, end_x, end_y, x, y):
    if start_x < x <= end_x and start_y < y <= end_y:
        return True
    return False

def change_caption(filename):
    word = filename
    if not word: word = "Untitled"
    pygame.display.set_caption("Stevino Crtanje (" + word + ")")

# classes
class Game:
    def __init__(self):
        self.drawing = False
        self.drawing_board = DrawingBoard()
        self.tools_board = ToolsBoard()
        self.pen = Pen()
        self.is_fill = False
        self.searched_color = WHITE
        self.up = True
        self.down = True
        self.left = True
        self.right = True
        self.undo_list = []
        self.redo_list = []
        self.file_path = ""
        self.saved = True

    def run(self):
        clock = pygame.time.Clock()
        fps = 60

        run = True
        while run:
            clock.tick(fps)

            keys = pygame.key.get_pressed()

            x, y = pygame.mouse.get_pos()
            for event in pygame.event.get():
                if event.type == pygame.QUIT or keys[pygame.K_ESCAPE]:
                    self.ask_for_save("Would you like to save before closing?")

                    run = False

                if event.type == pygame.MOUSEBUTTONDOWN:
                    if chechk_if_clicked(self.drawing_board.x, self.drawing_board.y, self.drawing_board.width, self.drawing_board.width, x, y):
                        self.make_drawing(self.undo_list)

                        if self.is_fill: self.fill(x, y)

                        self.drawing = True
                        if self.is_fill: self.drawing = False
                        else: self.drawing_board.dye(x, y, self.pen.color, self.pen.weight)

                        if len(self.redo_list) > 0:
                            self.redo_list = []

                        self.saved = False

                    else: # check for buttons
                        colored_result = self.tools_board.check_colored_and_return(x, y)
                        if colored_result: self.pen.color = colored_result

                        weighted_result = self.tools_board.check_wieghted_and_return(x, y)
                        if weighted_result: self.pen.weight = weighted_result

                        if self.tools_board.fill_button.is_clicked(x, y):
                            self.is_fill = not self.is_fill

                        if self.tools_board.clear_button.is_clicked(x, y):
                            for block in self.drawing_board.blocks:
                                block.color = WHITE
                            self.undo_list = []
                            self.redo_list = []

                        if self.tools_board.undo_button.is_clicked(x, y):
                            self.undo_and_redo(self.undo_list, self.redo_list)

                        if self.tools_board.redo_button.is_clicked(x, y):
                            self.undo_and_redo(self.redo_list, self.undo_list)

                        if self.tools_board.save_button.is_clicked(x, y):
                            self.save()

                        if self.tools_board.load_button.is_clicked(x, y):
                            self.load()

                if event.type == pygame.MOUSEBUTTONUP:
                    self.drawing = False

                if event.type == pygame.MOUSEMOTION and self.drawing:
                    self.drawing_board.dye(x, y, self.pen.color, self.pen.weight)

                if len(self.undo_list) > 0:
                    self.tools_board.undo_button.color = BLACK
                else:
                    self.tools_board.undo_button.color = DARK_GRAY

                if len(self.redo_list) > 0:
                    self.tools_board.redo_button.color = BLACK
                else:
                    self.tools_board.redo_button.color = DARK_GRAY

            self.update_screen()

        pygame.quit()

    def update_screen(self):
        self.draw()
        pygame.display.update()

    def draw(self):
        self.drawing_board.draw()
        self.tools_board.draw()
        self.tools_board.set_selected_colored_btn(self.pen.color)
        self.tools_board.set_selected_weighted_btn(self.pen.weight)
        self.tools_board.set_selected_fill_btn(self.is_fill)

    def fill(self, mx, my):
        preapared_blocks = []
        unchecked_blocks = []

        for block in self.drawing_board.blocks:
            if chechk_if_clicked(block.x, block.y, block.x + self.drawing_board.block_width, block.y + self.drawing_board.block_width, mx, my):
                unchecked_blocks.append(block)
                self.searched_color = block.color

        run = True
        while run:
            for checking_block in unchecked_blocks:
                index = self.drawing_board.blocks.index(checking_block)

                if checking_block.x == 0:
                    self.left = False
                if checking_block.y == 0:
                    self.up = False
                if checking_block.x == self.drawing_board.width - self.drawing_board.block_width:
                    self.right = False
                if checking_block.y == self.drawing_board.width - self.drawing_board.block_width:
                    self.down = False

                if index - 1 >= 0 and self.up:
                    if self.drawing_board.blocks[index-1].color == self.searched_color and not self.drawing_board.blocks[index-1].fill_checked: # up
                        unchecked_blocks.append(self.drawing_board.blocks[index-1])
                        self.drawing_board.blocks[index - 1].fill_checked = True

                if index + 1 <= len(self.drawing_board.blocks) - 1 and self.down:
                    if self.drawing_board.blocks[index+1].color == self.searched_color and not self.drawing_board.blocks[index+1].fill_checked: # down
                        unchecked_blocks.append(self.drawing_board.blocks[index+1])
                        self.drawing_board.blocks[index + 1].fill_checked = True

                if index - self.drawing_board.amount >= 0 and self.left:
                    if self.drawing_board.blocks[index-self.drawing_board.amount].color == self.searched_color and not self.drawing_board.blocks[index-self.drawing_board.amount].fill_checked: # left
                        unchecked_blocks.append(self.drawing_board.blocks[index - self.drawing_board.amount])
                        self.drawing_board.blocks[index - self.drawing_board.amount].fill_checked = True

                if index + self.drawing_board.amount <= len(self.drawing_board.blocks) - 1 and self.right:
                    if self.drawing_board.blocks[index + self.drawing_board.amount].color == self.searched_color and not self.drawing_board.blocks[index + self.drawing_board.amount].fill_checked: # right
                        unchecked_blocks.append(self.drawing_board.blocks[index + self.drawing_board.amount])
                        self.drawing_board.blocks[index + self.drawing_board.amount].fill_checked = True

                self.up = True
                self.down = True
                self.left = True
                self.right = True

                unchecked_blocks.remove(checking_block)
                preapared_blocks.append(checking_block)

            if len(unchecked_blocks) == 0:
                run = False

        for block in preapared_blocks:
            block.color = self.pen.color
            block.fill_checked = False

    def make_drawing(self, array):
        drawing = []
        for block in self.drawing_board.blocks:
            drawing.append(block.color)
        array.append(drawing)

    def draw_drawing(self, drawing):
        for index, color in enumerate(drawing):
            self.drawing_board.blocks[index].color = color

    def undo_and_redo(self, array1, array2):
        if len(array1) > 0:
            self.make_drawing(array2)

            self.draw_drawing(array1[-1])

            array1.remove(array1[-1])

    def save(self):
        if not self.file_path:
            window = Tk()
            window.attributes("-topmost", True)
            window.withdraw()

            self.file_path = asksaveasfilename(title="Save File", defaultextension=".stevinocrtanje", filetypes=[('Stevino Crtanje File','*.stevinocrtanje')])

            window.destroy()

        else:
            window = Tk()
            window.attributes("-topmost", True)
            window.withdraw()
            messagebox.showinfo("Success", "File saved successfuly.")
            window.destroy()

        if self.file_path:
            file = open(self.file_path, "w")

            for block in self.drawing_board.blocks:
                for index, color in enumerate(block.color):
                    sufix = ","
                    if index == 2: sufix = ";"
                    file.write(str(color) + sufix)

            file.close()

            self.saved = True
            change_caption(self.file_path.split("/")[-1])

    def ask_for_save(self, string):
        window = Tk()
        window.attributes("-topmost", True)
        window.withdraw()

        if not self.saved:
            if messagebox.askyesno("Save Work?", string):
                self.save()

    def load(self):
        self.ask_for_save("Would you like to save loading a new file?")

        window = Tk()
        window.attributes("-topmost", True)
        window.withdraw()

        self.file_path = askopenfilename(title="Load File", defaultextension=".stevinocrtanje", filetypes=[('Stevino Crtanje File','*.stevinocrtanje')])

        change_caption(self.file_path.split("/")[-1])

        if self.file_path:
            file = open(self.file_path, "r")
            colors = file.read().split(';')

            for index, block in enumerate(self.drawing_board.blocks):
                color_str = colors[index].split(",")
                color = []
                for num in color_str:
                    color.append(int(num))
                block.color = tuple(color)

class DrawingBoard:
    def __init__(self):
        # width and height should be the same as height of the screen
        self.width = HEIGHT
        self.x = 0
        self.y = 0
        self.block_width = 10
        self.clicked_pos = (0,0)
        self.amount = self.width // self.block_width

        self.blocks = []
        for i in range(self.amount):
            for j in range(self.amount):
                self.blocks.append( Block(i*self.block_width, j*self.block_width) )

    def draw(self):
        for block in self.blocks:
            pygame.draw.rect(screen, block.color, (block.x, block.y, self.block_width, self.block_width))

    def dye(self, x, y, color, weight):
        for block in self.blocks:
            if chechk_if_clicked(block.x, block.y, block.x + self.block_width, block.y + self.block_width, x, y):
                block.color = color
                self.clicked_pos = (block.x, block.y)

        shrink = weight - 1
        clicked_x, clicked_y = self.clicked_pos
        min_x, min_y = clicked_x - self.block_width * shrink, clicked_y - self.block_width * shrink
        max_x, max_y = clicked_x + self.block_width * shrink, clicked_y + self.block_width * shrink

        for block in self.blocks:
            if min_x <= block.x <= max_x and min_y <= block.y <= max_y:
                block.color = color

class Block:
    def __init__(self, x, y):
        self.color = WHITE
        self.x = x
        self.y = y
        self.fill_checked = False

class Pen:
    def __init__(self):
        self.color = BLACK
        self.weight = 1

class ToolsBoard:
    def __init__(self):
        self.x = HEIGHT
        self.y = 0
        self.color = GRAY

        # color buttons
        self.palette = [BLACK, WHITE, YELLOW, RED, BLUE, GREEN, PURPLE, ORANGE, BROWN]
        self.palette_width = 150
        self.row_p = 3
        self.width_p = self.palette_width // self.row_p
        self.startx_p = self.x + (WIDTH - self.x - self.palette_width) // 2
        self.starty_p = 30
        self.color_buttons = []
        for index, color in enumerate(self.palette):
            self.color_buttons.append( Button(self.startx_p + (index % self.row_p)*self.width_p, self.starty_p + (index // 3)*self.width_p, self.width_p, self.width_p, color, "", False, 0) )

        # weight buttons
        self.amount_w = 4
        self.width_w = 30
        self.gap_w = (WIDTH - self.x - self.width_w*self.amount_w - (self.startx_p-self.x)*2) // self.amount_w + 2
        self.startx_w = self.startx_p
        self.starty_w = self.starty_p + self.palette_width + 30
        self.weight_buttons = []
        for i in range(self.amount_w):
            self.weight_buttons.append( Button(self.startx_w + (self.gap_w + self.width_w)*i, self.starty_w,self.width_w, self.width_w, BLACK, str(i+1), True, i+1) )

        # fill button and clear button
        self.width_fc = self.width_w * 2 + self.gap_w
        self.height_fc = self.width_w
        self.starty_fc = self.starty_w + self.width_w + 30
        self.startx_f = self.startx_w
        self.startx_c = self.startx_f + self.width_fc + self.gap_w
        self.fill_button = Button(self.startx_f, self.starty_fc, self.width_fc, self.height_fc, BLACK, "Fill", True, 0)
        self.clear_button = Button(self.startx_c, self.starty_fc, self.width_fc, self.height_fc, BLACK, "Clear", True, 0)

        # undo button and redo button
        self.undo_button = Button(self.startx_f, self.starty_fc + self.height_fc + 30, self.width_fc, self.height_fc, DARK_GRAY, "Undo", True, 0)
        self.redo_button = Button(self.startx_c, self.starty_fc + self.height_fc + 30, self.width_fc, self.height_fc, DARK_GRAY, "Redo", True, 0)

        # save button and load button
        self.starty_s = self.starty_fc + self.height_fc + 30 + self.height_fc + 25
        self.save_button = Button(self.startx_f + 30, self.starty_s, 100, 40, BLACK, "Save", True, 0)
        self.load_button = Button(self.startx_f + 30, self.starty_s + 20 + 40, 100, 40, BLACK, "Load", True, 0)

    def draw(self):
        pygame.draw.rect(screen, self.color, (self.x, self.y, WIDTH - self.x, HEIGHT - self.y))

        for btn in self.color_buttons:
            btn.draw()

        for btn in self.weight_buttons:
            btn.draw()

        self.fill_button.draw()
        self.clear_button.draw()
        self.undo_button.draw()
        self.redo_button.draw()
        self.save_button.draw()
        self.load_button.draw()

    def check_colored_and_return(self, mx, my):
        for btn in self.color_buttons:
            if btn.is_clicked(mx, my): return btn.color
        return False

    def set_selected_colored_btn(self, color):
        for btn in self.color_buttons:
            if btn.color == color:
                pygame.draw.circle(screen, GRAY, (btn.x + self.width_p // 2, btn.y + self.width_p // 2), 8)

    def check_wieghted_and_return(self, mx, my):
        for btn in self.weight_buttons:
            if btn.is_clicked(mx, my): return btn.weight
        return False

    def set_selected_weighted_btn(self, weight):
        for btn in self.weight_buttons:
            if btn.weight == weight:
                pygame.draw.rect(screen, RED, (btn.x, btn.y, self.width_w, self.width_w), btn.frame)

    def set_selected_fill_btn(self, fill):
        if fill: self.fill_button.color = RED
        else: self.fill_button.color = BLACK

class Button:
    def __init__(self, x, y, width, height, color, text, has_frame, weight):
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self.color = color
        self.text = text
        self.has_frame = has_frame
        self.weight = weight
        self.frame = 5
        self.font = pygame.font.SysFont("comicsans", 30)

    def is_clicked(self, mx, my):
        return chechk_if_clicked(self.x, self.y, self.x+self.width, self.y+self.height, mx, my)

    def draw(self):
        rendered_text = self.font.render(self.text, 1, self.color)
        pygame.draw.rect(screen, self.color, (self.x, self.y, self.width, self.height), self.frame if self.has_frame else 0)
        screen.blit(rendered_text, (self.x + self.width//2 - rendered_text.get_width()//2, self.y + self.height//2 - rendered_text.get_height()//2))

g = Game()
g.run()