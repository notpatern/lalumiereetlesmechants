# Mauvais caractères

Un shoot 'em up vertical en ASCII dans la console Windows. 15 vagues, et de l'XP à ramasser pour améliorer son tir.

Déplacement : flèches, ZQSD ou WASD. Tir : Espace ou E. Pause : Échap. Menus : haut, bas, puis Espace ou Entrée.

Page itch.io : https://paul-hugy.itch.io/mauvais-caracteres

Paul : programmation gameplay. Sasha : programmation moteur et rendu.

## Compiler et lancer

Visual Studio 2022 (toolset v143, C++20), x64, F5. La console fait 80 x 30 : 28 lignes de jeu, 2 de HUD. Prévu pour la console Windows classique. En Debug, F2 passe à la vague suivante.

## Architecture

```
src/
  program.cpp      main
  Utilities/       outils génériques : Pool<T>, Countdown, ScreenShake, Vector2, Timer
  App/
    App.h/.cpp     la boucle et l'écran courant
    Screens/       les écrans (titre, partie, crédits), Menu, Hud
    Manager/       GameManager (une partie), InputManager (clavier)
    Player/        le vaisseau
    Entities/      Entity, PooledEntity, Enemy, Missile, XpOrb, Particle
    Gameplay/      fiches de données, Movement, Weapon, vagues, collisions, XP
    Render/        Renderer, Sprite, les dessins ASCII
    World/         la carte du fond
```

Une frame :

```
App          dt, clavier, Update de l'écran, changement d'écran si demandé, rendu, Sleep(16)
GameManager  joueur, vagues, ennemis, missiles, orbes, collisions, ramassage de l'XP
             gelé pendant un hit-stop, sauf le tremblement et les particules
Renderer     fond, sprites, textes des écrans, un seul WriteConsoleOutputA
```

`App` possède l'écran courant. `GameScreen` possède le `GameManager`, qui possède les pools, le joueur et les vagues. Entités et écrans s'inscrivent eux-mêmes dans les files du `Renderer`, le seul singleton.

## Choix techniques

- `Pool<T>` : aucune allocation pendant la partie, un plafond, et des objets rangés par paquets pour que leur adresse ne bouge jamais.
- Une pool de missiles par camp : un tir ne peut pas toucher son camp.
- Une seule classe `Enemy`, réglée par une fiche de données (`EnemyArchetype`). Pareil pour les missiles, les niveaux du joueur et les vagues.
- Un ennemi, c'est un `Movement` et une `Weapon`.
- Les vagues comptent leurs ennemis vivants grâce à un observer.
- Collisions en cases, avec Bresenham entre deux positions pour qu'un missile rapide ne traverse rien.
- Hit-stop et tremblement sont des données. Le tremblement n'est qu'un décalage de caméra.
- Convention : la portée dans le nom (`m_`, `s_`, `in` et `out`, constantes en majuscules), pas de déclaration anticipée, concepts C++20 (`Poolable`, `Hittable`).

## Défauts connus

- Missiles contre ennemis et séparation de la nuée en n². Ça suffit pour une soixantaine d'ennemis.
- Pas de pas de temps fixe : le dt varie, borné à 0,1 s.
- Pas de pause quand la fenêtre perd le focus.
- Pas de couches de rendu : on dessine dans l'ordre de la file.
- Les longs textes des écrans allouent encore à chaque frame.
- Pas de son.
