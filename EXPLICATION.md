# Documentation des fichiers du projet `file_system`

Ce fichier décrit la signification et l'utilisation de chaque fichier dans le projet `file_system`.

## Fichiers dans `include/`

### `fs_simple.h`
- **Rôle** : Définit les constantes globales utilisées dans le système de fichiers.
- **Utilité** : Centralise des paramètres importants comme la taille des blocs, le nombre d'inodes et l'identifiant du système de fichiers.

### `superblock.h`
- **Rôle** : Contient les définitions de la structure du superbloc ainsi que les déclarations des fonctions associées (initialisation, lecture, écriture).
- **Utilité** : Gère les informations critiques sur l'état du système de fichiers (nombre total de blocs, inodes, blocs libres, inodes libres, etc.).

### `inode.h`
- **Rôle** : Définit la structure des inodes et déclare les fonctions nécessaires pour leur gestion (création, lecture, écriture).
- **Utilité** : Les inodes stockent les métadonnées des fichiers, comme la taille, les permissions et les pointeurs vers les blocs de données.

### `block.h`
- **Rôle** : Déclare les fonctions de gestion des blocs de données, y compris l'allocation, la lecture et l'écriture des blocs.
- **Utilité** : Gère les blocs de données où le contenu des fichiers est effectivement stocké.

### `bitmap.h`
- **Rôle** : Déclare les fonctions permettant de gérer les bitmaps pour l'allocation et la libération des blocs et des inodes.
- **Utilité** : Les bitmaps sont utilisés pour savoir quels blocs et inodes sont libres ou alloués dans le système de fichiers.

### `constants.h`
- **Rôle** : Définit des constantes supplémentaires utilisées dans le projet, comme la taille maximale des noms de fichiers et le nombre maximal d'entrées dans un répertoire.
- **Utilité** : Centralise les limites et paramètres globaux du système de fichiers, facilitant les modifications.

---

## Fichiers dans `src/`

### `superblock.c`
- **Rôle** : Implémente les fonctions permettant de gérer le superbloc, comme son initialisation, sa lecture et son écriture.
- **Utilité** : Le superbloc contient les informations essentielles du système de fichiers, comme le nombre total de blocs, le nombre d'inodes, et les blocs ou inodes disponibles.

### `inode.c`
- **Rôle** : Implémente les fonctions pour la gestion des inodes, y compris leur création, leur lecture et leur écriture.
- **Utilité** : Les inodes stockent des informations critiques sur les fichiers (taille, permissions) et pointent vers les blocs où les données sont stockées.

### `block.c`
- **Rôle** : Implémente les fonctions permettant de gérer les blocs de données, y compris leur allocation, lecture et écriture.
- **Utilité** : Gère les blocs où sont effectivement stockées les données des fichiers dans le système de fichiers.

### `bitmap.c`
- **Rôle** : Implémente les fonctions pour allouer et libérer des inodes et des blocs à l'aide de bitmaps.
- **Utilité** : Les bitmaps permettent de suivre les blocs et les inodes qui sont alloués ou libres, assurant une gestion efficace des ressources.

### `filesystem.c`
- **Rôle** : Implémente les fonctionnalités globales du système de fichiers, comme le formatage, le montage, et le démontage du système de fichiers.
- **Utilité** : Cette partie permet d'initialiser et de préparer une partition ou une image de disque pour être utilisée avec le système de fichiers.

### `directory.c`
- **Rôle** : Implémente les fonctions permettant de gérer les répertoires, comme la création de répertoires, l'ajout d'entrées, et la recherche de fichiers dans les répertoires.
- **Utilité** : Permet la gestion des répertoires, qui contiennent les entrées pour les fichiers et sous-répertoires dans le système de fichiers.

### `file_ops.c`
- **Rôle** : Implémente les opérations sur les fichiers, y compris la lecture, l'écriture et la suppression.
- **Utilité** : Gère les opérations de base sur les fichiers, en assurant l'accès aux blocs de données par le biais des inodes.

---

## Fichiers dans `tests/`

### `test_superblock.c`
- **Rôle** : Teste l'initialisation, la lecture et l'écriture du superbloc.
- **Utilité** : Vérifie que le superbloc est correctement manipulé et que les informations essentielles du système de fichiers sont gérées correctement.

### `test_inode.c`
- **Rôle** : Teste la création, la lecture et l'écriture des inodes.
- **Utilité** : S'assure que les inodes, qui stockent les métadonnées des fichiers, sont bien manipulés et qu'ils pointent vers les bons blocs de données.

### `test_bitmap.c`
- **Rôle** : Teste l'allocation et la libération des inodes et des blocs à l'aide de bitmaps.
- **Utilité** : Vérifie que les bitmaps gèrent correctement les inodes et les blocs, en marquant ceux qui sont alloués ou disponibles.

### `test_block.c`
- **Rôle** : Teste l'allocation, la lecture et la libération des blocs de données.
- **Utilité** : S'assure que les blocs de données sont correctement alloués, libérés, et que leur contenu peut être lu et écrit sans erreur.

### `test_filesystem.c`
- **Rôle** : Teste les fonctionnalités globales du système de fichiers, comme le formatage et le montage.
- **Utilité** : Vérifie que le système de fichiers peut être formaté, monté et qu'il fonctionne correctement pour stocker et gérer des fichiers.

---

## Fichiers dans `tools/`

### `mkfs_simple.c`
- **Rôle** : Outil pour formater une image de disque avec le système de fichiers.
- **Utilité** : Utilisé pour initialiser un nouveau système de fichiers sur une image de disque, créant le superbloc, les inodes et les bitmaps pour la première fois.

### `fsck_simple.c`
- **Rôle** : Outil pour vérifier l'intégrité d'une image de disque contenant le système de fichiers.
- **Utilité** : Permet de vérifier et de corriger les erreurs potentielles dans le système de fichiers (corruptions, inodes ou blocs mal alloués).

---

