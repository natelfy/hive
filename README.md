# 🐝 Hive – Jeu de stratégie avec Intelligence Artificielle (Projet UTC – LO21 A24)

Ce projet a été réalisé en groupe dans le cadre de l’UV LO21 (Automne 2024) à l’Université de Technologie de Compiègne.  
Nous avons développé une application en **C++** qui implémente le jeu de société Hive, jouable en console, avec **trois niveaux d’IA** intégrés, selon les algorithmes **MinMax** et **Monte Carlo Tree Search (MCTS)**.

---

## 🧠 Objectifs du projet

- Implémenter le jeu **Hive** selon ses règles officielles
- Permettre une jouabilité **à deux joueurs humains ou contre l’IA**
- Proposer plusieurs niveaux d’intelligence artificielle
- Assurer une **architecture orientée objet** claire et évolutive
- Gérer les extensions officielles (Moustique, Coccinelle, Cloporte)

---

## 🎮 Fonctionnalités principales

- ♟️ Moteur de jeu Hive complet (règles, placement, déplacement, victoire)
- 🔄 Mode 2 joueurs humains OU Humain vs IA
- 🤖 3 niveaux d’IA :
  - *Facile* : coups aléatoires
  - *Moyen* : **MinMax** avec évaluation statique
  - *Difficile* : **Monte Carlo Tree Search**
- ↩️ Système de retours en arrière (annulation tour)
- 🧩 Extensions activables : **Moustique**, **Coccinelle**, **Cloporte**
- 🧪 Tests de validation du moteur du jeu
- 🎥 Vidéo de démonstration disponible (voir ci-dessous)

---

## 🛠️ Stack technique

- 🧪 Langage principal : **C++17**
- 🧱 Paradigme : **Programmation orientée objet**
- 📚 Bibliothèques : STL uniquement (version console)
- 🔧 Interface : **mode console** (version Qt prévue)
- 🧪 Tests unitaires simples

---

## 🧩 Architecture logicielle (résumé)

Le projet repose sur une architecture modulaire :
- **`Plateau`** : gestion des hexagones et positions
- **`Insecte` + héritiers** : règles propres à chaque type d’insecte
- **`Joueur`** (humain ou IA)
- **`IA` + dérivés** : IA_Aléatoire, IA_MinMax, IA_MCTS
- **`Partie`** : cœur du moteur de jeu (contrôle du déroulement)

L’ensemble des classes est conçu pour faciliter :
- l’ajout d’extensions (nouveaux insectes)
- l’ajout de nouvelles IA
- l’intégration future d’une **interface graphique Qt**