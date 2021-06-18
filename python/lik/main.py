import pygame
pygame.init()

#images
standimg = ['img/sprite00.png', 'img/sprite01.png', 'img/sprite02.png', 'img/sprite03.png', 'img/sprite04.png']
rightimg = ['img/sprite05.png', 'img/sprite06.png', 'img/sprite07.png', 'img/sprite08.png', 'img/sprite09.png']
leftimg = ['img/sprite10.png', 'img/sprite11.png', 'img/sprite12.png', 'img/sprite13.png', 'img/sprite14.png']

sw = 640
sh = 640

screen = pygame.display.set_mode((sw, sh))
pygame.display.set_caption("Stevina Igrica")

clock = pygame.time.Clock()

class Char:
    def __init__(self):
        self.w_h = 64
        self.vel = 5
        self.margin = 5
        self.x = sw // 2 - self.w_h // 2
        self.y = sh - self.w_h - self.margin
        self.moveIndex = 0
        self.reverseImg = False
        self.activeImgArr = standimg
        self.fixedJumpIndex = 15
        self.jumpIndex = self.fixedJumpIndex
        self.isJump = False

    def move(self, left, right, stand):
        if left and self.x - self.vel >= self.margin:
            self.x -= self.vel
            self.activeImgArr = leftimg
        elif right and self.x + self.vel <= sw - self.w_h - self.margin:
            self.x += self.vel
            self.activeImgArr = rightimg
        elif stand:
            self.activeImgArr = standimg

        if self.moveIndex == 4:
            self.reverseImg = True
        elif self.moveIndex == 0:
            self.reverseImg = False

        if self.reverseImg:
            self.moveIndex -= 1
        else:
            self.moveIndex += 1

    def check_for_max(self):
        if self.x == 8 or self.x == 573:
            return False
        else:
            return True

    def jump(self):
        if self.isJump:
            self.y -= self.jumpIndex
            self.jumpIndex -= 1
            if self.jumpIndex == self.fixedJumpIndex * -1 -1:
                self.isJump = False
                self.jumpIndex = self.fixedJumpIndex



    def draw_and_update(self):
        screen.fill((0, 0, 0))
        screen.blit(pygame.image.load(self.activeImgArr[self.moveIndex]), (self.x, self.y))
        pygame.display.update()

char = Char()
run = True
while run:
    clock.tick(20)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False

    keys = pygame.key.get_pressed()

    if keys[pygame.K_LEFT]:
        char.move(True, False, False)
    elif keys[pygame.K_RIGHT]:
        char.move(False, True, False)
    else:
        char.move(False, False, True)

    if not char.isJump:
        if keys[pygame.K_UP]:
            char.isJump = True
    char.jump()

    if char.check_for_max():
        char.draw_and_update()

pygame.quit()
