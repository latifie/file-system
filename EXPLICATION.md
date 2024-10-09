# Documentation des fichiers du projet `file_system`

Ce fichier décrit la signification et l'utilisation de chaque fichier dans le projet `file_system`.

## Fichiers dans `include/`

### `fs_simple.h`
- **Rôle** : Définit les constantes globales utilisées dans le système de fichiers.
- **Utilité** : Centralise des paramètres importants comme la taille des blocs, le nombre d'inodes et l'identifiant du système de fichiers.

### `superblock.h`
- **Rôle** : Contient les définitions de la structure du superbloc ainsi que les déclarations des fonctions associées (initialisation, lecture, écriture).
- **Utilité** : Gère les informations critiques sur l'état du système de fichiers (nombre total de blocs, inodes, etc.).

...

## Fichiers dans `src/`

### `superblock.c`
- **Rôle** : Implémente les fonctions permettant de gérer le superbloc, comme son initialisation, sa lecture et son écriture.
- **Utilité** : Le superbloc est essentiel pour stocker les métadonnées du système de fichiers (nombre de blocs libres, inodes libres, etc.).

...

## Fichiers dans `tests/`

### `test_superblock.c`
- **Rôle** : Teste l'initialisation, la lecture et l'écriture du superbloc.
- **Utilité** : S'assure que le superbloc est correctement manipulé dans le code.

...

## Fichiers dans `tools/`

### `mkfs_simple.c`
- **Rôle** : Outil pour formater une image de disque avec le système de fichiers.
- **Utilité** : Utilisé pour initialiser un nouveau système de fichiers dans un fichier de disque.

...

