import pygame
pygame.init()

sw = 500
sh = 500
screen = pygame.display.set_mode((sw, sh))
pygame.display.set_caption("Stevina Igrica")

def display_text(size, text, x, y):
    font = pygame.font.Font('freesansbold.ttf', size)
    text = font.render(text, True, (255, 255, 255))
    text_rect = text.get_rect()
    text_rect.center = (x, y)
    screen.blit(text, text_rect)

player_width = 50
speed = 5
bullets = []
class Player:
    def __init__(self, y, color, k_left, k_right, k_shoot, k_teleport,drctn):
        self.x = sw // 2 - player_width // 2
        self.y = y - player_width * 2
        self.color = color
        self.k_left = k_left
        self.k_right = k_right
        self.k_shoot = k_shoot
        self.drctn = drctn
        self.margin = 5
        self.shootOn = 200
        self.secToShoot = self.shootOn
        self.countToShoot = False
        self.fullMagazine = 7
        self.currentMagazine = self.fullMagazine
        self.reloadOn = 1500
        self.secToReload = self.reloadOn
        self.countToReload = False
        self.lost = False
        self.way = 0
        self.k_teleport = k_teleport
        self.teleportLength = 150
        self.countToTeleport = False
        self.teleportOn = 8000
        self.secToTeleport = self.teleportOn

    def draw(self):
        pygame.draw.rect(screen, self.color, (self.x, self.y, 50, 50))

    def move(self, _keys):
        if _keys[self.k_left]:
            if self.x - self.margin <= 0: return
            self.x -= speed
        if _keys[self.k_right]:
            if self.x + player_width + self.margin >= sw: return
            self.x += speed
        if _keys[self.k_shoot]:
            self.shoot()
        if _keys[self.k_teleport]:
            self.teleport()

        if _keys[self.k_left] and _keys[self.k_right]:
            self.way = 0
        elif _keys[self.k_left]:
            self.way = -1
        elif _keys[self.k_right]:
            self.way = 1
        else:
            self.way = 0

    def track(self, _keys, _ms):
        if self.lost: return
        self.move(_keys)
        self.draw()
        self.draw_ammo()

        if self.countToShoot:
            self.secToShoot += _ms
            if self.secToShoot == self.shootOn:
                self.countToShoot = False

        if self.countToReload:
            self.secToReload += _ms
            if self.secToReload == self.reloadOn:
                self.countToReload = False
                self.currentMagazine = self.fullMagazine

        teleport_color = (124, 125, 217)
        if self.countToTeleport:
            self.secToTeleport += _ms
            if self.secToTeleport == self.teleportOn:
                self.countToTeleport  = False
                teleport_color = (124, 125, 217)
            else:
                teleport_color = (124, 255, 200)

        ah = 15
        y = self.y + (-1 * self.drctn * player_width) + (player_width - ah) // 2
        pygame.draw.rect(screen, teleport_color, (350, y, self.secToTeleport // (self.teleportOn // 100), ah))

    def shoot(self):
        if self.secToShoot == self.shootOn and self.currentMagazine > 0:
            bullets.append(Bullet(self.x + player_width // 2, self.y + player_width // 2, self.color, self.drctn))
            self.secToShoot = 0
            self.countToShoot = True
            self.currentMagazine -= 1
            if self.currentMagazine == 0:
                self.countToReload = True
                self.secToReload = 0

    def draw_ammo(self):
        green = (0, 255, 0)
        aw = 15
        y = self.y + (-1 * self.drctn * player_width) + (player_width - aw ) // 2
        for i in range(self.currentMagazine):
            pygame.draw.rect(screen, green, (self.margin + i * aw * 2, y, aw, aw))

    def get_cords(self):
        return [self.x, self.y, self.drctn]

    def endgame(self, idx):
        if not idx == self.drctn:
            self.lost = True

    def again(self):
        self.lost = False
        self.currentMagazine = self.fullMagazine
        self.x = sw // 2 - player_width // 2
        self.secToTeleport = self.teleportOn
        self.countToTeleport = False
        bullets.clear()

    def teleport(self):
        if not self.countToTeleport and not self.way == 0:
            self.x += self.teleportLength * self.way
            if self.x < self.margin:
                self.x = self.margin
            elif self.x > sw - self.margin - player_width:
                self.x = sw - self.margin - player_width
            self.countToTeleport = True
            self.secToTeleport = 0


class Bullet:
    def __init__(self, x, y, color, drctn):
        self.x = x
        self.y = y
        self.color = color
        self.drctn = drctn
        self.speed = 10 * self.drctn
        self.target_x = 0
        self.target_y = 0
        self.radius = 10
        self.gameover = False

    def draw(self):
        pygame.draw.circle(screen, self.color, (self.x, self.y), self.radius)

    def move(self):
        self.y += self.speed
        if self.y > sh + self.radius or self.y < 0 - self.radius:
            bullets.remove(self)

    def set_target(self, arr):
        if self.drctn == arr[2] * -1:
            self.target_x = arr[0]
            self.target_y = arr[1]

    def win_check(self):
        if self.gameover == False \
        and (self.target_y < self.y + self.radius < self.target_y + player_width
        or self.target_y < self.y - self.radius < self.target_y + player_width) \
        and (self.target_x < self.x + self.radius < self.target_x + player_width
        or self.target_x < self.x - self.radius < self.target_x + player_width):
            return True


player1 = Player(player_width * 3, (0, 0, 255), pygame.K_a, pygame.K_d, pygame.K_w, pygame.K_s, 1)
player2 = Player(sh, (255, 0, 0), pygame.K_LEFT, pygame.K_RIGHT, pygame.K_UP, pygame.K_DOWN, -1)
ms = 50
gameover = False
run = True
while run:
    pygame.time.delay(ms)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False

    keys = pygame.key.get_pressed()

    screen.fill((0, 0, 0))

    if gameover:
        display_text(50, "GAMEOVER", sw // 2, sh // 2 - 50)
        display_text(20, "Press ENTER to play again!", sw // 2, sh // 2 - 10)

    for bullet in bullets:
        bullet.draw()
        bullet.move()
        bullet.set_target(player1.get_cords())
        bullet.set_target(player2.get_cords())
        if bullet.win_check():
            gameover = True
            player1.endgame(bullet.drctn)
            player2.endgame(bullet.drctn)

    player1.track(keys, ms)
    player2.track(keys, ms)

    pygame.display.update()

    if keys[pygame.K_RETURN] and gameover:
        player1.again()
        player2.again()
        gameover = False

pygame.quit()