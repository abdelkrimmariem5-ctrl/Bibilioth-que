=====================================
  Bibliothèque Numérique — README
  SAE 1.01 — BUT Informatique 1ère année
  Auteur : ABDELKERIM Mariem
=====================================

DESCRIPTION
-----------
Application en ligne de commande pour gérer une bibliothèque numérique.
Développée en C++17, elle permet d'ajouter, consulter, rechercher et
exporter des références de livres.


COMPILATION
-----------
Prérequis : g++ avec support C++17

Avec le Makefile (recommandé) :
    make

Sans Makefile :
    g++ -std=c++17 -Wall -o main main.cpp book.cpp library.cpp config.cpp menu.cpp

Pour nettoyer les fichiers compilés :
    make clean


UTILISATION
-----------
Lancer l'application :
    ./main

Au premier lancement, l'application vous demande de créer une bibliothèque
(titre + description). Ensuite, le menu principal s'affiche.

Menu principal :
    1. Consulter les références    — Liste paginée de tous les livres
    2. Gérer les références        — Ajouter, importer CSV, supprimer
    3. Chercher une référence      — Par ISBN, éditeur ou titre
    4. Exporter en HTML            — Génère bibliotheque.html
    5. Paramètres                  — Titre, affichage, logo ASCII
    6. Quitter


FICHIERS GÉNÉRÉS
----------------
    library.db       — Base de données des livres (format texte)
    app.conf         — Configuration (items par page, logo ASCII)
    bibliotheque.html — Export HTML généré par l'option 4


FORMAT CSV (Import)
-------------------
Le fichier CSV doit avoir un en-tête sur la première ligne et les
champs séparés par des virgules dans cet ordre :

    isbn,titre,auteurs,langue,date,genre,description

Exemple :
    978-2-07-289509-8,Le Petit Prince,Antoine de Saint-Exupéry,Français,1943,Roman,Un aviateur...

Un fichier d'exemple est fourni : tests/livres_test.csv


FORMAT ISBN-13
--------------
L'ISBN-13 doit être saisi avec des tirets entre les parties :
    Format : EAN-groupe-editeur-publication-cle
    Exemple : 978-2-07-289509-8

L'application vérifie automatiquement la validité du chiffre de contrôle.


JEUX DE TEST
------------
Les tests utilisent la commande `expect` disponible sur les VDI Linux.

Lancer tous les tests :
    expect tests/test.exp

Le script teste automatiquement :
    - Création d'une nouvelle bibliothèque
    - Import d'un fichier CSV
    - Consultation de la liste
    - Recherche par titre
    - Export HTML
    - Quitter proprement

Fichiers de test fournis :
    tests/test.exp        — Script expect de test automatique
    tests/livres_test.csv — Jeu de données CSV avec 5 livres


STRUCTURE DU CODE
-----------------
    main.cpp      — Point d'entrée, menu principal, initialisation
    book.hpp/cpp  — Structure Book, validation ISBN, code éditeur
    library.hpp/cpp — Structure Library, chargement/sauvegarde,
                      import CSV, export HTML, recherches
    config.hpp/cpp  — Structure Config, lecture/écriture app.conf
    menu.hpp/cpp    — Tous les menus et l'interface utilisateur
    Makefile        — Compilation automatique
