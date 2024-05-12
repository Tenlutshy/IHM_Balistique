# IHM Balistique

## 📌 Sommaire
1. [Projet](#💻-projet)
    - [Présentation](#présentation)
    - [Fonctionnalités](#fonctionnalités)
    - [Interface](#interface)
    - [Simulation](#simulation)

## 🎯 Badges

[![Moteur Qt](https://img.shields.io/badge/Moteur-Qt-green.svg)]()
[![Moteur Unity](https://img.shields.io/badge/Moteur-Unity-green.svg)]()
[![Langage C++](https://img.shields.io/badge/Langage-C++-blue.svg)]()
[![Langage C#](https://img.shields.io/badge/Langage-CSharp-blue.svg)]()
[![Langage SQLite](https://img.shields.io/badge/Langage-SQLite-blue.svg)]()
[![Protocole C#](https://img.shields.io/badge/Protocole-TCP/IP-red.svg)]()

## 💻 Projet

### Présentation

Ce projet est une interface homme machine permettant de prévoir les coordonnées de l'impact d'un tir balistique dans un environnement influencé par la gavité terrestre et le vent. Ce tir peut ensuite être simulé dans une scène simulant le condition du tir.

### Fonctionnalités

| Logiciel | Fonctionnalité | Description |
|:--------------|:-------------|:--------------|
| Interface | **Tap-to-Shot** | *Clique sur une zone de l'interface pour adapter automatiquement la configuration du canon afin d'atteindre la cible en fonction de l'environnement.* |
| Interface | **Configuration manuel** | *Usage de différent mode d'entrées pour la personnalisation des configurations d'environnement, du canon et du projectile.* |
| Interface | **Sauvegarde des impacts** | *Enregistrement des configurations d'environnement et du canon à chaque impacts afin de pouvoir les réutiliser.* |
| Interface | **Prédiction d'impact** | *Prévisualisation de la zone d'impact à l'aide d'un algorithme de prédiction.* |
| Interface | **Log des interractions** | *Les interractions avec l'interface sont loggé dans un fichier .log.* |
| Interface | **Fichier configue** | *Possibilité de configurer le port d'écoute TCP/IP depuis un fichier .ini.* |
| Interface & Simulateur | **Communication TCP/IP** | *Communication entre l'interface et le simulateur à l'aide d'un protocole TCP/IP et un encodage maison permettant le transfert de la configuration du canon, de l'environnement et du projectile.* |
| Simulateur | **Modification des paramètres de simulation** | *Application des configurations reçu de l'interface afin de simulé le tir de manière réaliste.* |
| Simulateur | **Affichage des impacts** | *Affichage des impacts dans l'environnement afin de simplifier la visualisation.* |
| Simulateur | **Fichier configue** | *Possibilité de configurer le port d'écoute TCP/IP depuis un fichier .json.* |


### Interface

L'interface est un logiciel réalisé à l'aide de Qt C++. Elle permet à l'utilisatuer de ce connecter à un simulateur d'environnement à l'aide du protocole TCP/IP ainsi que de réaliser des prévisions de tir par rapport à l'environnement et au configurations du canon. De plus l'utilisateur à la possibilité de réutiliser les configurations du canon permettant d'atteindre un impact déjà simulé en le séléctionnant.

[Installation et utilisation](./IHM/README.md)

### Simulation

Le simulateur est une scène Unity qui communique avec l'interface à l'ai du protocole TCP/IP. Celui-ci va permettre de simuler un tir balistique dans l'environnement configuré sur l'interface et de visualiser les différents points d'impact réalisé durant la session.

[Installation et utilisation](./CanonSimulation/README.md)
