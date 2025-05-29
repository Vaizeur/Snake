# 🐍 Snake en C avec ncurses
Un jeu Snake minimaliste codé en C, fonctionnant dans le terminal grâce à la bibliothèque ncurses.

# 📦 Fonctionnalités
- Déplacement du serpent (ZQSD)
- Génération aléatoire de pommes (A)
- Score affiché en direct
- Collision avec les murs ou soi-même = game over
- Carte dessinée dynamiquement dans le terminal

# 🧰 Prérequis
Système Unix/Linux ou WSL / VM sous Linux

Bibliothèque ncurses installée
```
sudo apt update
sudo apt install libncurses5-dev libncursesw5-dev
```

# 🚀 Compilation
Compilez le fichier avec gcc :
```
gcc -o snake snake.c -lncurses
```
