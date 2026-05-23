# ⚔️ The Royal Chronicle

> **Data Structures & Algorithms **  
> Binary Search Tree

---

## 📋 Table of Contents

- [Overview](#overview)
- [Project Structure](#project-structure)
- [Problem Description](#problem-description)
- [Data Structure](#data-structure)
- [Features](#features)
- [Algorithm Details](#algorithm-details)
- [Input Format](#input-format)
- [Output Format](#output-format)
- [Example](#example)
- [How to Compile & Run](#how-to-compile--run)
- [Edge Cases Handled](#edge-cases-handled)

---

## Overview

The Kingdom has waged a century of wars, and the Royal Archives overflow with unsorted scrolls. The Royal Scribe is ordered to organize the **Knight Registry** — a record of all knights who have sworn allegiance to the crown — using a **Binary Search Tree (BST)** ordered by each knight's **Bravery Score**: a unique integer awarded by the King for acts in combat.

---

## Project Structure

```
ex2/
├── ex2.cpp      # Full BST implementation + all tasks in main()
└── README.md    # This file
```

---

## Problem Description

### The Knight Record

Each BST node stores a complete knight dossier:

| Field | Type | Role |
|-------|------|------|
| `name` | `string` | Unique first name — used for name-based search |
| `braveryScore` | `int` | **BST key** — unique integer, determines position in tree |
| `house` | `string` | Noble house the knight serves |
| `battlesWon` | `int` | Total battles won in the King's name |

Knights are inserted **in the order they appear** in the royal register, using `braveryScore` as the BST key.

---

## Data Structure

### `BinarySearchTree<T>` (generic BST node)

Each node **is itself a tree**: it holds a pointer to its data, pointers to children and parent, and a pointer to the root of the whole tree.

```cpp
template<typename T>
class BinarySearchTree {
public:
    BinarySearchTree<T>* root;       // pointer to the root of the whole tree
    BinarySearchTree<T>* left_son;   // left child  (braveryScore <= current)
    BinarySearchTree<T>* right_son;  // right child (braveryScore >  current)
    BinarySearchTree<T>* parent;     // parent node (NULL if root)
    T* pinfo;                        // pointer to stored data (NULL if empty)
};
```

Instantiated as:

```cpp
BinarySearchTree<Knight>* bst = new BinarySearchTree<Knight>();
```

### BST Ordering Property

```
left subtree keys  ≤  current node key  <  right subtree keys
       (braveryScore)                        (braveryScore)
```

---

## Features

### Task 1 — BST Drawing *(manual — place image in project folder)*
Insert all knights in input order using `braveryScore` as the key. Draw the resulting tree showing for each node: name, bravery score, house, and battles won.

### Task 2 — Three Ceremonial Traversals
Print each knight's name and bravery score in three orders (5 knights per line):

| Traversal | Order | Ceremonial Purpose |
|-----------|-------|--------------------|
| **In-order** | Left → Root → Right | Tournament Ranking Scroll (ascending bravery) |
| **Pre-order** | Root → Left → Right | Royal Proclamation Order (read from the throne) |
| **Post-order** | Left → Right → Root | Tribute Collection Order (who pays taxes first) |

### Task 3 — Knights of a Given House
Prints all knights belonging to the queried house in **ascending bravery order** (in-order traversal filtered by house). Additionally displays:
- **Newest Recruit** — knight with the lowest bravery score (leftmost node)
- **Kingdom Champion** — knight with the highest bravery score (rightmost node)

### Task 4 — Structural Archive Report
Computes and displays:
- **BST Height** — depth of the deepest node
- **Leaf Nodes** — knights with no subordinates in the tree structure
- **Balance Check** — whether the tree satisfies the AVL height-balance condition (`|leftH − rightH| ≤ 1` at every node)

### Task 5 — Range Query `[L, R]`
Lists all knights whose `braveryScore` falls within `[L, R]` in ascending order, along with their house and battles won. Prints the **total battles won** by all knights in the range. Prunes the search using BST ordering to avoid visiting unnecessary subtrees.

### Task 6 — Political Queries

**6a — K-th Most Valiant Knight**  
Determines which knight holds rank `K` by bravery (1 = highest). Uses reverse in-order traversal (Right → Root → Left) counting down to `K`. Displays the full knight record.

**6b — Lowest Common Ancestor (LCA)**  
Given two knight names, finds their deepest common ancestor in the BST. Uses BST ordering for an efficient O(h) descent:
- Both scores < current → go left
- Both scores > current → go right
- Otherwise → current node is the LCA

### Task 7 — Banish a Knight
Removes a knight by name from the BST using standard BST deletion (three cases: leaf, one child, two children). Displays:
- The banished knight's name and bravery score
- The in-order successor/predecessor used to replace them
- The updated register in ascending bravery order

---

## Algorithm Details

### Insert — `O(h)`
Compares `braveryScore` at each node, descending left if smaller or equal, right if larger, until an empty slot is found.

### Search by Score — `O(h)`
Standard BST binary search exploiting the ordering property.

### Search by Name — `O(n)`
Since the tree is ordered by score (not name), a full tree traversal is required.

### Traversals — `O(n)`
All three traversals visit every node exactly once.

### Range Query — `O(h + k)` where k = results found
Prunes left subtree if `currentScore < L`, prunes right subtree if `currentScore > R`.

### K-th Most Valiant — `O(h + k)`
Reverse in-order traversal counts down from the largest score.

### LCA — `O(h)`
Single root-to-node descent using BST ordering.

### Remove — `O(h)`
Three cases:
1. **Leaf** — detach from parent
2. **One child** — copy in-order predecessor/successor data, remove that node
3. **Two children** — same as case 2 (find in-order predecessor via left subtree's rightmost node)

> `h` = tree height. For a balanced tree h = O(log n); worst case (sorted input) h = O(n).

---

## Input Format

```
N                                   // number of knights
name1 braveryScore1 house1 battles1 // one knight per line
name2 braveryScore2 house2 battles2
...
houseQuery                          // noble house for Task 3
L R                                 // bravery score range for Task 5
K                                   // rank for Task 6a (k-th most valiant)
nameLCA1 nameLCA2                   // two knights for LCA (Task 6b)
banishName                          // knight to banish (Task 7)
```

---

## Output Format

```
=== 1 ===

=== 2 ===
In-order (Classement du Tournoi):
 Knight1(score) Knight2(score) ... (5 per line)
Pre-order (Proclamation Royale):
 ...
Post-order (Collecte des Tributs):
 ...

=== 3 ===
Chevaliers de la Maison <house> (bravoure croissante):
 <name> | bravoure : <score> | batailles remportees : <battles>
Nouvelle Recrue (bravoure la plus basse): <name> | <score> | <house> | <battles> batailles
Champion du Royaume (plus eleve): <name> | bravoure: <score> | <house> | <battles> batailles

=== 4 ===
Hauteur des Archives: <height>
Noeuds feuilles (chevaliers sans subordonnes): <count>
 <name>(<score>), ...
The archive is balanced.  -- OR --  The archive is unbalanced.

=== 5 ===
Chevaliers avec bravoure dans [L, R] :
 <name>(<score>) | Maison <house> | <battles> batailles
Total de batailles remportees par les chevaliers dans la plage: <total>

=== 6 ===
<K>eme chevalier le plus vaillant: <name> | bravoure: <score> | Maison <house> | <battles> batailles
LCA de <name1> et <name2>:
 <lcaName> | bravoure: <score>

=== 7 ===
<name> (bravoure : <score>) a ete banni du Registre.
(Remplace par le successor in-order : <name>, bravoure : <score>)

Registre apres bannissement (in-order):
 <name>(<score>) ...
```

---

## Example

### Input

```
10
Arthur 50 Pendragon 30
Lancelot 30 Lionel 45
Gawain 70 Pendragon 22
Percival 20 Fisher 15
Tristan 40 Cornwall 28
Galahad 60 Lionel 38
Bedivere 80 Pendragon 19
Bors 25 Lionel 12
Gareth 55 Pendragon 17
Lamorak 35 Cornwall 33
Pendragon
25 60
3
Bors Lamorak
Tristan
```

### Output

```
=== 1 ===

=== 2 ===
In-order (Classement du Tournoi):
 Percival (20)
 Bors (25)
 Lancelot (30)
 Lamorak (35)
 Tristan (40)

 Arthur (50)
 Gareth (55)
 Galahad (60)
 Gawain (70)
 Bedivere (80)

Pre-order (Proclamation Royale):
 Arthur (50)
 Lancelot (30)
 Percival (20)
 Bors (25)
 Tristan (40)

 Lamorak (35)
 Gawain (70)
 Galahad (60)
 Gareth (55)
 Bedivere (80)

Post-order (Collecte des Tributs):
 Bors (25)
 Percival (20)
 Lamorak (35)
 Tristan (40)
 Lancelot (30)

 Gareth (55)
 Galahad (60)
 Bedivere (80)
 Gawain (70)
 Arthur (50)

=== 3 ===
Chevaliers de la Maison Pendragon (bravoure croissante):
 Arthur | bravoure : 50 | batailles remportees : 30
 Gareth | bravoure : 55 | batailles remportees : 17
 Gawain | bravoure : 70 | batailles remportees : 22
 Bedivere | bravoure : 80 | batailles remportees : 19

Nouvelle Recrue (bravoure la plus basse): Percival | 20 | Fisher | 15 batailles
Champion du Royaume (plus eleve): Bedivere | bravoure: 80 | Pendragon | 19 batailles

=== 4 ===
Hauteur des Archives: 4
Noeuds feuilles (chevaliers sans subordonnes): 4
 Bors(25), Lamorak(35), Gareth(55), Bedivere(80)
The archive is balanced.

=== 5 ===
Chevaliers avec bravoure dans [25, 60] :
 Bors(25) | Maison Lionel | 12 batailles
 Lancelot(30) | Maison Lionel | 45 batailles
 Lamorak(35) | Maison Cornwall | 33 batailles
 Tristan(40) | Maison Cornwall | 28 batailles
 Arthur(50) | Maison Pendragon | 30 batailles
 Gareth(55) | Maison Pendragon | 17 batailles
 Galahad(60) | Maison Lionel | 38 batailles
Total de batailles remportees par les chevaliers dans la plage: 203

=== 6 ===
3eme chevalier le plus vaillant: Galahad | bravoure: 60 | Maison Lionel | 38 batailles

LCA de Bors et Lamorak:
 Lancelot | bravoure: 30

=== 7 ===
Tristan (bravoure : 40) a ete banni du Registre.
(Remplace par le successor in-order : Lamorak, bravoure : 35)

Registre apres bannissement (in-order):
 Percival(20) Bors(25) Lancelot(30) Lamorak(35)
 Arthur(50) Gareth(55) Galahad(60) Gawain(70) Bedivere(80)
```

---

## How to Compile & Run

### Prerequisites
- A C++ compiler supporting C++11 or later (`g++`, `clang++`, MSVC)

### Compile

```bash
g++ -std=c++11 -o ex2 ex2.cpp
```

### Run with input file

```bash
./ex2 < input.txt
```

### Run interactively

```bash
./ex2
# type input manually
```

---

## Edge Cases Handled

| Situation | Behavior |
|-----------|----------|
| Knight name not found (LCA query) | Prints which name is missing |
| Knight name not found (banish) | Prints appropriate not-found message |
| K out of bounds (k-th query) | Prints `Rang K hors limites.` |
| No knights in range `[L, R]` | Prints `Aucun chevalier dans cette plage.` |
| No knights in queried house | No output lines for that house (silent) |
| Removing the root (only node) | Clears `pinfo`, tree becomes empty |
| Removing a leaf node | Detaches from parent cleanly |
| Removing a node with two children | Uses in-order predecessor (data swap + leaf removal) |
| Checking balance on empty subtree | Treated as height 0 — no crash |
| Unbalanced tree input (sorted order) | Correctly detected and reported |
