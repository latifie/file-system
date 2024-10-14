# file-system

## Arborescence de notre Système de fichier 
```
file_system/
├── include/                          # Dossier pour les fichiers d'en-tête
│   ├── fs_simple.h                   
│   ├── superblock.h                  
│   ├── inode.h                       
│   ├── block.h                       
│   ├── bitmap.h                      
│   └── constants.h                   # Constantes supplémentaires (ex : tailles de fichiers)
│
├── src/                              
│   ├── superblock.c                  
│   ├── inode.c                       
│   ├── block.c                       
│   ├── bitmap.c                      
│   ├── filesystem.c                  # Fonctions pour le formatage et montage du FS
│   ├── directory.c                   # Fonctions pour la gestion des répertoires
│   └── file_ops.c                    # Fonctions pour les opérations sur les fichiers
│
├── tests/                            # Dossier pour les tests unitaires
│   ├── test_superblock.c             
│   ├── test_inode.c                  
│   ├── test_bitmap.c                 
│   ├── test_block.c                  
│   └── test_filesystem.c             # Teste les opérations globales du FS (formatage, montage)
│
├── tools/                            # Dossier pour les outils utilitaires
│   ├── mkfs_simple.c                 
│   └── fsck_simple.c                 
│
├── EXPLICATION.md                    
├── .gitignore                        # Fichier gitignore pour exclure certains fichiers
└── README.md                         # Fichier README pour décrire le projet
```


## EXT2 est basé sur plusieurs concepts clés :

Superblock : Contient des informations essentielles sur le système de fichiers, comme la taille, le nombre de blocs, le nombre d'inodes, etc.

Inodes : Chaque fichier et répertoire est représenté par un inode. Un inode contient des informations sur le fichier (taille, permissions, timestamp, etc.) ainsi que des pointeurs vers les blocs de données.

Blocs de données : Ce sont les unités de stockage où les données des fichiers sont réellement enregistrées.

Groupes de blocs : Le système EXT2 divise le disque en plusieurs groupes de blocs, chacun contenant des inodes et des blocs de données.

Bitmap des inodes et des blocs : Des bitmaps sont utilisés pour suivre quels inodes et quels blocs sont utilisés ou libres.


## 2. Étapes pour implémenter le simulateur

a. Initialisation du système de fichiers
Crée un disque simulé (fichier binaire par exemple) où tu stockeras tes données.
Simule la création d'un superblock qui contiendra les informations de base de ton système de fichiers (nombre de blocs, nombre d'inodes, taille des blocs, etc.).

b. Implémenter la structure des inodes
Créer une structure qui simule un inode, contenant des informations comme :
Taille du fichier.
Permissions.
Pointeurs vers les blocs de données.
Timestamp de création, modification, etc.
Les inodes sont stockés dans un tableau ou une structure de gestion des inodes.

c. Gestion des blocs de données
Implémente un mécanisme pour allouer et libérer des blocs de données sur ton disque simulé.
Utiliser des bitmaps pour suivre quels blocs sont utilisés ou disponibles.

d. Système de fichiers hiérarchique
Implémenter des répertoires comme des fichiers spéciaux. Chaque répertoire contient une table de noms de fichiers et les numéros d'inode correspondants.
Créer des fonctions pour naviguer entre répertoires, créer/supprimer des fichiers et répertoires.

e. Lecture et écriture de fichiers
Crée des fonctions pour lire et écrire des données dans les fichiers.
Utilise les inodes pour trouver les blocs de données correspondants et gérer la lecture/écriture des données.

f. Gestion des métadonnées
Implémenter des fonctions pour gérer les métadonnées des fichiers comme les permissions, la taille et les timestamps.

```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INODES 100  // Nombre maximum d'inodes dans le système de fichiers
#define MAX_BLOCKS 500  // Nombre maximum de blocs de données

// Structure décrivant le superblock du système de fichiers, contenant les informations essentielles
typedef struct {
    int s_inodes_count;        // Total des inodes dans le système de fichiers
    int s_blocks_count;        // Total des blocs dans le système de fichiers
    int s_free_inodes_count;   // Nombre d'inodes libres
    int s_free_blocks_count;   // Nombre de blocs libres
} SuperBlock;

// Structure décrivant un inode dans le système de fichiers
typedef struct {
    int i_mode;     // Mode de l'inode (0 = libre, 1 = fichier, 2 = répertoire)
    int i_size;     // Taille du fichier ou répertoire que l'inode représente
    int i_blocks[10];  // Blocs de données attribués à l'inode (simplifié à 10 blocs maximum)
} Inode;

// Structure d'une entrée de répertoire
typedef struct {
    int inode;          // Numéro d'inode de l'entrée de répertoire
    char name[256];     // Nom de l'entrée
} DirEntry;

Inode inodes[MAX_INODES];   // Tableau des inodes
int data_blocks[MAX_BLOCKS]; // Tableau simulant les blocs de données (1 si utilisé, 0 sinon)
SuperBlock superblock;      // Instance du superblock

// Initialise le système de fichiers en configurant les structures de données de base
void init_fs() {
    superblock.s_inodes_count = MAX_INODES;
    superblock.s_blocks_count = MAX_BLOCKS;
    superblock.s_free_inodes_count = MAX_INODES;
    superblock.s_free_blocks_count = MAX_BLOCKS;
    memset(inodes, 0, sizeof(inodes));
    memset(data_blocks, 0, sizeof(data_blocks));
}

// Alloue un inode libre et retourne son ID, ou -1 si aucun inode libre
int allocate_inode() {
    for (int i = 0; i < MAX_INODES; i++) {
        if (inodes[i].i_mode == 0) {
            inodes[i].i_mode = 1; // Marquer comme utilisé pour un fichier
            superblock.s_free_inodes_count--;
            return i;
        }
    }
    return -1; // Aucun inode libre
}

// Libère un inode spécifié par inode_id
void free_inode(int inode_id) {
    if (inodes[inode_id].i_mode != 0) {
        memset(&inodes[inode_id], 0, sizeof(Inode)); // Réinitialiser l'inode à zéro
        superblock.s_free_inodes_count++;
    }
}

int main() {
    init_fs(); // Initialiser le système de fichiers
    int inode_id = allocate_inode(); // Allouer un inode
    if (inode_id != -1) {
        printf("Inode %d alloué\n", inode_id);
    }
    free_inode(inode_id); // Libérer l'inode
    printf("Inode %d libéré\n", inode_id);
    return 0;
} ```


Ajouter : 
regarder ext2 et source 
tests (de façon unitaire)
modularité 

la table de matiere les points principale 
