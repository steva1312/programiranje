import pygame
import math

# setting the word
LETTERS = ["A", "B", "C", "Č", "Ć", "D", "Dž", "Đ", "E", "F", "G", "H", "I", "J", "K", "L", "Lj", "M", "N", "Nj", "O", "P", "R", "S", "Š", "T", "U", "V", "Z", "Ž"]
word = ""
guessed = ""
MAX_CHARS = 10
def word_check():
    global word
    word = str.upper(input("Unesi reč:"))
    msg = []
    fine = False
    fine_letters = 0
    for ltr in word:
        for LTR in LETTERS:
            if ltr == LTR: fine_letters += 1

    if not fine_letters == len(word): msg.append("Nevažeća slova.")
    if len(word) > MAX_CHARS: msg.append("Reč mora da bude kraća od " + str(MAX_CHARS) + " slova.")
    if len(msg) == 0: fine = True
    for message in msg:
        print(message)

    return fine

correct = False
while not correct:
    correct = word_check()
for i in range(len(word)):
    guessed += '_'

# initialising
pygame.init()
WIDTH,HEIGHT = 800,600
screen = pygame.display.set_mode((WIDTH,HEIGHT))
pygame.display.set_caption("Vešala")

# colors
BLACK = (0,0,0)
WHITE = (255,255,255)
RED = (255,0,0)

# fonts
BTN_FONT = pygame.font.SysFont("comicsans", 35)
WORD_FONT = pygame.font.SysFont("comicsans", 50)
END_FONT = pygame.font.SysFont("arial", 60)

# load images
IMAGE_COUNT = 7
images = []
for i in range(IMAGE_COUNT):
    images.append(pygame.image.load("images/hangman" + str(i) + ".png"))
img = pygame.image.load("images/hangman0.png")
hangman_status = 0
IMAGE_POS = (75,75)

# buttons
letters = []
l_num = len(LETTERS)
MARGIN, RADIUS, ROWS = 50, 20, 2
GAP = (WIDTH - MARGIN*2 - RADIUS*2*(l_num//ROWS)) // (l_num // ROWS - 1)
START_Y = HEIGHT - GAP*(ROWS-1) - RADIUS*2*ROWS - MARGIN
for i in range(l_num):
    bx = MARGIN + RADIUS + (GAP + RADIUS * 2) * (i % (l_num // ROWS))
    by = START_Y + RADIUS + (GAP + RADIUS * 2) * (i // (l_num // ROWS))
    letters.append( [bx, by, LETTERS[i], False] ) # [x, y, letter, clicked]

# main loop
gameover = False
winner = ""
guessed_letters = 0
def draw():
    screen.fill((255,255,255))

    screen.blit(images[hangman_status], IMAGE_POS)

    mx, my = pygame.mouse.get_pos()
    for letter in letters:
        lx, ly, ltr, clicked = letter
        if not clicked:
            color = BLACK
            distance = math.sqrt( (mx-lx)**2 + (my-ly)**2 )
            if distance <= RADIUS: color = RED
            pygame.draw.circle(screen, color, (lx, ly), RADIUS, 3)
            text = BTN_FONT.render(ltr, 1, color)
            screen.blit(text, (lx - text.get_width()//2, ly - text.get_height()//2))

    display_word = ""
    for char in guessed:
        display_word += char + " "
    text = WORD_FONT.render(display_word, 1, BLACK)
    screen.blit(text, (400,175))

    if gameover:
        end_text = END_FONT.render(winner + " je pobedio!", 1, RED)
        screen.blit(end_text, (280, 50))

    pygame.display.update()

FPS = 60
clock = pygame.time.Clock()
run = True
while run:
    clock.tick(FPS)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False

        if not gameover:
            if event.type == pygame.MOUSEBUTTONDOWN:
                x, y = pygame.mouse.get_pos()

                for letr in letters:
                    l_x, l_y, l, clickedd = letr
                    dist = math.sqrt( (x-l_x)**2 + (y-l_y)**2 )

                    if dist <= RADIUS:
                        letter_positions = []
                        for i in range(len(word)):
                            if word[i] == l: letter_positions.append(i)

                        for pos in letter_positions:
                            guessed = list(guessed)
                            guessed[pos] = l
                            guessed = ''.join(guessed)
                            guessed_letters += 1

                        if guessed_letters == len(word):
                            gameover = True
                            winner = "Mislilac"

                        if len(letter_positions) == 0:
                            hangman_status += 1
                            if hangman_status == IMAGE_COUNT - 1:
                                gameover = True
                                winner = "Organizator"
                                guessed = word
                        letr[3] = True

    draw()

pygame.quit()