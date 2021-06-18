import pygame

# inition
pygame.init()
WIDTH, HEIGHT = 400, 400
screeen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Stevino crtanje")

# main loop
circles = []
run = True
clock = pygame.time.Clock()
FPS = 80
draw = False
while run:
    clock.tick(FPS)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False

        if event.type == pygame.MOUSEBUTTONDOWN:
            circles.append(pygame.mouse.get_pos())
            draw = True

        if event.type == pygame.MOUSEBUTTONUP:
            draw = False

        if draw and event.type == pygame.MOUSEMOTION:
            circles.append(pygame.mouse.get_pos())

    screeen.fill((255,255,255))
    for pos in circles:
        pygame.draw.circle(screeen, (0,0,0), pos, 10)

    pygame.display.update()

pygame.quit()