import pygame
from pygame import mixer
import random
import math

# Initialize the game
pygame.init()

# creating the screen
width = 800
height = 600
screan = pygame.display.set_mode((width, height))

# Background
background = pygame.image.load("./background2.png")

# Background Sound
bg_sound = mixer.music.load('./background.wav')
mixer.music.play(-1)
pygame.mixer.music.set_volume(0.4)

# Title and Icon
pygame.display.set_caption("Space Invader")
icon = pygame.image.load("./space-invaders.png")
pygame.display.set_icon(icon)

# Player
playerImg = pygame.image.load("./player.png")
playerX = width/2 - 32
playerY = height*(23/30)
playerX_change = 0


def player(x, y):
    screan.blit(playerImg, (x, y))


# Score
score_value = 0
score_font = pygame.font.Font('./space_age.ttf', 32)
textX = 10
textY = 10


def show_score(x, y):
    text = score_font.render('SCORE : ' + str(score_value), True, (225, 225, 225))
    screan.blit(text, (x, y))

# Game Over
over_font = pygame.font.Font('./new_space.ttf', 64)

def game_over():
    game_over_text = over_font.render('GAME OVER', True, (225, 225, 225))
    screan.blit(game_over_text, (150, 220))


# Enemy
enemyImg = []
enemyX = []
enemyY = []
enemyX_change = []
enemyY_change = []
num_enemies = 3

for i in range(num_enemies):
    enemyImg.append(pygame.image.load("./enemy.png"))
    enemyX.append(random.randint(0, 735))
    enemyY.append(random.randint(50, 300))
    enemyX_change.append(5)
    enemyY_change.append(0)


def enemy(x, y, i):
    screan.blit(enemyImg[i], (x, y))


# Bullet
# Ready - bullet is ready but not showing
# Fire - bullet appears on screen

bulletImg = pygame.image.load("./bullet.png")
bulletX = 0
bulletY = 480
bulletX_change = 0
bulletY_change = 25
bullet_state = "ready"


def bullet_fire(x, y):
    global bullet_state
    bullet_state = "fire"
    screan.blit(bulletImg, (x + 16, y + 10))


def isCollision(enemyX, enemyY, bulletX, bulletY):
    distance = math.sqrt((math.pow(enemyX - bulletX, 2) + (math.pow(enemyY - bulletY, 2))))
    if distance < 27:
        return True


# Game Loop
running = True
while running:

    screan.fill((0, 0, 0))
    screan.blit(background, (0, 0))
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_RIGHT:
                playerX_change = 12
            if event.key == pygame.K_LEFT:
                playerX_change = -12
            if event.key == pygame.K_SPACE:
                if bullet_state is "ready":
                    bullet_sound = mixer.Sound('./laser.wav')
                    bullet_sound.play()
                    bulletX = playerX
                    bullet_fire(bulletX, bulletY)

        if event.type == pygame.KEYUP:
            if event.key == pygame.K_RIGHT or event.key == pygame.K_LEFT:
                playerX_change = 0

    playerX += playerX_change
    if playerX <= 0:
        playerX = 0
    if playerX >= 736:
        playerX = 736

    # Enemy Movement
    for i in range(num_enemies):
        if enemyY[i] > 400:
            for j in range(num_enemies):
                enemyY[j] = 2000
            game_over()
            break
        enemyX[i] += enemyX_change[i]
        if enemyX[i] <= 0:
            enemyX_change[i] = 5
            enemyY[i] += 40
        if enemyX[i] >= 736:
            enemyX_change[i] = -5
            enemyY[i] += 40

        # Collision
        collision = isCollision(enemyX[i], enemyY[i], bulletX, bulletY)
        if collision:
            explosion_sound = mixer.Sound('./explosion.wav')
            explosion_sound.play()
            bulletY = 480
            bullet_state = "ready"
            score_value += 1
            enemyX[i] = random.randint(0, 735)
            enemyY[i] = random.randint(50, 400)

        enemy(enemyX[i], enemyY[i], i)

    # Bullet Movement
    if bulletY <= 0:
        bulletY = 480
        bullet_state = "ready"

    if bullet_state is "fire":
        bullet_fire(bulletX, bulletY)
        bulletY -= bulletY_change

    player(playerX, playerY)
    show_score(textX, textY)
    pygame.display.update()
