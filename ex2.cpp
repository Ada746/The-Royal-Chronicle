#include <iostream>
#include <string>

#define MAX_N 1000

using namespace std;

// Represents a single knight's record stored in the BST
struct Knight {
    string name;         // unique first name of the knight
    int braveryScore;    // BST key — unique integer awarded by the King; determines position in tree
    string house;        // noble house the knight serves
    int battlesWon;      // total battles won in the King's name
};

// Generic Binary Search Tree node.
// Each node IS a tree: it holds a pointer to its data (pinfo),
// pointers to left/right children, a parent pointer, and a root pointer.
// The BST is ordered by braveryScore (ascending left to right).
template<typename T> class BinarySearchTree
{
public:
    BinarySearchTree<T>* root;       // pointer to the root node of the whole tree
    BinarySearchTree<T>* left_son;   // left child (braveryScore <= current node's score)
    BinarySearchTree<T>* right_son;  // right child (braveryScore > current node's score)
    BinarySearchTree<T>* parent;     // parent node (NULL if this is the root)
    T* pinfo;                        // pointer to the data stored at this node (NULL if empty)

    // Constructor: creates an empty node (no data, no children)
    BinarySearchTree()
    {
        left_son = right_son = NULL;
        root = this;   // a freshly created node considers itself the root
        pinfo = NULL;
    }

    // Allocate and store a copy of 'info' at this node
    void setInfo(T info)
    {
        pinfo = new T;
        *pinfo = info;
    }

    // Public insert: if the tree is empty, this node becomes the root/first entry;
    // otherwise delegate to the recursive helper
    void insert(T x)
    {
        if (pinfo == NULL)
            setInfo(x);       // first node becomes the root
        else
            insert_rec(x);    // tree already has data — find the correct position
    }

    // Recursive insert: walks the tree comparing braveryScore values
    void insert_rec(T x)
    {
        // compare by braveryScore to decide left or right subtree
        if (x.braveryScore <= pinfo->braveryScore) {
            // go into the left subtree (smaller or equal score)
            if (left_son == NULL) {
                // no left child yet — create a new leaf node here
                left_son = new BinarySearchTree<T>;
                left_son->pinfo = new T;
                *(left_son->pinfo) = x;
                left_son->left_son = left_son->right_son = NULL;
                left_son->parent = this;
                left_son->root = root;   // propagate the root pointer
            }
            else {
                left_son->insert_rec(x); // keep descending left
            }
        }
        else {
            // go into the right subtree (larger score)
            if (right_son == NULL) {
                // no right child yet — create a new leaf node here
                right_son = new BinarySearchTree<T>;
                right_son->pinfo = new T;
                *(right_son->pinfo) = x;
                right_son->left_son = right_son->right_son = NULL;
                right_son->parent = this;
                right_son->root = root;  // propagate the root pointer
            }
            else {
                right_son->insert_rec(x); // keep descending right
            }
        }
    }

    // Search by braveryScore — exploits BST ordering for O(h) lookup
    // Returns the node whose braveryScore matches 'score', or NULL if not found
    BinarySearchTree<T>* find(int score) {
        if (pinfo == NULL)
            return NULL;

        if ((pinfo->braveryScore) == score) {
            return this;  // found
        }

        if (score <= (pinfo->braveryScore)) {
            // target score is smaller — search left subtree
            if (left_son != NULL) {
                return left_son->find(score);
            }
            else {
                return NULL; // not found
            }
        }
        else {
            // target score is larger — search right subtree
            if (right_son != NULL) {
                return right_son->find(score);
            }
            else {
                return NULL; // not found
            }
        }
    }

    // Search by knight name — must traverse the entire tree (O(n)) because
    // the BST is ordered by braveryScore, not by name
    BinarySearchTree<T>* findByName(string name) {
        if (pinfo == NULL) {
            return NULL;
        }
        if (pinfo->name == name) {
            return this;  // found at current node
        }
        // try the left subtree first
        BinarySearchTree<T>* rez = NULL;
        if (left_son != NULL) {
            rez = left_son->findByName(name);
        }
        if (rez != NULL) {
            return rez;  // found in left subtree
        }
        // not in left subtree — try the right subtree
        if (right_son != NULL) {
            return right_son->findByName(name);
        }
        return NULL; // not found in either subtree
    }

    // ── Task 2: three ceremonial traversals ──────────────────────────────────

    // Pre-order traversal: Root → Left → Right
    // Used for the "Royal Proclamation Order" (read aloud from the throne)
    // 'count' tracks how many nodes have been printed; a newline is added every 5
    void preOrderTraversal(int& count) {
        cout << pinfo->name << " (" << pinfo->braveryScore << ")" << endl;
        count++;
        if (count % 5 == 0) {
            cout << "\n";
        }
        if (left_son != NULL) {
            left_son->preOrderTraversal(count);
        }
        if (right_son != NULL) {
            right_son->preOrderTraversal(count);
        }
    }

    // In-order traversal: Left → Root → Right
    // Visits nodes in ascending braveryScore order.
    // Used for the "Tournament Ranking Scroll" (seeding for the annual joust)
    void inOrderTraversal(int& count) {
        if (left_son != NULL) {
            left_son->inOrderTraversal(count);
        }
        cout << pinfo->name << " (" << pinfo->braveryScore << ")" << endl;
        count++;
        if (count % 5 == 0) {
            cout << "\n";
        }
        if (right_son != NULL) {
            right_son->inOrderTraversal(count);
        }
    }

    // Post-order traversal: Left → Right → Root
    // Used for the "Tribute Collection Order" (determines who pays taxes first)
    void postOrderTraversal(int& count) {
        if (left_son != NULL) {
            left_son->postOrderTraversal(count);
        }
        if (right_son != NULL) {
            right_son->postOrderTraversal(count);
        }
        cout << pinfo->name << " (" << pinfo->braveryScore << ")" << endl;
        count++;
        if (count % 5 == 0) {
            cout << "\n";
        }
    }

    // ── Task 3: knights of a given house in ascending bravery order ───────────
    // Uses in-order traversal so results are sorted by braveryScore automatically
    void knightsByHouse(string house) {
        if (left_son != NULL) {
            left_son->knightsByHouse(house);  // visit smaller scores first
        }
        if (pinfo->house == house) {
            // print this knight if they belong to the requested house
            cout << " " << pinfo->name
                 << " | bravoure : " << pinfo->braveryScore
                 << " | batailles remportees : " << pinfo->battlesWon << endl;
        }
        if (right_son != NULL) {
            right_son->knightsByHouse(house); // visit larger scores after
        }
    }

    // Find the node with the minimum braveryScore (leftmost node in the BST)
    BinarySearchTree<T>* findMin() {
        BinarySearchTree<T>* curr = this;
        while (curr->left_son != NULL) {
            curr = curr->left_son; // keep going left until there is no left child
        }
        return curr;
    }

    // Find the node with the maximum braveryScore (rightmost node in the BST)
    BinarySearchTree<T>* findMax() {
        BinarySearchTree<T>* curr = this;
        while (curr->right_son != NULL) {
            curr = curr->right_son; // keep going right until there is no right child
        }
        return curr;
    }

    // ── Task 4: structural report — height, leaves, balance ──────────────────

    // Compute the height of the subtree rooted at this node.
    // Height of a single node = 0; an empty child contributes -1.
    int maximumHeight()
    {
        int leftH  = (left_son  != NULL) ? left_son->maximumHeight()  : -1;
        int rightH = (right_son != NULL) ? right_son->maximumHeight() : -1;
        return (leftH > rightH ? leftH : rightH) + 1;
    }

    // Collect all leaf nodes (nodes with no children) into an array.
    // Leaves represent knights with no subordinates in the tree structure.
    void collectLeaves(T leaves[], int& count) {
        if (left_son == NULL && right_son == NULL) {
            // this node is a leaf — record it and stop recursing
            leaves[count++] = *pinfo;
            return;
        }
        if (left_son != NULL) {
            left_son->collectLeaves(leaves, count);
        }
        if (right_son != NULL) {
            right_son->collectLeaves(leaves, count);
        }
    }

    // Check if the BST is height-balanced (AVL condition: |leftH - rightH| <= 1 at every node).
    // Returns the height of the subtree if balanced, or -1 if unbalanced.
    // Using -1 as a sentinel allows the imbalance flag to propagate up the recursion.
    int checkBalanced() {
        int leftH = (left_son  != NULL) ? left_son->checkBalanced()  : 0;
        if (leftH == -1) {
            return -1; // left subtree is already unbalanced — propagate immediately
        }
        int rightH = (right_son != NULL) ? right_son->checkBalanced() : 0;
        if (rightH == -1) {
            return -1; // right subtree is already unbalanced — propagate immediately
        }
        int diff = leftH - rightH;
        if (diff < -1 || diff > 1) {
            return -1; // height difference exceeds 1 — this subtree is unbalanced
        }
        return (leftH > rightH ? leftH : rightH) + 1; // balanced: return height
    }

    // ── Task 5: range query [L, R] ────────────────────────────────────────────
    // Prints all knights whose braveryScore falls within [L, R] in ascending order.
    // Pruning: only descends into left subtree if current score >= L (could have values in range),
    //          only descends into right subtree if current score <= R.
    void rangeQuery(int L, int R, int& totalBattles) {
        // only go left if there could be qualifying nodes there (score >= L means left subtree might have values >= L)
        if (left_son != NULL && pinfo->braveryScore >= L) {
            left_son->rangeQuery(L, R, totalBattles);
        }
        // print current node if it falls within the range
        if (pinfo->braveryScore >= L && pinfo->braveryScore <= R) {
            cout << " " << pinfo->name << "(" << pinfo->braveryScore << ")"
                 << " | Maison " << pinfo->house
                 << " | " << pinfo->battlesWon << " batailles" << endl;
            totalBattles += pinfo->battlesWon;
        }
        // only go right if there could be qualifying nodes there
        if (right_son != NULL && pinfo->braveryScore <= R) {
            right_son->rangeQuery(L, R, totalBattles);
        }
    }

    // ── Task 6a: k-th most valiant knight ────────────────────────────────────
    // Traverses in reverse in-order (Right → Root → Left) to visit nodes from
    // highest to lowest braveryScore. Increments 'count' at each visit;
    // returns the node when count reaches k.
    BinarySearchTree<T>* kthKnight(int k, int& count) {
        BinarySearchTree<T>* result = NULL;

        // visit right subtree first (higher scores = more valiant)
        if (right_son != NULL) {
            result = right_son->kthKnight(k, count);
            if (result != NULL) {
                return result; // found in right subtree — stop recursing
            }
        }

        // visit current node
        count++;
        if (count == k) {
            return this; // this is the k-th most valiant knight
        }

        // visit left subtree (lower scores)
        if (left_son != NULL) {
            result = left_son->kthKnight(k, count);
            if (result != NULL) {
                return result;
            }
        }
        return NULL; // k is out of bounds
    }

    // ── Task 6b: Lowest Common Ancestor (LCA) ────────────────────────────────
    // Exploits BST ordering: if both scores are smaller than current, LCA is in left subtree;
    // if both are larger, LCA is in right subtree; otherwise, current node is the LCA.
    BinarySearchTree<T>* lca(int score1, int score2) {
        if (pinfo == NULL) {
            return NULL;
        }

        // both scores are smaller — LCA must be in the left subtree
        if (score1 < pinfo->braveryScore && score2 < pinfo->braveryScore) {
            if (left_son != NULL) {
                return left_son->lca(score1, score2);
            }
            return NULL;
        }

        // both scores are larger — LCA must be in the right subtree
        if (score1 > pinfo->braveryScore && score2 > pinfo->braveryScore) {
            if (right_son != NULL) {
                return right_son->lca(score1, score2);
            }
            return NULL;
        }

        // scores split across current node (one left, one right, or one equals current)
        // — this node is the LCA
        return this;
    }

    // ── Task 7: banish (remove) a knight by name ─────────────────────────────
    // Finds the node by name, prints banishment info (including in-order successor
    // or predecessor used to replace it), then calls remove() to restructure the tree.
    void removeByName(string name) {
        BinarySearchTree<T>* node = findByName(name);
        if (node == NULL) {
            cout << "Chevalier '" << name << "' introuvable dans le Registre." << endl;
            return;
        }
        int score    = node->pinfo->braveryScore;
        string nodeName = node->pinfo->name;

        // determine the in-order successor or predecessor for display purposes
        string successorName  = "";
        int    successorScore = -1;

        if (node->right_son != NULL) {
            // in-order successor = leftmost node in the right subtree
            BinarySearchTree<T>* succ = node->right_son->findMin();
            successorName  = succ->pinfo->name;
            successorScore = succ->pinfo->braveryScore;
        }
        else if (node->left_son != NULL) {
            // no right subtree — use in-order predecessor (rightmost of left subtree)
            BinarySearchTree<T>* pred = node->left_son->findMax();
            successorName  = pred->pinfo->name;
            successorScore = pred->pinfo->braveryScore;
        }

        node->remove(); // restructure the BST

        cout << nodeName << " (bravoure : " << score << ") a ete banni du Registre." << endl;
        if (successorScore != -1) {
            cout << "(Remplace par le successor in-order : " << successorName
                 << ", bravoure : " << successorScore << ")" << endl;
        }
    }

    // Core BST node removal — handles three standard cases:
    // Case 1 — Leaf node: simply detach from parent (or clear root if it is the root)
    // Case 2 — Node with at least one child: copy the in-order predecessor's data
    //           into this node, then recursively remove the predecessor leaf
    void remove()
    {
        BinarySearchTree<T>* p;
        T* paux;

        // ── Case 1: leaf node (no children) ──────────────────────────────────
        if (left_son == NULL && right_son == NULL)
        {
            if (parent == NULL)
            {
                // removing the root when it is the only node — just clear its data
                delete this->pinfo;
                root->pinfo = NULL;
            }
            else
            {
                // detach this leaf from its parent
                if (parent->left_son == this)
                    parent->left_son = NULL;
                else
                    parent->right_son = NULL;

                delete this->pinfo;
                delete this;
            }
        }
        else
        {
            // ── Case 2: node has at least one child ──────────────────────────
            if (left_son != NULL)
            {
                // find the in-order predecessor: rightmost node of the left subtree
                p = left_son;
                while (p->right_son != NULL)
                    p = p->right_son;
            }
            else
            {
                // no left child — find the in-order successor: leftmost of right subtree
                p = right_son;
                while (p->left_son != NULL)
                    p = p->left_son;
            }

            // swap data between current node and the predecessor/successor
            paux      = p->pinfo;
            p->pinfo  = this->pinfo;
            this->pinfo = paux;

            // now remove the predecessor/successor node (which is simpler — at most one child)
            p->remove();
        }
    }

    // Compact in-order traversal used after banishment (Task 7).
    // Prints "name(score)" with a newline every 5 nodes; skips empty root gracefully.
    void inOrderSimple(int& count) {
        if (pinfo == NULL) {
            return; // empty tree — nothing to print
        }
        if (left_son != NULL) {
            left_son->inOrderSimple(count);
        }
        cout << " " << pinfo->name << "(" << pinfo->braveryScore << ")";
        count++;
        if (count % 5 == 0) {
            cout << "\n";
        }
        if (right_son != NULL) {
            right_son->inOrderSimple(count);
        }
    }
};

int main()
{
    // create an empty BST to hold all knights
    BinarySearchTree<Knight>* bst = new BinarySearchTree<Knight>();

    int N;
    cin >> N; // number of knights to register

    // read each knight and insert into the BST ordered by braveryScore
    for (int i = 0; i < N; i++) {
        Knight k;
        cin >> k.name >> k.braveryScore >> k.house >> k.battlesWon;
        bst->insert(k);
    }

    string houseQuery;   // noble house whose knights will be listed (Task 3)
    cin >> houseQuery;

    int L, R;            // bravery score range for the decree query (Task 5)
    cin >> L >> R;

    int K;               // rank for the k-th most valiant query (Task 6a)
    cin >> K;

    string lcaName1, lcaName2;  // two knight names for the LCA query (Task 6b)
    cin >> lcaName1 >> lcaName2;

    string banishName;   // knight to be banished from the register (Task 7)
    cin >> banishName;

    // ── Task 1: graph is drawn manually and placed in the project folder ──────
    cout << endl;
    cout << "=== 1 ===" << endl;
    cout << endl;

    // ── Task 2: three ceremonial traversals ──────────────────────────────────
    cout << "=== 2 ===" << endl;

    cout << "In-order (Classement du Tournoi):" << endl;
    int cnt = 0;
    bst->inOrderTraversal(cnt);
    if (cnt % 5 != 0) {
        cout << endl; // ensure a final newline if count is not a multiple of 5
    }

    cout << "Pre-order (Proclamation Royale):" << endl;
    cnt = 0;
    bst->preOrderTraversal(cnt);
    if (cnt % 5 != 0) {
        cout << endl;
    }

    cout << "Post-order (Collecte des Tributs):" << endl;
    cnt = 0;
    bst->postOrderTraversal(cnt);
    if (cnt % 5 != 0) {
        cout << endl;
    }

    // ── Task 3: knights of a given house + overall min/max ───────────────────
    cout << "=== 3 ===" << endl;
    cout << "Chevaliers de la Maison " << houseQuery << " (bravoure croissante):" << endl;
    bst->knightsByHouse(houseQuery);

    // findMin() goes all the way left; findMax() goes all the way right
    BinarySearchTree<Knight>* minNode = bst->findMin();
    BinarySearchTree<Knight>* maxNode = bst->findMax();

    cout << endl;
    cout << "Nouvelle Recrue (bravoure la plus basse): "
         << minNode->pinfo->name << " | " << minNode->pinfo->braveryScore
         << " | " << minNode->pinfo->house
         << " | " << minNode->pinfo->battlesWon << " batailles" << endl;

    cout << "Champion du Royaume (plus eleve): "
         << maxNode->pinfo->name << " | bravoure: " << maxNode->pinfo->braveryScore
         << " | " << maxNode->pinfo->house
         << " | " << maxNode->pinfo->battlesWon << " batailles" << endl;

    // ── Task 4: structural report ─────────────────────────────────────────────
    cout << endl;
    cout << "=== 4 ===" << endl;

    int height = bst->maximumHeight();
    cout << "Hauteur des Archives: " << height + 1 << endl; // +1 converts 0-indexed height to node count

    Knight leaves[MAX_N];
    int nrLeaves = 0;
    bst->collectLeaves(leaves, nrLeaves);
    cout << "Noeuds feuilles (chevaliers sans subordonnes): " << nrLeaves << endl;
    cout << " ";
    for (int i = 0; i < nrLeaves; i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << leaves[i].name << "(" << leaves[i].braveryScore << ")";
    }
    cout << endl;

    // checkBalanced() returns -1 if unbalanced, or the height if balanced
    int balanced = bst->checkBalanced();
    if (balanced != -1) {
        cout << "The archive is balanced." << endl;
    }
    else {
        cout << "The archive is unbalanced." << endl;
    }

    // ── Task 5: range query [L, R] ────────────────────────────────────────────
    cout << endl;
    cout << "=== 5 ===" << endl;
    cout << "Chevaliers avec bravoure dans [" << L << ", " << R << "] :" << endl;
    int totalBattles = 0;
    bst->rangeQuery(L, R, totalBattles);

    if (totalBattles == 0) {
        cout << "Aucun chevalier dans cette plage." << endl;
    }
    else {
        cout << "Total de batailles remportees par les chevaliers dans la plage: " << totalBattles << endl;
    }

    // ── Task 6: political queries ─────────────────────────────────────────────
    cout << endl;
    cout << "=== 6 ===" << endl;

    // Task 6a — k-th most valiant knight (reverse in-order, 1-indexed)
    int count6 = 0;
    BinarySearchTree<Knight>* kth = bst->kthKnight(K, count6);
    if (kth != NULL) {
        cout << K << "eme chevalier le plus vaillant: "
             << kth->pinfo->name << " | bravoure: " << kth->pinfo->braveryScore
             << " | Maison " << kth->pinfo->house
             << " | " << kth->pinfo->battlesWon << " batailles" << endl;
    }
    else {
        cout << "Rang " << K << " hors limites." << endl;
    }

    // Task 6b — Lowest Common Ancestor of two knights
    // First resolve names to bravery scores, then call lca() which uses BST ordering
    BinarySearchTree<Knight>* node1 = bst->findByName(lcaName1);
    BinarySearchTree<Knight>* node2 = bst->findByName(lcaName2);
    cout << endl;
    cout << "LCA de " << lcaName1 << " et " << lcaName2 << ":" << endl;

    if (node1 == NULL) {
        cout << " " << lcaName1 << " introuvable dans le Registre." << endl;
    }
    else if (node2 == NULL) {
        cout << " " << lcaName2 << " introuvable dans le Registre." << endl;
    }
    else {
        int s1 = node1->pinfo->braveryScore;
        int s2 = node2->pinfo->braveryScore;
        BinarySearchTree<Knight>* lcaNode = bst->lca(s1, s2);
        if (lcaNode != NULL) {
            cout << " " << lcaNode->pinfo->name
                 << " | bravoure: " << lcaNode->pinfo->braveryScore << endl;
        }
        else {
            cout << " LCA introuvable." << endl;
        }
    }

    // ── Task 7: banish a knight and display the updated register ─────────────
    cout << endl;
    cout << "=== 7 ===" << endl;
    bst->removeByName(banishName);
    cout << endl;
    cout << "Registre apres bannissement (in-order):" << endl;
    int cnt7 = 0;
    if (bst->pinfo != NULL) {
        bst->inOrderSimple(cnt7); // print remaining knights in ascending bravery order
    }
    if (cnt7 % 5 != 0) {
        cout << endl;
    }

    delete bst;

    return 0;
}
